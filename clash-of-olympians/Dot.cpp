//
//  Dot.cpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 22/03/2023.
//

#include "Dot.hpp"

//Dot functions
Dot::Dot(int x, int y, SDL_Renderer*& renderer, std::string img) {
    //Initialize the offsets
    mPosX = x;
    mPosY = y;
    mcPosX = x;
    mcPosY = y;
    
    //Initialize the velocity
    mVelY = 0;
    mVelX = 0;
    
    //Initialize the angle
    mAngle = 45;
    
    //Initialize mLTexture
    mLTexture = new LTexture;
    mLTexture->loadFromFile(renderer, img);
    //Initialize mRenderer
    mRenderer = renderer;
    
    //Initialize mClip
    mClip = {0, 0, mLTexture->getWidth(), mLTexture->getHeight()};
    
    //Create the necessary SDL_Rects;
    mColliders.resize(1);
    
    //Initialize the collision boxes's width and height
    mColliders[ 0 ].w = 1;
    mColliders[ 0 ].h = 1;

    
    //Initialize colliders relative to position
    shiftColliders();
}

Dot::~Dot() {
    delete mLTexture;
}

void Dot::handleEvent(SDL_Event &e) {
    //If mouse was pressed
    
   
    int x, y;
    SDL_GetMouseState(&x, &y);
    double k1 = mVel*(x - DOT_WIDTH/2 - mPosX)/sqrt((x - mPosX)*(x - mPosX) + (y - mPosY)*(y - mPosY));
    double k2 = mVel*(y - DOT_HEIGHT/2 - mPosY)/sqrt((x - mPosX)*(x - mPosX) + (y - mPosY)*(y - mPosY));
    if (e.type == SDL_MOUSEBUTTONDOWN) {
       isHolding = true;
    }
    
    if (e.type == SDL_MOUSEBUTTONUP) {
        isHolding = false;
        mVelX = k1;
        mVelY = k2;
        std::cout << 1 << std::endl;
    }
    if (isHolding) {
        mAngle = acos(-k1/sqrt((k1*k1 + k2*k2)))*k2/abs(k2)/3.14159*180;
        std::cout << 2 << std::endl;
    }
    else {
        mAngle = acos(-mVelX/sqrt((mVelY*mVelY + mVelX*mVelX)))*mVelY/abs(mVelY)/3.14159*180;
        std::cout << "check" << std::endl;
    }
    
}

void Dot::changeAngle() {
    mAngle = acos(-mVelX/sqrt((mVelY*mVelY + mVelX*mVelX)))*mVelY/abs(mVelY)/3.14159*180;
}

void Dot::move() {
    //Move the dot left or right
    mPosX += mVelX;
    //move the dot up or down
    mPosY += mVelY;
    if (mVelY != 0) mVelY += DOT_THROW_VEL*0.02;
    shiftColliders();
    
    //If the dot went too far
    if ( mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH) {
        //move back
        //mPosX -= mVelX;
        isTouched = true;
    }
    
    //If the dot went too far
    if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT - 40) {
        //move back
        isTouched = true;
    }
}

void Dot::render() {
    //Show the dot
    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 0);
    SDL_RenderFillRect(mRenderer, &mColliders[0]);
    mLTexture->render (mRenderer, mPosX, mPosY, &mClip, 0.5, - mAngle + 180 );
    

}

double Dot::getPosX() {
    return mPosX;
}

double Dot::getPosY() {
    return mPosY;
}

void Dot::shiftColliders() {
    
    //Center the collision box
    mColliders[0].x = mPosX + mLTexture->getWidth()*0.25*(1 + mVelX/sqrt(mVelX*mVelX + mVelY*mVelY));
    
    //Set the collision box at its row offset
    mColliders[0].y = mPosY + mLTexture->getWidth()*0.25*(mVelY/sqrt(mVelX*mVelX + mVelY*mVelY));
    
}
