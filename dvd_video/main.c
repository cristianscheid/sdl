#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *initialize_texture_from_file(const char *file_name, SDL_Renderer *renderer)
{
    SDL_Surface *image = IMG_Load(file_name);
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return image_texture;
}

static const int width = 800;
static const int height = 600;

int main(int argc, char **argv)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Hello, SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Initialize support for loading PNG and JPEG images
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    SDL_Texture *image_texture = initialize_texture_from_file("dvd.jpg", renderer);
    int image_width, image_height;

    // Get texture width/height
    SDL_QueryTexture(image_texture, NULL, NULL, &image_width, &image_height);
    // Define where on the "screen" we want to draw the texture
    SDL_Rect texture_destination;

    texture_destination.x = 50;
    texture_destination.y = 50;
    texture_destination.w = 200;
    texture_destination.h = 200;
    int xDir = 1;
    int yDir = 1;

    bool running = true;
    SDL_Event event;
    while (running)
    {
        // Process events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw
        SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);

        if (xDir == 1 && texture_destination.x < width - texture_destination.w)
        {
            texture_destination.x++;
        }
        else if (xDir == 0 && texture_destination.x > 0)
        {
            texture_destination.x--;
        }
        else if (texture_destination.x == width - texture_destination.w)
        {
            xDir = 0;
            texture_destination.x--;
        }
        else if (texture_destination.x == 0)
        {
            xDir = 1;
            texture_destination.x++;
        }

        if (yDir == 1 && texture_destination.y < height - texture_destination.h)
        {
            texture_destination.y++;
        }
        else if (yDir == 0 && texture_destination.y > 0)
        {
            texture_destination.y--;
        }
        else if (texture_destination.y == height - texture_destination.h)
        {
            yDir = 0;
            texture_destination.y--;
        }
        else if (texture_destination.y == 0)
        {
            yDir = 1;
            texture_destination.y++;
        }

        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    // Release resources
    SDL_DestroyTexture(image_texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}