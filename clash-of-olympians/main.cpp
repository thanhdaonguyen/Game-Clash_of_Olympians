

#include "header1.hpp"
#include "Dot.hpp"
#include "LTexture.hpp"
#include "Hero.hpp"
#include "enemy.hpp"
#include "SDLApp.hpp"
#include "reward.hpp"


using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 700;
const char* title = "CLASH OF OLYMPIANS";

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

//Handle collisions of two vector of enemy and dot
void HandleCollisionsSpearAndEnemy(vector<Dot*> dotVec, vector<Enemy*> emVec) {
    for (int i = 0; i < dotVec.size(); i++) {
        for (int j = 0; j < emVec.size(); j++) {
            if (checkRectCollisions(dotVec[i]->mColliders, emVec[j]->mColliders)) {
                dotVec[i]->isTouched = true;
                emVec[j]->takeDamage(dotVec[i]->mDam);
                break;
            }
        }
    }
}

//Scene textures
LTexture gBG(app->GetRenderer(),"assets/background.png");
LTexture gMenuBG(app->GetRenderer(), "assets/menuBackground.jpeg");
LTexture gHPTexture(app->GetRenderer());
LTexture gScore(app->GetRenderer());
LTexture gButton(app->GetRenderer());

//state for the currently hovered button
enum {
    NONE_BUTTON,
    PLAY_BUTTON,
    HIGHSCORE_BUTTON,
    QUIT_BUTTON,
    MENU_BUTTON,
    TRYAGAIN_BUTTON,
    RESTART_BUTTON,
    CONTINUE_BUTTON,
    BACK_BUTTON,
    PAUSE_BUTTON,
};

int buttonState = NONE_BUTTON;

TTF_Font* gFont = TTF_OpenFont( "assets/vinque/vinque.otf", 28 );


//##############################
//CREATE OBJECT

//Event handler
SDL_Event e;
//The hero
Hero myHero(150,440,app->GetRenderer(),"assets/achillies/achillies1.png");
//the dots (weapons) that will be moving on the screen
vector<Dot*> dotVec;
//the dots (bullet) of flying demon and gordon
vector<Dot*> bulVec;
//the reward vector
vector<Reward*> rwdVec;
//enemys
vector<Enemy*> emVec;

//function to add enemies to emVec
void addToemVec(int score, int preScore) {
    if (score < 10) {
        if (score % 3 == 0 && score > preScore) {
            emVec.push_back(new Enemy(1280,600,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png"));
        }
    }
    else if (10 <= score && score < 25) {
        if (score % 5 == 0 && score > preScore) {
            emVec.push_back(new Enemy(1280,600,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png"));
            emVec.push_back(new Enemy(1590,250,app->GetRenderer(),"Flydemon","assets/flyingdemon.png"));
        }
    }
    else if (25 <= score && score < 50) {
        if (score % 5 == 0 && score > preScore) {
            emVec.push_back(new Enemy(1280,600,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png"));
            emVec.push_back(new Enemy(1590,250,app->GetRenderer(),"Flydemon","assets/flyingdemon.png"));
            emVec.back()->isHovering = true;
        }
    }
    else {
        if (score % 5 == 0 && score > preScore) {
            emVec.push_back(new Enemy(1280,600,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png"));
            emVec.push_back(new Enemy(1590,250,app->GetRenderer(),"Flydemon","assets/flyingdemon.png"));
            emVec.back()->isHovering = true;
        }
        if (score % 12 == 0 && score > preScore) {
            emVec.push_back(new Enemy(800,610,app->GetRenderer(),"Shadow","assets/boss/shadow-80x70.png"));
        }
    }
//    emVec.push_back(new Enemy(1280,600,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png"));
//    emVec.push_back(new Enemy(1350,600,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png"));
//    emVec.push_back(new Enemy(1460,600,app->GetRenderer(),"Goblin","assets/goblin/goblinsword.png"));
//    emVec.push_back(new Enemy(1590,250,app->GetRenderer(),"Flydemon","assets/flyingdemon.png"));
//    emVec.push_back(new Enemy(1700,300,app->GetRenderer(),"Flydemon","assets/flyingdemon.png"));
//    emVec.push_back(new Enemy(800,610,app->GetRenderer(),"Shadow","assets/boss/shadow-80x70.png"));
    
}
//function to delete all enemies
void deleteEmVec() {
    int k4 = (int)emVec.size();
    for (int i = k4 - 1; i >= 0; i--) {
        delete emVec[i];
        emVec.erase(emVec.begin() + i);
        emVec.shrink_to_fit();
        
    }
}
//function to delete all spear
void deleteSpear() {
    int k4 = (int)dotVec.size();
    for (int i = k4 - 1; i >= 0; i--) {
        delete dotVec[i];
        dotVec.erase(dotVec.begin() + i);
        dotVec.shrink_to_fit();
    }
}
//function to delete all bullet from demons
void deleteBulVec() {
    int k4 = (int)bulVec.size();
    for (int i = k4 - 1; i >= 0; i--) {
        delete bulVec[i];
        bulVec.erase(bulVec.begin() + i);
        bulVec.shrink_to_fit();
        
    }
}
//function to delete all rewards
void deleteRwdVec() {
    int k4 = (int)rwdVec.size();
    for (int i = k4 - 1; i >= 0; i--) {
        delete rwdVec[i];
        rwdVec.erase(rwdVec.begin() + i);
        rwdVec.shrink_to_fit();
    }
}

//The force bar
SDL_Rect forcebarbound ={198, 548, 104, 12};
SDL_Rect forcebar = {200, 550, 0, 8};
bool forceHold = false;
//the HP bar
int HP = 100;
SDL_Color scoreTextColor = { 85, 117, 149 };
//The music
Mix_Music *gMusic = Mix_LoadMUS( "assets/maintheme.mp3" );
Mix_Chunk *gThrow = Mix_LoadWAV( "assets/throwsound.mp3" );
//the timer for throwing spear
Uint32 mTimer = 0;
//the sign for game state
bool isPlaying = false;
bool isMenu = true;
bool isHighscore = false;
bool isPause = false;
bool isDied = false;
//the score mile stone
Uint32 scoreTimer = 0;
Uint32 scoreTimer0 = 0;
//the score
int score = 0;
int preScore = 0;
//the highscore

vector<int> hScoreVec;
void readHighScoreFromFile() {
    ifstream fin("assets/highscore.txt");
    hScoreVec.clear();
    string line;
    for (int i = 0; i < 3; i++) {
        getline(fin, line);
        int temp = stoi(line);
//        cout << temp << ' ';
        hScoreVec.push_back(temp);
    }
    sort(hScoreVec.begin(), hScoreVec.end(), [](int a, int b) {
            return a > b;
        });
}
void writeHighScoreToFile() {
    
    ofstream fout("assets/highscore.txt");
    for (int i = 0; i < 3; i++) {
        fout << hScoreVec[i] << endl;
    }
    fout.close();
    
    ifstream fin("assets/highscore.txt");
    for (int i = 0; i < 3; i++) {
        string line;
        getline(fin, line);
    }

}
void addNewScore(int score) {
    hScoreVec.push_back(score);
    cout << score << endl;
    sort(hScoreVec.begin(), hScoreVec.end(), [](int a, int b) {
            return a > b;
        });
    writeHighScoreToFile();
    readHighScoreFromFile();
}

//variables for time compensation
Uint32 timeCompen1;
Uint32 timeCompen2;
//the reward timer
int healthTimer = 0;
int strengthTimer = 0;
//count variable for generating enemies
int countGenerateEnemies = 0;

//Handle collisions of two vector of spear and reward
void HandleCollisionsSpearAndReward(vector<Dot*> dotVec, vector<Reward*> rwdVec) {
    for (int i = 0; i < dotVec.size(); i++) {
        for (int j = 0; j < rwdVec.size(); j++) {
            if (checkRectCollisions(dotVec[i]->mColliders, rwdVec[j]->mColliders)) {
                rwdVec[j]->giveReward(&myHero, HP);
                rwdVec[j]->isTouched = true;
                break;
            }
        }
    }
}

//###############################


//HANDLE ALL EVENTS
void HandleEvent () {
    if (isMenu) {
        
        //popup animation for menu
        buttonState = NONE_BUTTON;
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (475 < x && x < 725) {
            if (486 < y && y < 534) buttonState = PLAY_BUTTON;
            if (541 < y && y < 589) buttonState = HIGHSCORE_BUTTON;
            if (596 < y && y < 644) buttonState = QUIT_BUTTON;
        }
        
        while( SDL_PollEvent( &e ) != 0 ) {
            
            //User requests play game
            if (e.type == SDL_MOUSEBUTTONUP && buttonState == QUIT_BUTTON) {
                app->EndAppLoop();
            }
            if( e.type == SDL_MOUSEBUTTONUP && buttonState == PLAY_BUTTON ) {

//                //reset enemies
//                addToemVec(score, preScore);
//                //reset enemies' states
//                for (int i = 0; i < emVec.size(); i++) {
//                    emVec[i]->isStop = false;
//                    emVec[i]->mCount = 0;
//                }
                
                //delete bullets remain on the screen
                deleteBulVec();
                
                //delete spears remain on the screen
                deleteSpear();
                
                //delete rewards remain on the screen
                deleteRwdVec();
                //reset hero's abilities
                myHero.speed = 1;
                myHero.strength = 3;
                
                //reset score and HP
                HP = 100;
                scoreTimer = SDL_GetTicks();
                scoreTimer0 = SDL_GetTicks();
                //reset state
                isPlaying = true;
                isMenu = false;
            }
            if (e.type == SDL_MOUSEBUTTONUP && buttonState == HIGHSCORE_BUTTON) {
                isHighscore = true;
                isMenu =false;
            }
        }
    }
    
    if (isPlaying) {
        
        buttonState = NONE_BUTTON;
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (1120 < x && x < 1180) {
            if (20 < y && y < 80) buttonState = PAUSE_BUTTON;
        }
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_MOUSEBUTTONDOWN && buttonState == PAUSE_BUTTON ) {
                timeCompen1 = SDL_GetTicks();
                isPlaying = false;
                isPause = true;
                continue;
            }
            
            //aim shots
            if( e.type == SDL_MOUSEBUTTONDOWN) {
                dotVec.push_back(new Dot(180,420,app->GetRenderer(), "spear", "assets/spear.png", &myHero));
                forceHold = true;
                myHero.isHolding = true;
            }
            
            //fire
            if( e.type == SDL_MOUSEBUTTONUP) {
                
                forceHold = false;
                dotVec.back()->DOT_THROW_VEL*=myHero.speed;
                dotVec.back()->mVel = 5 + dotVec.back()->DOT_THROW_VEL*double(forcebar.w)/100;
                forcebar.w = 0;
                myHero.isThrow = true;
                myHero.mCount = 0;
                //throw sound
                Mix_PlayChannel( -1, gThrow, 0 );
                mTimer = SDL_GetTicks();
                
            }
            
            //handle event for spears
            if (dotVec.size() != 0) (dotVec.back())->handleEvent(e);
        }
        
        //set score
        preScore = score;
        scoreTimer = SDL_GetTicks();
        score = (int)(scoreTimer - scoreTimer0)/1000;
        
        //add enemies
        addToemVec(score, preScore);
        
        // force bar
        if (forceHold && forcebar.w < 100) forcebar.w += 2;
        
        //give reward
        if (score % 13 == 0 && score > preScore) {
            rwdVec.push_back(new Reward(rand() % 5 + 500, 0, "health", 20, app->GetRenderer(), "assets/heart.png") );
        }
        
        if (score % 19 == 0 && score > preScore) {
            rwdVec.push_back(new Reward(rand() % 5 + 500, 0, "strength", 0.3, app->GetRenderer(), "assets/strength.png") );
        }
        
        
        //Erase the rewards that are touched
        int k3 = (int)rwdVec.size();
        for (int i = k3 - 1; i >= 0; i--) {
            if (rwdVec[i]->isTouched) {
                delete rwdVec[i];
                rwdVec.erase(rwdVec.begin() + i);
                rwdVec.shrink_to_fit();
            }
        }
        
        //Check the dot vector to delete redundant dots
        int k = (int)dotVec.size();
        for (int i = k - 1; i >= 0; i--) {
            if (dotVec[i]->isTouched) {
                delete dotVec[i];
                dotVec.erase(dotVec.begin() + i);
                dotVec.shrink_to_fit();
            }
        }
        
        //Check the bullet vector to delete redundant dots
        int k2 = (int)bulVec.size();
        for (int i = k2 - 1; i >= 0; i--) {
            if (bulVec[i]->isTouched) {
                HP -= 5;
                delete bulVec[i];
                bulVec.erase(bulVec.begin() + i);
                bulVec.shrink_to_fit();
            }
        }
        //Delete enemies that are eliminated
        int k4 = (int)emVec.size();
        for (int i = k4 - 1; i >= 0; i--) {
            if (emVec[i]->isTouched) {
                delete emVec[i];
                emVec.erase(emVec.begin() + i);
                emVec.shrink_to_fit();
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
        
        //move reward
        for (int i = 0; i < rwdVec.size(); i++) {
            rwdVec[i]->move();
        }
        
        //deal damage
        for (int i = 0; i < emVec.size(); i++) {
            if (emVec[i]->isStop) {
                if (emVec[i]->mType == "Goblin") HP -= emVec[i]->doDamage();
                
                else if (emVec[i]->mType == "Flydemon") {
                    emVec[i]->mTimer ++;
                    if (emVec[i]->mTimer % 60 == 0) {
                        emVec[i]->mTimer = 0;
                        bulVec.push_back(new Dot(emVec[i]->getPosX(),emVec[i]->getPosY() + 5,app->GetRenderer(), "bullet" ,"assets/fireball/bubble_explo1.png", emVec[i]));
                        bulVec.back()->setVel(-10,1);
                        
                    }
                }
                else if (emVec[i]->mType == "Shadow") {
                    emVec[i]->mTimer ++;
                    if (emVec[i]->mTimer % 100 == 0) {
                        emVec[i]->mTimer = 0;
                        bulVec.push_back(new Dot(emVec[i]->getPosX(),emVec[i]->getPosY() + 5,app->GetRenderer(), "bullet" ,"assets/fireball/bubble_explo1.png", emVec[i]));
                        bulVec.back()->setVel(-15,-15);
                        
                    }
                }
            }
        }
        
        //check collisions
        HandleCollisionsSpearAndEnemy(dotVec, emVec);
        HandleCollisionsSpearAndReward(dotVec, rwdVec);
        
        //end game
        if (HP <= 0) {
            HP = 0;
            addNewScore(score);
            isPlaying = false;
            isDied = true;
        }
    }
    
    if (isPause) {
        //popup animation for menu
        buttonState = NONE_BUTTON;
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (475 < x && x < 725) {
            if (316 < y && y < 364) buttonState = CONTINUE_BUTTON;
            if (376 < y && y < 424) buttonState = RESTART_BUTTON;
            if (436 < y && y < 484) buttonState = MENU_BUTTON;
        }
        
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.key.keysym.sym == SDLK_q ) {
                app->EndAppLoop();
            }
            //User requests restart
            if( e.type == SDL_MOUSEBUTTONUP && buttonState == RESTART_BUTTON ) {
                
                addNewScore(score);
                //delete bullets remain on the screen
                deleteBulVec();
                //delette rewards remain on the screen
                deleteRwdVec();
                //delete spears
                deleteSpear();
                //delete old enemies
                deleteEmVec();
                
                //reset hero's abilities
                myHero.speed = 1;
                myHero.strength = 3;
                //reset game state
                HP = 100;
                scoreTimer = SDL_GetTicks();
                scoreTimer0 = SDL_GetTicks();
                isPlaying = true;
                isPause = false;
            }
            //User requests continue
            if( e.type == SDL_MOUSEBUTTONUP && buttonState == CONTINUE_BUTTON ) {
                timeCompen2 = SDL_GetTicks();
                scoreTimer0 += timeCompen2 - timeCompen1;
                isPlaying = true;
                isPause = false;
            }
            
            if( e.type == SDL_MOUSEBUTTONUP && buttonState == MENU_BUTTON ) {
                addNewScore(score);
                //delete bullets remain on the screen
                deleteBulVec();
                //delette rewards remain on the screen
                deleteRwdVec();
                //delete spears
                deleteSpear();
                //delete old enemies
                deleteEmVec();
                isMenu = true;
                isPause = false;
            }
            
        }
    }
    
    if (isDied) {
        //popup animation for menu
        buttonState = NONE_BUTTON;
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (475 < x && x < 725) {
            if (296 < y && y < 344) buttonState = TRYAGAIN_BUTTON;
            if (356 < y && y < 404) buttonState = MENU_BUTTON;
        }
        
        while( SDL_PollEvent( &e ) != 0 ) {
            
            //User requests play game
            if (e.type == SDL_MOUSEBUTTONUP && buttonState == MENU_BUTTON) {
                deleteEmVec();
                deleteSpear();
                deleteRwdVec();
                deleteBulVec();
                isMenu = true;
                isDied = false;
            }
            if (e.type == SDL_MOUSEBUTTONUP && buttonState == TRYAGAIN_BUTTON) {
                
                //delete bullets remain on the screen
                deleteBulVec();
                
                //delete spears remain on the screen
                deleteSpear();
                
                //delete rewards remain on the screen
                deleteRwdVec();
                
                //delete enemies remain on the screen
                deleteEmVec();
                
                //reset hero's abilities
                myHero.speed = 1;
                myHero.strength = 3;
                
                //reset score and HP
                HP = 100;
                scoreTimer = SDL_GetTicks();
                scoreTimer0 = SDL_GetTicks();
                //reset state
                isPlaying = true;
                isDied = false;
            }
        }
    }
    
    if (isHighscore) {
        //popup animation for menu
        buttonState = NONE_BUTTON;
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (475 < x && x < 725) {
            if (426 < y && y < 474) buttonState = BACK_BUTTON;
        }
        
        while( SDL_PollEvent( &e ) != 0 ) {
            
            if (e.type == SDL_MOUSEBUTTONUP && buttonState == BACK_BUTTON) {
                isHighscore = false;
                isMenu = true;
            }
        }
    }
}

//HANDLE ALL RENDERINGS
void HandleRendering () {
    
    if (isMenu) {
        SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 255, 0);
        SDL_RenderClear(app->GetRenderer());
        SDL_RenderCopy(app->GetRenderer(), gMenuBG.mTexture, NULL, NULL);
        scoreTextColor = { 255, 188, 0 };
        
        double a = 1;
        double b = 1;
        double c = 1;
        
        if (buttonState == PLAY_BUTTON) a = 1.1;
        if (buttonState == HIGHSCORE_BUTTON) b = 1.1;
        if (buttonState == QUIT_BUTTON) c = 1.1;
        
        gButton.loadFromFile(app->GetRenderer(), "assets/button/play.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*a/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*a/2) + 160, NULL, a);
        gButton.loadFromFile(app->GetRenderer(), "assets/button/highscore.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*b/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*b/2) + 215, NULL, b);
        gButton.loadFromFile(app->GetRenderer(), "assets/button/quit.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*c/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*c/2) + 270, NULL, c);
        
        
    }
    
    if (isDied) {
        
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
        //render bullets
        for (unsigned long i = 0; i < bulVec.size(); i++) {
            bulVec[i]->render();
        }
        //render reward
        for (unsigned long i = 0; i < rwdVec.size(); i++) {
            rwdVec[i]->render();
        }
        
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
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Your score: " + to_string(score), scoreTextColor);
        gScore.render(app->GetRenderer(), 20, 5);
        
        
        //render pause menu text
        gButton.loadFromFile(app->GetRenderer(), "assets/tabletemplate.jpg");
        gButton.render(app->GetRenderer(),(SCREEN_WIDTH - gButton.getWidth())/2, (SCREEN_HEIGHT - gButton.getHeight())/2);
//        scoreTextColor = { 255, 188, 0 };
//        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "YOU LOSE!", scoreTextColor);
//        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2));
//        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Press T to try again, M for menu", scoreTextColor);
//        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2) + 50);
        
        //render buttons
        double a = 1;
        double b = 1;
        
        if (buttonState == TRYAGAIN_BUTTON) a = 1.1;
        if (buttonState == MENU_BUTTON) b = 1.1;
        
        gButton.loadFromFile(app->GetRenderer(), "assets/button/tryagain.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*a/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*a/2) - 30, NULL, a);
        gButton.loadFromFile(app->GetRenderer(), "assets/button/menu.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*b/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*b/2) + 30, NULL, b);
       
    }
    
    if (isPlaying) {
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
        //render bullets
        for (unsigned long i = 0; i < bulVec.size(); i++) {
            bulVec[i]->render();
        }
        //render reward
        for (unsigned long i = 0; i < rwdVec.size(); i++) {
            rwdVec[i]->render();
        }
        
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
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Your score: " + to_string(score), scoreTextColor);
        gScore.render(app->GetRenderer(), 20, 5);
        //render pause button
        //render button
        double a = 1;
        if (buttonState == PAUSE_BUTTON) a = 1.1;
        
        gButton.loadFromFile(app->GetRenderer(), "assets/button/pause2.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH - 50 - gButton.getWidth()*a/2), (50 - gButton.getHeight()*a/2), NULL, a);
        //render game guide
        if (score < 5) {
            scoreTextColor = { 255, 188, 0 };
            gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Press MOUSE on the screen to aim shots!", scoreTextColor);
            gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2));
        }
    }
    //end game menu
    if (isPause) {
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
        //render bullets
        for (unsigned long i = 0; i < bulVec.size(); i++) {
            bulVec[i]->render();
        }
        //render reward
        for (unsigned long i = 0; i < rwdVec.size(); i++) {
            rwdVec[i]->render();
        }
        
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
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "Your score: " + to_string(score), scoreTextColor);
        gScore.render(app->GetRenderer(), 20, 5);
        
        //render pause table
        gButton.loadFromFile(app->GetRenderer(), "assets/tabletemplate.jpg");
        gButton.render(app->GetRenderer(),(SCREEN_WIDTH - gButton.getWidth())/2, (SCREEN_HEIGHT - gButton.getHeight())/2);
        
        gButton.loadFromFile(app->GetRenderer(), "assets/button/PAUSE.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()/2), (SCREEN_HEIGHT/2 - gButton.getHeight()/2) - 100);
        //render buttons
        double a = 1;
        double b = 1;
        double c = 1;
        
        if (buttonState == CONTINUE_BUTTON) a = 1.1;
        if (buttonState == RESTART_BUTTON) b = 1.1;
        if (buttonState == MENU_BUTTON) c = 1.1;

        
        gButton.loadFromFile(app->GetRenderer(), "assets/button/continue.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*a/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*a/2) - 10, NULL, a);
        gButton.loadFromFile(app->GetRenderer(), "assets/button/restart.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*b/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*b/2) + 50, NULL, b);
        gButton.loadFromFile(app->GetRenderer(), "assets/button/menu.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*c/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*c/2) + 110, NULL, c);
    }
    //highscore
    if (isHighscore) {
        SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 255, 0);
        SDL_RenderClear(app->GetRenderer());
        SDL_RenderCopy(app->GetRenderer(), gMenuBG.mTexture, NULL, NULL);
        
        
        //render highscore board
        gButton.loadFromFile(app->GetRenderer(), "assets/tabletemplate.jpg");
        gButton.render(app->GetRenderer(),(SCREEN_WIDTH - gButton.getWidth())/2, (SCREEN_HEIGHT - gButton.getHeight())/2);
        
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "1st place:     " + to_string(hScoreVec[0]), scoreTextColor);
        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2) -80);
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "2nd place:     " + to_string(hScoreVec[1]), scoreTextColor);
        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2) -30);
        gScore.loadFromRenderedText(app->GetRenderer(), gFont, "3th place:     " + to_string(hScoreVec[2]), scoreTextColor);
        gScore.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gScore.getWidth()/2), (SCREEN_HEIGHT/2 - gScore.getHeight()/2) +20);
        
        //render button
        double a = 1;
        if (buttonState == BACK_BUTTON) a = 1.1;
        
        gButton.loadFromFile(app->GetRenderer(), "assets/button/back.png");
        gButton.render(app->GetRenderer(), (SCREEN_WIDTH/2 - gButton.getWidth()*a/2), (SCREEN_HEIGHT/2 - gButton.getHeight()*a/2) + 100, NULL, a);
    }
    //Play music
    if( Mix_PlayingMusic() == 0 ) Mix_PlayMusic( gMusic, -1 );

}

//close the program
void close() {

    //Free loaded images
    
    gBG.free();
    gHPTexture.free();
    gScore.free();
    gMenuBG.free();
    gButton.free();
    
    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    //Free music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    Mix_FreeChunk(gThrow);
    gThrow = NULL;
    
    //delete dynamically allocated objects
    delete app;
    deleteEmVec();
    deleteSpear();
    deleteBulVec();
    deleteRwdVec();
    
}


int main( int argc, char* args[] ) {
    
    readHighScoreFromFile();
    
    //create random seed
    srand((unsigned int)time(NULL));
    
    //add enemy to emVec vector
    //set function for app
    app->SetEventCallback(HandleEvent);
    app->SetRenderCallback(HandleRendering);
    
    //Run game loop
    app->RunAppLoop();
     
    //Free resources and close SDL
    close();

    return 0;
}



