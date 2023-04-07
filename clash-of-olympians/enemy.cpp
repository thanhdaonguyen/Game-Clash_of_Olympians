//
//  enemy.cpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 04/04/2023.
//

#include "enemy.hpp"

using namespace std;

Enemy::Enemy(int x, int y, SDL_Renderer*& renderer, std::string type, std::string img) {
    
    //Initialize the mRenderer
    mRenderer = renderer;
    cout << mRenderer << endl;
    cout << renderer << endl;
    //Initialize the mLTexture
    mLTexture = new LTexture();
    mLTexture->loadFromFile(renderer, img);
    
    //Initialize the coordinates
    mPosX = x;
    mPosY = y;
    //Set dam
    if (type == "Goblin") {
        mDam = 7;
    }
    if (type == "Flydemon") {
        mDam = 5;
    }
    
    //Initialize the velocity
    mVelX = -3;
    mVelY = 0;
    
    //Initialize type
    mType = type;
    
    if (type == "Goblin") {
        mClip = {192, 192, 64, 64};
        //Create the necessary SDL_Rects
        mColliders.resize(1);
        //Initialize the collision boxes's width and height
        mColliders[0].w = 64;
        mColliders[0].h = 64;
        
        shiftColliders();
    }
    if (type == "Flydemon") {
        mClip = {192, 0, 64, 64};
        //Create the necessary SDL_Rects
        mColliders.resize(1);
        //Initialize the collision boxes's width and height
        mColliders[0].w = 64;
        mColliders[0].h = 64;
        
        shiftColliders();
    }
    
    
}

Enemy::~Enemy() {
    delete mLTexture;
}

void Enemy::move() {
    //Move the object
    mPosX += mVelX;
    mPosY += mVelY;
    //change the colliders' positions
    shiftColliders();
    
    //If the enemy is close enough, it will stop
    if ( mPosX < stopX ) {
        mVelY = 0;
        mVelX = 0;
        isStop = true;
    }
    
    if ( isTouched ) {
        isTouched = false;
        isStop = false;
        if (mType == "Goblin") {
            mPosX = 1400;
            mVelX = -3;
        }
        if (mType == "Flydemon") {
            mPosX = 1400;
            mPosY = rand() % (400 - 200 + 1) + 200;
            mVelX = -3;
        }
        mTimer = 0;
        
    }
}
int Enemy::doDamage() {
    mTimer ++;
    if (mTimer % 35 == 0) {
        mTimer = 0;
        return mDam;
    }
    return 0;
}
void Enemy::render() {
    //Show the enemy on the screen
    mLTexture->render(mRenderer, mPosX, mPosY, &mClip);
}

double Enemy::getPosY() {
    return mPosY;
}

double Enemy::getPosX() {
    return mPosX;
}

void Enemy::setStop(double x) {
    stopX = x;
}

void Enemy::shiftColliders() {
    //the row offset
    int r = 0;
    
    //Go through the object's collision boxes
    for (int set = 0; set < mColliders.size(); set++) {
        mColliders[set].x = int(mPosX);
        mColliders[set].y = int(mPosY) + r;
        
        //Move the row offset down the height of the collision box
        r += mColliders[set].h;
    }
    
}
