//
//  Dot.cpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 22/03/2023.
//

#include "Dot.hpp"

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

void Dot::render(SDL_Renderer*& renderer, LTexture& dotTexture) {
    //Show the dot
    dotTexture.render (renderer, mPosX, mPosY);
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
