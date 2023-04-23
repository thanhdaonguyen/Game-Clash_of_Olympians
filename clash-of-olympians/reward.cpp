//
//  reward.cpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 14/04/2023.
//

#include "reward.hpp"


//the constructor
Reward::Reward(int x, int y, std::string type, double value, SDL_Renderer*& renderer, std::string img) {
    //get the hero's position
    mPosX = x;
    mPosY = y;
    
    //set the velocities
    mVelX = 0;
    mVelY = 1;
    
    //Set mLTexture
    mLTexture = new LTexture;
    mLTexture->loadFromFile(renderer, img);
    
    //Set renderer
    mRenderer = renderer;
    
    //set the reward index
    if (type == "health") mHealth = value;
    if (type == "strength") mStrength = value;
    
    //set colliders
    //Create the necessary SDL_Rects;
    mColliders.resize(1);
    
    //Initialize the collision boxes's width and height
    mColliders[ 0 ].w = mLTexture->getWidth();
    mColliders[ 0 ].h = mLTexture->getHeight();
}
Reward::~Reward() {
    delete mLTexture;
}

void Reward::move() {
    mPosX += mVelX;
    mPosY += mVelY;
    
    if (mPosY > 700) {
        isTouched = true;
    }
    
    shiftColliders();
}


void Reward::render() {
    mLTexture->render(mRenderer, mPosX, mPosY);
}

void Reward::giveReward(Hero* mHero, int& HP) {
    mHero->strength += mStrength;
    if(mStrength != 0) mHero->speed += 0.2;
    HP += mHealth;
}

void Reward::shiftColliders() {
    
    //Change the position of colliders related to the position of the object
    mColliders[0].x = mPosX;
    mColliders[0].y = mPosY;
    
}
