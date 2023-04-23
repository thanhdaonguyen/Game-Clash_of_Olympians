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
    //Initialize the mLTexture
    mLTexture = new LTexture();
    mLTexture->loadFromFile(renderer, img);
    
    //Initialize the coordinates
    mPosX = x;
    mPosY = y;
    //Set dam
    if (type == "Goblin") {
        mDam = 3;
    }
    if (type == "Flydemon") {
        mDam = 5;
    }
    
    //Initialize the velocity
    if (type == "Goblin") {
        mVelX = -1.2;
        mVelY = 0;
    }
    if (type == "Flydemon") {
        mVelX = -1.5;
        mVelY = 0;
    }

    //initialize health
    if (type == "Goblin") {
        health = 6;
        healthmax = 6;
    }
    if (type == "Flydemon") {
        health = 7;
        healthmax = 7;
    }
    
    
    //Initialize type
    mType = type;
    
    
    //set colliders
    if (type == "Goblin") {
        mClip = {192, 193, 64, 64};
        //Create the necessary SDL_Rects
        mColliders.resize(1);
        //Initialize the collision boxes's width and height
        mColliders[0].w = 30;
        mColliders[0].h = 60;
        
        shiftColliders();
    }
    if (type == "Flydemon") {
        mClip = {0, 0, 64, 64};
        //Create the necessary SDL_Rects
        mColliders.resize(2);
        //Initialize the collision boxes's width and height
        mColliders[0].w = 46;
        mColliders[0].h = 64;
        
        mColliders[1].w = 46;
        mColliders[1].w = 64;
        
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
        isStop = true;
    }
    
    if (isStop == true) {
        mVelX = 0;
        mVelY = 0;
    }
    
    //if the object doesn't stop anymore
    if (isStop == false) {
        if (mType == "Goblin") {
            mVelX = -1.2;
        }
        if (mType == "Flydemon") {
            mVelX = -1.5;
        }
    }
    
    //if the object is touched by spear or comes to the boundary)
    if ( isTouched ) {
        //reset the state
        isTouched = false;
        isStop = false;
        //reset the position
        if (mType == "Goblin") {
            mPosX = rand() % (1800 - 1300 + 1) + 1300;
            mVelX = -1.2;
        }
        if (mType == "Flydemon") {
            mPosX = 1400;
            mPosY = rand() % (400 - 200 + 1) + 200;
            mVelX = -1.5;
        }
        //reset the timer for doing dam
        mTimer = 0;
        //reset the health
        health = healthmax;
    }
    
    //change mCount and mClip
    if (mType == "Goblin") {
        if (!isStop) {
            mClip.x = 64*(mCount/6);
            mCount --;
            if (mCount < 0) mCount = 8*6-1;
        }
        else {
            mClip.x = 64*(mCount/10);
            mCount --;
            if (mCount < 8*10) mCount = 11*10-1;
        }
    }
    
    if (mType == "Flydemon") {
        mClip.x = 64*(mCount/6);
        mCount --;
        if (mCount < 0) mCount = 6*6-1;
    }
}
int Enemy::doDamage() {
    if (mType == "Goblin") {
        mTimer ++;
        if (mTimer % 35 == 0) {
            mTimer = 0;
            return mDam;
        }
    }
    if (mType == "Flydemon") {
        return mDam;
    }
    return 0;
}

void Enemy::takeDamage(double dam) {
    health -= dam;
    if (health < 0) health = 0;
    if (health <= 0) {
        isTouched = true;
    }
}

void Enemy::render() {
    //Show the enemy on the screen
    
    if (mType == "Goblin") {
        SDL_Rect healthbarbound ={int(mPosX) + 40 - 1, int(mPosY) - 10 - 1, 20, 6};
        SDL_Rect healthbar = {int(mPosX) + 40 , int(mPosY) - 10, int(health/healthmax*18), 4};
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderFillRect(mRenderer, &healthbarbound);
        SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(mRenderer, &healthbar);
        mLTexture->render(mRenderer, mPosX, mPosY, &mClip, 1.3);
    }
    if (mType == "Flydemon") {
        SDL_Rect healthbarbound ={int(mPosX) + 25 - 1, int(mPosY) - 1, 20, 6};
        SDL_Rect healthbar = {int(mPosX) + 25 , int(mPosY) , int(health/healthmax*18), 4};
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderFillRect(mRenderer, &healthbarbound);
        SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(mRenderer, &healthbar);
        mLTexture->render(mRenderer, mPosX, mPosY, &mClip, 1.5);
    }
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

//change the position of colliders related to the position of the object
void Enemy::shiftColliders() {
    if (mType == "Goblin") {
        mColliders[0].x = int(mPosX) + 37;
        mColliders[0].y = int(mPosY) + 13;
    }
    if (mType == "Flydemon") {
        mColliders[0].x = int(mPosX) + 23;
        mColliders[0].y = int(mPosY) + 19;
        mColliders[1].x = int(mPosX) + 30;
        mColliders[1].y = int(mPosY) + 50;
    }
}


void Enemy::setPos(double x, double y) {
    mPosX = x;
    mPosY = y;
}
