#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Screen dimensions
static const int screenWidth = 800;
static const int screenHeight = 600;
// Rectangle dimensions (used for snake and apple)
static int recWidth = 50;
static int recHeight = 50;
// Set directions
static int left = 1;
static int right = 2;
static int up = 3;
static int down = 4;
static int direction;
static int frameDuration;
// Apple
static int randomIndexX;
static int randomIndexY;
static int applePosX;
static int applePosY;
// Snake
static int snakeSize;
static int step = 50;
// Create structure that composes the snake and apple
struct Rectangle
{
    int posX;
    int posY;
    int posXOld;
    int posYOld;
};
// Initialize an array of rectangles that compose de snake
static struct Rectangle recsSnake[50];

void initializeSnake()
{
    direction = right;
    snakeSize = 5;
    int initialX = 350;
    int initialY = 250;
    recsSnake[0].posX = initialX, recsSnake[0].posY = initialY;
    recsSnake[1].posX = initialX - recWidth, recsSnake[1].posY = initialY;
    recsSnake[2].posX = initialX - (recWidth * 2), recsSnake[2].posY = initialY;
    recsSnake[3].posX = initialX - (recWidth * 3), recsSnake[3].posY = initialY;
    recsSnake[4].posX = initialX - (recWidth * 4), recsSnake[4].posY = initialY;
}

void initializeApple()
{
    bool availableCoordinate = false;
    while (!availableCoordinate)
    {
        randomIndexX = rand() % (screenWidth / recWidth);
        randomIndexY = rand() % (screenHeight / recHeight);
        applePosX = randomIndexX * recWidth;
        applePosY = randomIndexY * recHeight;

        availableCoordinate = true;
        for (size_t i = 0; i < snakeSize; i++)
        {
            if (applePosX == recsSnake[i].posX && applePosY == recsSnake[i].posY)
            {
                availableCoordinate = false;
                break;
            }
        }
    }
}

void renderMenu(SDL_Renderer *renderer, SDL_Event event)
{
    bool runningMenu = true;
    while (runningMenu)
    {
        // Background
        SDL_SetRenderDrawColor(renderer, 30, 100, 20, 255);
        SDL_RenderClear(renderer);
        // Show what was drawn
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                runningMenu = false;
                break;
            }
        }
    }
}

int main(int argc, char **argv)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;

    renderMenu(renderer, event);

    initializeSnake();
    srand(time(NULL));
    initializeApple();

    bool runningGame = true;
    while (runningGame)
    {
        // Background
        SDL_SetRenderDrawColor(renderer, 30, 100, 20, 255);
        SDL_RenderClear(renderer);
        // Apple
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_Rect rect = {applePosX, applePosY, recWidth, recHeight};
        SDL_RenderFillRect(renderer, &rect);
        // Snake
        SDL_SetRenderDrawColor(renderer, 200, 200, 20, 255);
        for (size_t i = 0; i < snakeSize; i++)
        {
            SDL_Rect rect = {recsSnake[i].posX, recsSnake[i].posY, recWidth, recHeight};
            // SDL_RenderFillRect(renderer, &rect);
            SDL_RenderDrawRect(renderer, &rect);
        }

        // Show what was drawn
        SDL_RenderPresent(renderer);

        // Process events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                runningGame = false;
            }
            switch (event.type)
            {
            // Look for a keypress
            case SDL_KEYDOWN:
                // Check the SDLKey values and move change the coords
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if (direction != right)
                    {
                        direction = left;
                    }
                    break;
                case SDLK_RIGHT:
                    if (direction != left)
                    {
                        direction = right;
                    }
                    break;
                case SDLK_UP:
                    if (direction != down)
                    {
                        direction = up;
                    }
                    break;
                case SDLK_DOWN:
                    if (direction != up)
                    {
                        direction = down;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        // Check for collision between Snake and Apple
        if (recsSnake[0].posX == applePosX && recsSnake[0].posY == applePosY)
        {
            recsSnake[snakeSize].posX = recsSnake[snakeSize - 1].posX;
            recsSnake[snakeSize].posY = recsSnake[snakeSize - 1].posY;
            snakeSize++;
            initializeApple();
        }
        // Check for collision between Snake head and body
        for (size_t i = 1; i < snakeSize; i++)
        {
            if (recsSnake[0].posX == recsSnake[i].posX && recsSnake[0].posY == recsSnake[i].posY)
            {
                return 0;
            }
        }
        // Check for collision between Snake and Wall (screen limit)
        if (recsSnake[0].posX < 0 || recsSnake[0].posX > screenWidth - recWidth || recsSnake[0].posY < 0 || recsSnake[0].posY > screenHeight - recHeight)
        {
            return 0;
        }
        // This frameDuration defines the speed with which the snake moves
        if (frameDuration < 30)
        {
            frameDuration++;
        }
        else
        {
            recsSnake[0].posXOld = recsSnake[0].posX;
            recsSnake[0].posYOld = recsSnake[0].posY;

            for (size_t i = 1; i < snakeSize; i++)
            {
                // Store current x,y coordinates so the next item of array replace this position
                recsSnake[i].posXOld = recsSnake[i].posX;
                recsSnake[i].posYOld = recsSnake[i].posY;
                // Get the x,y coordinates from the previous item of array
                recsSnake[i].posX = recsSnake[i - 1].posXOld;
                recsSnake[i].posY = recsSnake[i - 1].posYOld;
            }

            if (direction == left)
            {
                recsSnake[0].posX = recsSnake[0].posX - step;
            }
            else if (direction == right)
            {
                recsSnake[0].posX = recsSnake[0].posX + step;
            }
            else if (direction == up)
            {
                recsSnake[0].posY = recsSnake[0].posY - step;
            }
            else if (direction == down)
            {
                recsSnake[0].posY = recsSnake[0].posY + step;
            }

            frameDuration = 0;
        }
    }

    // Release resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}