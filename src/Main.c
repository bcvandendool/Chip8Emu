#include <stdio.h>
#include <unistd.h>

#include "include/Input.h"
#include "include/Chip8.h"
#include "include/Graphics.h"

void playBeep();

int main(int argc, char **argv)
{

    initChip8();
    initGraphics();
    initInput();

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
            drawFlag = 0;
        }

        if(sound_timer > 0)
        {
            playBeep();
        }

        handleSDLEvents();

        setInput();

        sleep(1/600);


    }

    return 0;

}

void playBeep()
{


    
}