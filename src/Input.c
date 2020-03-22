#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "include/Chip8.h"

const Uint8* currentKeyStates;

void initInput()
{

    memset(key, 0, sizeof key);
    currentKeyStates = NULL;

}

void setInput()
{

    SDL_PumpEvents();
    currentKeyStates = SDL_GetKeyboardState(NULL);

    /**
     * Keypad                   Keyboard
     * +-+-+-+-+                +-+-+-+-+
     * |1|2|3|C|                |1|2|3|4|
     * +-+-+-+-+                +-+-+-+-+
     * |4|5|6|D|                |Q|W|E|R|
     * +-+-+-+-+       =>       +-+-+-+-+
     * |7|8|9|E|                |A|S|D|F|
     * +-+-+-+-+                +-+-+-+-+
     * |A|0|B|F|                |Z|X|C|V|
     * +-+-+-+-+                +-+-+-+-+
     * 
    */

    key[0x0] = currentKeyStates[SDL_SCANCODE_X];
    key[0x1] = currentKeyStates[SDL_SCANCODE_1];
    key[0x2] = currentKeyStates[SDL_SCANCODE_2];
    key[0x3] = currentKeyStates[SDL_SCANCODE_3];
    key[0x4] = currentKeyStates[SDL_SCANCODE_Q];
    key[0x5] = currentKeyStates[SDL_SCANCODE_W];
    key[0x6] = currentKeyStates[SDL_SCANCODE_E];
    key[0x7] = currentKeyStates[SDL_SCANCODE_A];
    key[0x8] = currentKeyStates[SDL_SCANCODE_S];
    key[0x9] = currentKeyStates[SDL_SCANCODE_D];
    key[0xA] = currentKeyStates[SDL_SCANCODE_Z];
    key[0xB] = currentKeyStates[SDL_SCANCODE_C];
    key[0xC] = currentKeyStates[SDL_SCANCODE_4];
    key[0xD] = currentKeyStates[SDL_SCANCODE_R];
    key[0xE] = currentKeyStates[SDL_SCANCODE_F];
    key[0xF] = currentKeyStates[SDL_SCANCODE_V];
    
}

unsigned char waitTillKeyPress()
{

    SDL_Event e;
    struct timespec ts = {0, 5000000L};

    while(1)
    {

        while(SDL_PollEvent(&e))
        {

            if(e.type == SDL_KEYDOWN)
            {

                switch (e.key.keysym.sym)
                {
                case SDLK_x:
                    return 0x0;
                    break;

                case SDLK_1:
                    return 0x1;
                    break;

                case SDLK_2:
                    return 0x2;
                    break;

                case SDLK_3:
                    return 0x3;
                    break;

                case SDLK_q:
                    return 0x4;
                    break;

                case SDLK_w:
                    return 0x5;
                    break;

                case SDLK_e:
                    return 0x6;
                    break;

                case SDLK_a:
                    return 0x7;
                    break;

                case SDLK_s:
                    return 0x8;
                    break;

                case SDLK_d:
                    return 0x9;
                    break;

                case SDLK_z:
                    return 0xA;
                    break;

                case SDLK_c:
                    return 0xB;
                    break;

                case SDLK_4:
                    return 0xC;
                    break;

                case SDLK_r:
                    return 0xD;
                    break;
                
                case SDLK_f:
                    return 0xE;
                    break;

                case SDLK_v:
                    return 0xF;
                    break;

                default:
                    break;
                }

            }

        }

        nanosleep(&ts, NULL);

    }

}