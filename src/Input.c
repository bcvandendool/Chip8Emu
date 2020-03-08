#include <stdio.h>
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