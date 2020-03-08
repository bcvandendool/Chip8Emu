#include <stdio.h>

#include "include/Input.h"
#include "include/Chip8.h"
#include "include/Graphics.h"

void playBeep();

int main(int argc, char **argv)
{

    initChip8();
    initInput();
    initGraphics();

    if(argc != 2)
    {

        printf("Please specify a rom file to load\n");
        return -1;

    }

    loadROM(*(argv + 1));

    while(systemIsRunning)
    {

        executeOP();

        if(drawFlag)
        {
            drawGraphics();
        }

        if(playSound)
        {
            playBeep();
        }

        setInput();

    }

    return 0;

}

void playBeep()
{


    
}