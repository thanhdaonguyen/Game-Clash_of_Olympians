

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

TTF_Font* gFont = TTF_OpenFont( "assets/16_true_type_fonts/lazy.ttf", 28 );


//##############################
//CREATE OBJECT

//Event handler
SDL_Event e;
//The hero
Hero myHero(230,450,app->GetRenderer(),"assets/Shieldmaiden/4x/idle_1.png");
//the dots (weapons) that will be moving on the screen
vector<Dot*> dotVec;
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
SDL_Rect forcebarbound ={268, 428, 64, 14};
SDL_Rect forcebar = {270, 430, 0, 10};
bool forceHold = false;
//the HP bar
int HP = 1000000;
SDL_Color scoreTextColor = { 0, 0, 0 };



//###############################


//HANDLE ALL EVENTS
void HandleEvent () {
    
    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ) {
        //User requests quit
        if( e.key.keysym.sym == SDLK_ESCAPE ) {
            app->EndAppLoop();
        }
        if( e.type == SDL_MOUSEBUTTONDOWN) {
            dotVec.push_back(new Dot(320,470,app->GetRenderer(),"assets/26_motion/dot.bmp"));
            forceHold = true;
        }

        if( e.type == SDL_MOUSEBUTTONUP) {
            forceHold = false;
            dotVec.back()->mVel = dotVec.back()->DOT_THROW_VEL*double(forcebar.w + 9)/60;
            forcebar.w = 0;
        }

        if (dotVec.size() != 0) (dotVec.back())->handleEvent(e);



    }
    // force bar
    if (forceHold && forcebar.w < 60) forcebar.w += 2;


    //Check the dot vector to delete redundant dots
    int k = (int)dotVec.size();
    for (int i = k - 1; i >= 0; i--) {
        if (dotVec[i]->isTouched) {
            delete dotVec[i];
            dotVec.erase(dotVec.begin() + i);
            dotVec.shrink_to_fit();
        }
    }

    // move dots
    for (int i = 0; i < dotVec.size(); i++) {
        dotVec[i]->move();
    }

    //move enemys
    for (int i = 0; i < emVec.size(); i++) {
        emVec[i]->move();
    }

    //deal damage
    for (int i = 0; i < emVec.size(); i++) {
        if (emVec[i]->isStop) {
            HP -= emVec[i]->doDamage();
        }
    }

    //check collisions
    checkCollisions(dotVec, emVec);
    
}

//HANDLE ALL RENDERINGS
void HandleRendering () {
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
            emVec[i]->render();
    }
    //render force bar
    SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 255, 0); // set color to blue
    SDL_RenderFillRect(app->GetRenderer(), &forcebarbound); // draw filled rectangle
    SDL_SetRenderDrawColor(app->GetRenderer(), 100, 200, 40, 0); // set color to blue
    SDL_RenderFillRect(app->GetRenderer(), &forcebar); // draw filled rectangle

    //Render HP to the screen
    gHPTexture.loadFromRenderedText(app->GetRenderer(), gFont, "Your HP: " + to_string(HP), scoreTextColor);
    gHPTexture.render(app->GetRenderer(), 200, 200);
    //Render score to the screen
    gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Your score: " + to_string(int(SDL_GetTicks()/1000)), scoreTextColor);
    gScore.render(app->GetRenderer(), 200, 250);
    //end game
    if (HP <= 0) {
        HP = 0;
        SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 255, 255);
        SDL_RenderClear(app->GetRenderer());
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "YOU LOSE!", scoreTextColor);
        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2));
        SDL_RenderPresent(app->GetRenderer());
        SDL_Delay(3000);

        app->EndAppLoop();
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



