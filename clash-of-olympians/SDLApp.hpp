//
//  SDLApp.hpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 07/04/2023.
//

#ifndef SDLApp_hpp
#define SDLApp_hpp

#include "header1.hpp"
#include <functional>
#include <stdio.h>

class SDLApp {
private:
    //Pointer to window and renderer
    SDL_Window* mWindow = NULL;
//    SDL_Renderer* mRenderer = NULL;
    
    //Check if app is quited or not
    bool mQuit = false;
    
    //Callback function to handle events and rendering
    std::function<void()> mEventCallback;
    std::function<void()> mRenderCallback;
    std::function<void()> mGameObjects;
    
public:
    SDL_Renderer* mRenderer = NULL;
    
    //Constructor
    SDLApp(int imgFlag, const char* title, int x, int y, int w, int h);

    //Destructor
    ~SDLApp();
    
    //Get the renderer
    SDL_Renderer* GetRenderer();

    //Handle events and rendering
    void SetEventCallback(std::function<void()> func);
    void SetRenderCallback(std::function<void()> func);

    //Function for starting and ending app loop
    void RunAppLoop();
    void EndAppLoop();
    
    
};


#endif /* SDLApp_hpp */

