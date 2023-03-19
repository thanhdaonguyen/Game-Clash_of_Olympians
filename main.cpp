#include "header1.hpp"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


//Texture wrapper class
class LTexture {
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //Load image at specified path
    bool loadFromFile( std::string path );

    //Create image from font string
    bool loadFromRenderedText (std::string text, SDL_Color textcolor);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    //modulation
    void setColor( Uint8 r, Uint8 g, Uint8 b);

    //Setblending mode
    void setBlendMod ( SDL_BlendMode blending);

    //Blend the texture
    void blendTexture ( Uint8 alpha );

    //Get image dimensions
    int getWidth();
    int getHeigt();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

//The dot that will move around on the screen
class Dot {
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_THROW_VEL = 10;
    
    //Maximum axis velocity of the dot
    static const int DOT_VEL = 3;
    
    //Initializes the variables
    Dot(int x, int y);
    
    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);
    
    //Moves the dot
    void move();
    
    //Shows the dot on the screen
    void render();
    
    //Get the collision boxes
    std::vector<SDL_Rect>& getColliders();
    
private:
    //The X and Y offsets of the dot
    double mPosX, mPosY;
    
    //The velocity of the dot
    double mVelX, mVelY;
    
    //Dot's collision boxes
    std::vector<SDL_Rect> mColliders;
    
    //Moves the collision boxes relative to the dot's position
    void shiftColliders();
};

//The hero
class Hero {
    
public:
    
    //Initialize the hero
    Hero(int x, int y);
    
//    //Takes key presses and adjust state
//    void handleEvent(SDL_Event& e);
    
    //Get the heroimage
    void getImage(std::string path);
    
    //Show the hero on the screen
    void render();
    
private:
    //texture that contain of the hero
    string hTexture;
    
    //image's positions
    int hPosX;
    int hPosY;
    
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gHero;


//Ltexture's functions
LTexture::LTexture() {
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std:: string path ) {

    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL ) {
        printf( "Unable to load image from %s. Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else {
        // Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 255, 255, 255));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == NULL) {
            printf("Unable to create texture from surface %s, SDL_Error: %s", path.c_str(), SDL_GetError());
        }
        else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight =  loadedSurface->h;
        }

        //Get rid of load loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free() {
    //Free texture if it exists
    if( mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight};
    

    //Set clip rendering dimensions
    if(clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
        
    }

    //Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
        
}

void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b) {
    //Modulate texture
    SDL_SetTextureColorMod(mTexture, r, g, b);
}

void LTexture::setBlendMod(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::blendTexture(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeigt() {
    return mHeight;
}


//Dot functions
Dot::Dot(int x, int y) {
    //Initialize the offsets
    mPosX = x;
    mPosY = y;
    
    //Initialize the velocity
    mVelY = 0;
    mVelX = 0;
    
    //Create the necessary SDL_Rects;
    mColliders.resize(11);
    
    //Initialize the collision boxes's width and height
    mColliders[ 0 ].w = 6;
    mColliders[ 0 ].h = 1;

    mColliders[ 1 ].w = 10;
    mColliders[ 1 ].h = 1;

    mColliders[ 2 ].w = 14;
    mColliders[ 2 ].h = 1;

    mColliders[ 3 ].w = 16;
    mColliders[ 3 ].h = 2;

    mColliders[ 4 ].w = 18;
    mColliders[ 4 ].h = 2;

    mColliders[ 5 ].w = 20;
    mColliders[ 5 ].h = 6;

    mColliders[ 6 ].w = 18;
    mColliders[ 6 ].h = 2;

    mColliders[ 7 ].w = 16;
    mColliders[ 7 ].h = 2;

    mColliders[ 8 ].w = 14;
    mColliders[ 8 ].h = 1;

    mColliders[ 9 ].w = 10;
    mColliders[ 9 ].h = 1;

    mColliders[ 10 ].w = 6;
    mColliders[ 10 ].h = 1;
    
    //Initialize colliders relative to position
    shiftColliders();
}

void Dot::handleEvent(SDL_Event &e) {
    //If mouse was pressed
    if (e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        mVelX = (double)DOT_THROW_VEL*(x - DOT_WIDTH/2 - mPosX)/sqrt((x - mPosX)*(x - mPosX) + (y - mPosY)*(y - mPosY));
        mVelY = (double)DOT_THROW_VEL*(y - DOT_HEIGHT/2 - mPosY)/sqrt((x - mPosX)*(x - mPosX) + (y - mPosY)*(y - mPosY));
        
    }
}

void Dot::move() {
    //Move the dot left or right
    mPosX += mVelX;
    //move the dot up or down
    mPosY += mVelY;
    shiftColliders();
    
    //If the dot went too far
    if ( mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH) {
        //move back
        //mPosX -= mVelX;
        mPosX = 150;
        mPosY = 100;
        mVelX = 0;
        mVelY = 0;
        shiftColliders();
    }
    
    //If the dot went too far
    if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT) {
        //move back
        //mPosY -= mVelY;
        mPosX = 150;
        mPosY = 100;
        mVelX = 0;
        mVelY = 0;
        shiftColliders();
    }
}

void Dot::render() {
    //Show the dot
    gDotTexture.render (mPosX, mPosY);
}

void Dot::shiftColliders() {
    //The row offset
    int r = 0;
    
    //Go through the dot's collision boxes
    for (int set = 0; set < mColliders.size(); set++) {
        //Center the collision box
        mColliders[set].x = mPosX + (DOT_WIDTH - mColliders[set].w)/2;
        
        //Set the collision box at its row offset
        mColliders[set].y = mPosY + r;
        
        //Move the row offset down the height of the collision box
        r += mColliders[set].h;
    }
}

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {

        //Set texture filtering to linear
        if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf ("Warning: Linear texture filtering not enabled");
        }

        //Create window
        gWindow = SDL_CreateWindow( "Clash of Olympians", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else {
            //create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s/n", SDL_GetError());
                success = false;
            }
            else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("Renderer could not be created! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;
    
    if(!gDotTexture.loadFromFile("26_motion/dot.bmp")) {
        printf ("Failed to load dot image");
        success = false;
    }
    
    return success;
}

void close() {

    //Free loaded images
    gDotTexture.free();
    gHero.free();

    //destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


//Hero functions
Hero::Hero(int x, int y) {
    //get the hero's position
    hPosX = x;
    hPosY = y;
}

void Hero::render() {
    gHero.render(hPosX, hPosY);
    gHero.loadFromFile(hTexture);
};

void Hero::getImage(std::string path) {
    hTexture = path;
}


int main( int argc, char* args[] ) {


    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        //Load media
        if( !loadMedia() ) {
            printf( "Failed to load media!\n" );
        }
        else {
            //Main loop flag
            bool quit = false;
            
            //Event handler
            SDL_Event e;
            
            //the dot that will be moving on the screen
            Dot dot(150,100);
            
            //The hero
            Hero myHero(100,100);
            myHero.getImage("Shieldmaiden/2x/idle_1.png");
            
            
            //While application is running
            while( !quit ) {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 ) {
                    //User requests quit
                    if( e.key.keysym.sym == SDLK_ESCAPE ) {
                        quit = true;
                    }
                    dot.handleEvent(e);
                }
                
                //move the dot
                dot.move();
                
                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                
                //render objects
                dot.render();
                myHero.render();
                
                //Present
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    

    //Free resources and close SDL
    close();

    return 0;
}




