#include <stdio.h>
#include "Chip8.h"

void setupGraphics();
void setupInput();
void drawGraphics();

void main()
{

    setupGraphics();
    setupInput();

    initialize();
    loadGame("pong");

    for(;;)
    {

        emulateCycle();

        if(drawFlag)
        {

            drawGraphics();

        }

        setKeys();

    }

}

void setupGraphics()
{



}

void setupInput()
{



}

void drawGraphics()
{



}