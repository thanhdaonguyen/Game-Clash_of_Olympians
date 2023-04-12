

#include "header1.hpp"
#include "Dot.hpp"
#include "LTexture.hpp"
#include "Hero.hpp"
#include "enemy.hpp"
#include "SDLApp.hpp"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 700;
const char* title = "Flappy Bird";

SDLApp* app = new SDLApp(IMG_INIT_PNG, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT);
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Check collisions of two vector of Rects
bool checkRectCollisions(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b) {
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Go through the A boxes
    for( int Abox = 0; Abox < a.size(); Abox++ )
    {
        //Calculate the sides of rect A
        leftA = a[ Abox ].x;
        rightA = a[ Abox ].x + a[ Abox ].w;
        topA = a[ Abox ].y;
        bottomA = a[ Abox ].y + a[ Abox ].h;

        //Go through the B boxes
        for( int Bbox = 0; Bbox < b.size(); Bbox++ )
        {
            //Calculate the sides of rect B
            leftB = b[ Bbox ].x;
            rightB = b[ Bbox ].x + b[ Bbox ].w;
            topB = b[ Bbox ].y;
            bottomB = b[ Bbox ].y + b[ Bbox ].h;

            //If no sides from A are outside of B
            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                //A collision is detected
                return true;
            }
        }
    }

    //If neither set of collision boxes touched
    return false;
}

//Check collisions of two vector of enemy and dot
void checkCollisions(vector<Dot*> dotVec, vector<Enemy*> emVec) {
    for (int i = 0; i < dotVec.size(); i++) {
        for (int j = 0; j < emVec.size(); j++) {
            if (checkRectCollisions(dotVec[i]->mColliders, emVec[j]->mColliders)) {
                dotVec[i]->isTouched = true;
                emVec[j]->isTouched = true;
                break;
            }
        }
    }
}

//Scene textures
LTexture gBG(app->GetRenderer(),"assets/background.png");
LTexture gHPTexture(app->GetRenderer());
LTexture gScore(app->GetRenderer());

TTF_Font* gFont = TTF_OpenFont( "assets/vinque/vinque.otf", 28 );


//##############################
//CREATE OBJECT

//Event handler
SDL_Event e;
//The hero
Hero myHero(150,440,app->GetRenderer(),"assets/Shieldmaiden/4x/idle_1.png");
//the dots (weapons) that will be moving on the screen
vector<Dot*> dotVec;
//the dots (bullet) of flying demon
vector<Dot*> bulVec;

//enemys
vector<Enemy*> emVec;
Enemy gob1(1280,620,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png");
Enemy gob2(1350,620,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png");
Enemy gob3(1460,620,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png");
Enemy fly1(1590,250,app->GetRenderer(),"Flydemon","assets/flyingdemon.png");
Enemy fly2(1700,300,app->GetRenderer(),"Flydemon","assets/flyingdemon.png");

void addToemVec() {
    emVec.push_back(&gob1);
    emVec.push_back(&fly1);
    emVec.push_back(&gob2);
    emVec.push_back(&fly2);
    emVec.push_back(&gob3);
}
//The force bar
SDL_Rect forcebarbound ={198, 548, 104, 14};
SDL_Rect forcebar = {200, 550, 0, 10};
bool forceHold = false;
//the HP bar
int HP = 100;
SDL_Color scoreTextColor = { 85, 117, 149 };
//The music
Mix_Music *gMusic = Mix_LoadMUS( "assets/maintheme.mp3" );
Mix_Chunk *gThrow = Mix_LoadWAV( "assets/throwsound.mp3" );
//the timer for throwing spear
Uint32 mTimer = 0;
//the sign for playing
bool isPlaying = true;
//the score mile stone
Uint32 score = 0;
Uint32 score0 = 0;



//###############################


//HANDLE ALL EVENTS
void HandleEvent () {
    
    if (isPlaying) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.key.keysym.sym == SDLK_ESCAPE ) {
                app->EndAppLoop();
            }
            if( e.type == SDL_MOUSEBUTTONDOWN) {
                dotVec.push_back(new Dot(180,420,app->GetRenderer(), "spear", "assets/spear.png"));
                forceHold = true;
                myHero.isHolding = true;
            }
            
            if( e.type == SDL_MOUSEBUTTONUP) {
                
                forceHold = false;
                dotVec.back()->mVel = 5 + dotVec.back()->DOT_THROW_VEL*double(forcebar.w)/100;
                forcebar.w = 0;
                myHero.isThrow = true;
                myHero.mCount = 0;
                //throw sound
                Mix_PlayChannel( -1, gThrow, 0 );
                mTimer = SDL_GetTicks();
                
            }
            
            if (dotVec.size() != 0) (dotVec.back())->handleEvent(e);
        }
        
        
        // force bar
        if (forceHold && forcebar.w < 100) forcebar.w += 2;
        
        
        //Check the dot vector to delete redundant dots
        int k = (int)dotVec.size();
        for (int i = k - 1; i >= 0; i--) {
            if (dotVec[i]->isTouched) {
                delete dotVec[i];
                dotVec.erase(dotVec.begin() + i);
                dotVec.shrink_to_fit();
            }
        }
        
        //Check the bul vector to delete redundant dots
        int k2 = (int)bulVec.size();
        for (int i = k2 - 1; i >= 0; i--) {
            if (bulVec[i]->isTouched) {
                HP -= 5;
                delete bulVec[i];
                bulVec.erase(bulVec.begin() + i);
                bulVec.shrink_to_fit();
            }
        }

        
        
        // move dots
        for (int i = 0; i < dotVec.size(); i++) {
            
            dotVec[i]->move();
            dotVec[i]->changeAngle();
        }
        
        //move bul
        for (int i = 0; i < bulVec.size(); i++) {
            bulVec[i]->move();
            bulVec[i]->changeAngle();
        }
       
        
        //move enemys
        for (int i = 0; i < emVec.size(); i++) {
            emVec[i]->move();
        }
        
        //deal damage
        for (int i = 0; i < emVec.size(); i++) {
            if (emVec[i]->isStop) {
                if (emVec[i]->mType == "Goblin") HP -= emVec[i]->doDamage();
                else if (emVec[i]->mType == "Flydemon") {
                    emVec[i]->mTimer ++;
                    if (emVec[i]->mTimer % 60 == 0) {
                        emVec[i]->mTimer = 0;
                        bulVec.push_back(new Dot(emVec[i]->getPosX(),emVec[i]->getPosY(),app->GetRenderer(), "bullet" ,"assets/fireball/bubble_explo1.png"));
                        bulVec.back()->setVel(-5,1);
                    }
                }
            }
        }
        
        //check collisions
        checkCollisions(dotVec, emVec);
    }
    
    else {
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.key.keysym.sym == SDLK_q ) {
                app->EndAppLoop();
            }
            if( e.key.keysym.sym == SDLK_r ) {
                gob1.setPos(1280,620);
                gob2.setPos(1350,620);
                gob3.setPos(1460,620);
                fly1.setPos(1590,250);
                fly2.setPos(1700,300);
                for (int i = 0; i < emVec.size(); i++) {
                    emVec[i]->isStop = false;
                }
                HP = 100;
                score = SDL_GetTicks();
                score0 = SDL_GetTicks();
                isPlaying = true;
            }
            
        }
    }
    
    
}

//HANDLE ALL RENDERINGS
void HandleRendering () {
    
    cout << bulVec.size() << endl;
    //Clear screen
    SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(app->GetRenderer());
    
    //render background
    SDL_RenderCopy(app->GetRenderer(), gBG.mTexture, NULL, NULL);
    //render hero
    
    myHero.render();
    //render dots;
    unsigned long k1 = 0, k2 = dotVec.size();
    for(unsigned long i = k1; i < k2; i++) {
        dotVec[i]->render();
    }
    
    //render enemy
    for (unsigned long i = 0; i < emVec.size(); i++) {
        //        SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 255, 255);
        //        SDL_RenderFillRect(app->GetRenderer(), &emVec[i]->mColliders[0]);
        emVec[i]->render();
    }
    //render bullet
    for (unsigned long i = 0; i < bulVec.size(); i++) {
        bulVec[i]->render();
    }
    //render b
    
    //render force bar
    SDL_SetRenderDrawColor(app->GetRenderer(), 140, 140, 140, 0); // set color to blue
    SDL_RenderFillRect(app->GetRenderer(), &forcebarbound); // draw filled rectangle
    SDL_SetRenderDrawColor(app->GetRenderer(), 218, 138, 47, 0); // set color to blue
    SDL_RenderFillRect(app->GetRenderer(), &forcebar); // draw filled rectangle
    
    //Render HP to the screen
    scoreTextColor = { 85, 117, 149 };
    gHPTexture.loadFromRenderedText(app->GetRenderer(), gFont, to_string(HP), scoreTextColor);
    gHPTexture.render(app->GetRenderer(), 75, 55);
    //Render score to the screen
    if (isPlaying == true) score = SDL_GetTicks();
    gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Your score: " + to_string(int((score - score0)/1000)), scoreTextColor);
    gScore.render(app->GetRenderer(), 20, 5);
    //Play music
    if( Mix_PlayingMusic() == 0 ) Mix_PlayMusic( gMusic, -1 );
    
    //end game
    if (HP <= 0) {
        HP = 0;
        isPlaying = false;
    }
    
    if (isPlaying == false) {
        scoreTextColor = { 255, 188, 0 };
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "YOU LOSE!", scoreTextColor);
        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2));
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Press Q to quit game, R to restart", scoreTextColor);
        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2) + 50);
        
    }
}

//close the program
void close() {

    //Free loaded images
    
    gBG.free();
    gHPTexture.free();
    gScore.free();
    
    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    //Free music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    Mix_FreeChunk(gThrow);
    gThrow = NULL;
    
    //Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    
    //delete dynamically allocated objects
    delete app;
}



int main( int argc, char* args[] ) {
    
    //add enemy to emVec vector
    addToemVec();
    //set function for app
    app->SetEventCallback(HandleEvent);
    app->SetRenderCallback(HandleRendering);
    
    //Run game loop
    app->RunAppLoop();
     
    //Free resources and close SDL
    close();

    return 0;
}



