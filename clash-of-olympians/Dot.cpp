//
//  Dot.cpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 22/03/2023.
//

#include "Dot.hpp"

//Dot functions
Dot::Dot(int x, int y, SDL_Renderer*& renderer, std::string type, std::string img, Hero* user) {
    //set type
    mType = type;
    
    //Initialize the offsets
    mPosX = x;
    mPosY = y;
    
    
    //Initialize the velocity
    mVelY = 0;
    mVelX = 0;
    
    //Initialize the angle
    mAngle = 45;
    
    //Initialize the damage
    mDam = user->strength;
    
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
    mColliders[ 0 ].w = 5;
    mColliders[ 0 ].h = 5;

    
    //Initialize colliders relative to position
    shiftColliders();
}

//Dot functions
Dot::Dot(int x, int y, SDL_Renderer*& renderer, std::string type, std::string img, Enemy* user) {
    //set type
    mType = type;
    
    //Initialize the offsets
    mPosX = x;
    mPosY = y;
    
    
    //Initialize the velocity
    mVelY = 0;
    mVelX = 0;
    
    //Initialize the angle
    mAngle = 45;
    
    //Initialize the damage
    mDam = user->mDam;
    
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
    mColliders[ 0 ].w = 5;
    mColliders[ 0 ].h = 5;

    
    //Initialize colliders relative to position
    shiftColliders();
}


Dot::~Dot() {
    delete mLTexture;
}

void Dot::handleEvent(SDL_Event &e) {
    //If mouse was pressed
    
    //set velocity
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
    }
    if (isHolding) {
        mAngle = acos(-k1/sqrt((k1*k1 + k2*k2)))*k2/abs(k2)/3.14159*180;
    }
    else {
        mAngle = acos(-mVelX/sqrt((mVelY*mVelY + mVelX*mVelX)))*mVelY/abs(mVelY)/3.14159*180;
    }
    
}

//change the angle when velocity changes
void Dot::changeAngle() {
    mAngle = acos(-mVelX/sqrt((mVelY*mVelY + mVelX*mVelX)))*mVelY/abs(mVelY)/3.14159*180;
}

//move the object
void Dot::move() {
    //Move the dot left or right
    mPosX += mVelX;
    //move the dot up or down
    mPosY += mVelY;
    if (mVelY != 0) mVelY += mGravity;
    shiftColliders();
    if (mType == "spear") {
        
        //If the dot went too far
        if (mPosY > SCREEN_HEIGHT - 40) {
            //move back
            isTouched = true;
        }
    }
    if (mType == "bullet") {
        //if the bullet touches the tower
        if ( mPosX < 110 || mPosX > SCREEN_WIDTH) {
            //move back
            //mPosX -= mVelX;
            isTouched = true;
        }
//        if (mPosY > SCREEN_HEIGHT - 200) {
//            //move back
//            isTouched = true;
//        }
    }
}

void Dot::render() {
    //Show the object
    SDL_RenderFillRect(mRenderer, &mColliders[0]);
    if (mType == "spear") mLTexture->render (mRenderer, mPosX, mPosY, &mClip, 0.5, - mAngle + 180 );
    if (mType == "bullet") mLTexture->render (mRenderer, mPosX, mPosY, &mClip, 1.5, - mAngle + 180 );

}

double Dot::getPosX() {
    return mPosX;
}

double Dot::getPosY() {
    return mPosY;
}

void Dot::shiftColliders() {
    
    //Change the position of colliders related to the position of the object
    mColliders[0].x = mPosX + (mLTexture->getWidth())*0.25*(1 + mVelX/sqrt(mVelX*mVelX + mVelY*mVelY));
    mColliders[0].y = mPosY + (mLTexture->getWidth())*0.25*(mVelY/sqrt(mVelX*mVelX + mVelY*mVelY));
    
}

void Dot::setVel(double x, double y) {
    mVelX = x;
    mVelY = y;
}
