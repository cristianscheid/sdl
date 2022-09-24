/**
 * hello3_image.c - Initializes SDL, loads an image, And displys it in a window
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int main(void)
{
    // attempt to initialize graphics and timer system
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window *win = SDL_CreateWindow("Hello, CS50!",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 480, 0);

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("Hack-Regular.ttf", 72);
    SDL_Color color = {0, 200, 0};

    // load the image into memory
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, "Hello World!", color);

    // load the image data into the graphics hardware's memory
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, text_surface);
    SDL_FreeSurface(text_surface);

    // create a rectangle
    SDL_Rect Message_rect;
    // get texture witdth and height
    SDL_QueryTexture(tex, NULL, NULL, &Message_rect.w, &Message_rect.h);
    // set rectangle x and y position
    Message_rect.x = 50;
    Message_rect.y = 50;

    // clear the window
    SDL_RenderClear(rend);
    
    // draw the text to the window
    SDL_RenderCopy(rend, tex, NULL, &Message_rect);
    SDL_RenderPresent(rend);

    // wait a few seconds
    SDL_Delay(2000);

    // clean up resources before exiting
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
