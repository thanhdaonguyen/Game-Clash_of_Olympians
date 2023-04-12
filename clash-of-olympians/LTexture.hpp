//
//  LTexture.hpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 20/03/2023.
//

#ifndef LTexture_hpp
#define LTexture_hpp


#include "header1.hpp"


class LTexture {
public:
    //Initializes variables
    LTexture(SDL_Renderer*& renderer, std::string path = "");
    LTexture();
    //Deallocates memory
    ~LTexture();

    //Load image at specified path
    bool loadFromFile(SDL_Renderer*& aRenderer, std::string path );

    //Create image from font string
    bool loadFromRenderedText (SDL_Renderer*& renderer, TTF_Font*& gFont, std::string textureText, SDL_Color textColor);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render(SDL_Renderer*& aRenderer, int x, int y, SDL_Rect* clip = NULL, double ratio = 1, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    //modulation
    void setColor( Uint8 r, Uint8 g, Uint8 b);

    //Setblending mode
    void setBlendMod ( SDL_BlendMode blending);

    //Blend the texture
    void blendTexture ( Uint8 alpha );

    //Get image dimensions
    int getWidth();
    int getHeight();
    
    //The actual hardware texture
    SDL_Texture* mTexture = NULL;
private:
    
    //Image dimensions
    int mWidth;
    int mHeight;
};


#endif /* LTexture_hpp */
