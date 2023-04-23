//
//  reward.hpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 14/04/2023.
//

#ifndef reward_hpp
#define reward_hpp

#include <stdio.h>
#include "header1.hpp"
#include <iostream>
#include "LTexture.hpp"
#include "Hero.hpp"


class Reward {
public:
    
    double maxVel;
    //logic value to check if the reward is earned
    bool isTouched = false;
    //all the value that the reward carry
    double mStrength = 0;
    double mHealth = 0;
    

    //the constructor
    Reward(int x, int y, std::string type, double value, SDL_Renderer*& renderer, std::string img);
    //the deconstructor
    ~Reward();
    //move
    void move();
    //render function
    void render();
    //function to give the player the reward
    void giveReward(Hero* mHero, int& HP);
    //Dot's collision boxes
    std::vector<SDL_Rect> mColliders;
    //Moves the collision boxes relative to the reward's position
    void shiftColliders();
    
    
private:
    
    //the position
    double mPosX, mPosY;
    //the velocities
    double mVelX, mVelY;
    //
    LTexture* mLTexture = NULL;
    SDL_Renderer* mRenderer = NULL;
};



#endif /* reward_hpp */
