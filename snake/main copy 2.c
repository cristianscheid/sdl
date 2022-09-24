#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// window dimensions
static int const windowWidth = 800;
static int const windowHeight = 600;
// rectangle dimensions (used to compose snake and apple)
static int const recWidth = 50;
static int const recHeight = 50;
// how many pixels/frame snake will move
static int const step = 50;
// set directions
static int const left = 1;
static int const right = 2;
static int const up = 3;
static int const down = 4;
// boolean to quit game
static bool quitGame = false;
// apple
static int applePosX;
static int applePosY;
// snake
static int snakeSize;
static int direction;
static int directionOld;
// create rectangle structure
struct Rectangle
{
    int posX;
    int posY;
    int posXOld;
    int posYOld;
};
// initialize array of rectangles that compose the snake
static struct Rectangle snake[50];

void initializeSnake()
{
    direction = right;
    snakeSize = 5;
    int initialX = 350;
    int initialY = 250;
    snake[0].posX = initialX, snake[0].posY = initialY;
    snake[1].posX = initialX - recWidth, snake[1].posY = initialY;
    snake[2].posX = initialX - (recWidth * 2), snake[2].posY = initialY;
    snake[3].posX = initialX - (recWidth * 3), snake[3].posY = initialY;
    snake[4].posX = initialX - (recWidth * 4), snake[4].posY = initialY;
}

void initializeApple()
{
    bool availablePosition = false;
    while (!availablePosition)
    {
        // generate random x,y position for apple
        applePosX = (rand() % (windowWidth / recWidth)) * recWidth;
        applePosY = (rand() % (windowHeight / recHeight)) * recHeight;
        // check if position is available (not used by snake)
        availablePosition = true;
        for (size_t i = 0; i < snakeSize; i++)
        {
            if (applePosX == snake[i].posX && applePosY == snake[i].posY)
            {
                availablePosition = false;
                break;
            }
        }
    }
}

void renderMenu(SDL_Renderer *renderer, SDL_Event event)
{
    TTF_Font *font = TTF_OpenFont("fonts/Hack-Regular.ttf", 72);
    SDL_Color color = {200, 200, 20};
    // load the image into memory
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Snake Game", color);
    // load the image data into the graphics hardware's memory
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    // create a rectangle
    SDL_Rect textRect;
    // get texture witdth and height
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    // set rectangle x and y position
    textRect.x = (windowWidth / 2) - (textRect.w / 2);
    textRect.y = 100;

    bool runningMenu = true;
    while (runningMenu)
    {
        // background
        SDL_SetRenderDrawColor(renderer, 30, 100, 20, 255);
        SDL_RenderClear(renderer);
        // draw the text to the window
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        // show what was drawn
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                runningMenu = false;
                quitGame = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_RETURN:
                    runningMenu = false;
                    initializeApple();
                    initializeSnake();
                    break;
                default:
                    break;
                }
            }
        }
    }
}

int main(void)
{
    // initialize graphics and timer system
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    // create window
    SDL_Window *window = SDL_CreateWindow("Snake Game",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth, windowHeight, 0);
    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);

    SDL_Event event;

    // initialize ttf font
    TTF_Init();

    // render menu
    renderMenu(renderer, event);

    // iniatilize random number generator
    srand(time(NULL));

    initializeSnake();
    initializeApple();

    while (!quitGame)
    {
        directionOld = direction;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quitGame = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if (directionOld != right)
                    {
                        direction = left;
                    }
                    break;
                case SDLK_RIGHT:
                    if (directionOld != left)
                    {
                        direction = right;
                    }
                    break;
                case SDLK_UP:
                    if (directionOld != down)
                    {
                        direction = up;
                    }
                    break;
                case SDLK_DOWN:
                    if (directionOld != up)
                    {
                        direction = down;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        // move snake
        snake[0].posXOld = snake[0].posX;
        snake[0].posYOld = snake[0].posY;
        for (size_t i = 1; i < snakeSize; i++)
        {
            // store current x,y coordinates so the next item of array replace this position
            snake[i].posXOld = snake[i].posX;
            snake[i].posYOld = snake[i].posY;
            // get the x,y coordinates from the previous item of array
            snake[i].posX = snake[i - 1].posXOld;
            snake[i].posY = snake[i - 1].posYOld;
        }

        if (direction == left)
        {
            snake[0].posX = snake[0].posX - step;
        }
        else if (direction == right)
        {
            snake[0].posX = snake[0].posX + step;
        }
        else if (direction == up)
        {
            snake[0].posY = snake[0].posY - step;
        }
        else if (direction == down)
        {
            snake[0].posY = snake[0].posY + step;
        }

        // check for collision between snake and apple
        if (snake[0].posX == applePosX && snake[0].posY == applePosY)
        {
            snake[snakeSize].posX = snake[snakeSize - 1].posX;
            snake[snakeSize].posY = snake[snakeSize - 1].posY;
            snakeSize++;
            initializeApple();
        }
        // check for collision between snake head and body
        for (size_t i = 1; i < snakeSize; i++)
        {
            if (snake[0].posX == snake[i].posX && snake[0].posY == snake[i].posY)
            {
                renderMenu(renderer, event);
            }
        }
        // check for collision between snake and window limits
        if (snake[0].posX < 0 || snake[0].posX > windowWidth - recWidth || snake[0].posY < 0 || snake[0].posY > windowHeight - recHeight)
        {
            renderMenu(renderer, event);
        }

        // draw background
        SDL_SetRenderDrawColor(renderer, 30, 100, 20, 255);
        SDL_RenderClear(renderer);
        // draw apple
        SDL_Rect rect = {applePosX, applePosY, recWidth, recHeight};
        SDL_SetRenderDrawColor(renderer, 220, 20, 20, 255);
        SDL_RenderFillRect(renderer, &rect);
        // draw snake
        for (size_t i = 0; i < snakeSize; i++)
        {
            SDL_Rect rect = {snake[i].posX, snake[i].posY, recWidth, recHeight};
            SDL_Rect rect2 = {snake[i].posX + 4, snake[i].posY + 4, recWidth - 8, recHeight - 8};
            SDL_SetRenderDrawColor(renderer, 200, 200, 20, 255);
            SDL_RenderDrawRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 150, 150, 20, 255);
            SDL_RenderFillRect(renderer, &rect2);
        }
        // show what was drawn
        SDL_RenderPresent(renderer);
        // wait 20/60th of a second
        SDL_Delay(20000 / 60);
    }

    // clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}