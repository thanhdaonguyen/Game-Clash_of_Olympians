//
//  enemy.hpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 04/04/2023.
//

#ifndef enemy_hpp
#define enemy_hpp

#include <stdio.h>
#include "header1.hpp"
#include "LTexture.hpp"

class Enemy {
public:
    //the velocity of the enemy
    double mVelX;
    double mVelY;
    //the damage of the enemy
    int mDam;
    //timer to check if it do damage
    int mTimer = 0;
    //the counter for rendering images
    int mCount = 0;
    //the number of sprites to render
    int mNumSprite = 0;
    //the health of the enemy
    double health;
    double healthmax;
    
    //the type of the enemy
    std::string mType;
    
    
    //Constructor
    Enemy (int x, int y, SDL_Renderer*& renderer, std::string type, std::string img);
    //Destructor
    ~Enemy();
    //move the enemy
    void move();
    //deal damage to the tower
    int doDamage();
    //deal with the dam it take from the hero
    void takeDamage(double dam);
    //show the enemy on
    void render();
    //Get the coordinates
    double getPosX();
    double getPosY();
    //set the stop position
    void setStop(double x);
    //set position
    void setPos(double x, double y);
    //Moves the collision boxes relative to the position
    void shiftColliders();
    //check if the enemy approached close enough or be touched
    bool isStop = false;
    bool isTouched = false;
    //check if flydemon is hovering
    bool isHovering = false;
    //Dot's collision boxes
    std::vector<SDL_Rect> mColliders;
    //clip
    SDL_Rect mClip;

private:
    //Position of the object
    double mPosX, mPosY;
    //The position where the enemy would stop
    double stopX = 330;
    //LTexture of the object
    LTexture* mLTexture = NULL;
    //renderer of the object
    SDL_Renderer* mRenderer = NULL;
    
        
};

#endif /* enemy_hpp */
