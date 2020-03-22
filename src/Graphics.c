#include <stdio.h>
#include <SDL2/SDL.h>

#include "include/Chip8.h"
#include "include/Graphics.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

SDL_Window *win = NULL;
SDL_Renderer * ren = NULL;
SDL_Event e;

void initGraphics()
{

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {

        printf("SDL_Init error: %s\n", SDL_GetError());
        return;

    }

    win = SDL_CreateWindow("Chip-8 Emulator", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(win == NULL)
    {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(ren == NULL)
    {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    drawGraphics();

}

void handleSDLEvents()
{

    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
            systemIsRunning = 0;
        } else if(e.type == SDL_KEYDOWN)
        {

            if(e.key.keysym.sym == SDLK_RIGHT)
            {
                systemStep = 1;
            } else if(e.key.keysym.sym == SDLK_DOWN)
            {
                systemIsPaused = 1;
            } else if(e.key.keysym.sym == SDLK_UP)
            {
                systemIsPaused = 0;
            }

        }
    }

}

void drawGraphics()
{

    int pixel_height = SCREEN_HEIGHT / 32;
    int pixel_width = SCREEN_WIDTH / 64;

    SDL_Rect pixel = {0, 0, 0, 0};

    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(ren);

    for(int y = 0; y < 32; y++)
    {
        for(int x = 0; x < 64; x++)
        {
            if(gfx[y*64+x] == 1)
            {
                SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);
            } else
            {
                SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
            }
            
            pixel.x = x * pixel_width;
            pixel.y = y * pixel_height;
            pixel.w = pixel_width;
            pixel.h = pixel_height;
            SDL_RenderFillRect(ren, &pixel);
        }
    }

    SDL_RenderPresent(ren);

}