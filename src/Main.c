#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <sys/time.h>

#include "include/Input.h"
#include "include/Chip8.h"
#include "include/Graphics.h"

void playBeep();

int main(int argc, char **argv)
{

    initChip8();
    initGraphics();
    initInput();

    int frequency = 600;

    if(argc < 2)
    {

        printf("Please specify a rom file to load\n");
        return -1;

    }

    if(argc > 2)
    {

        for(int i = 2; i < argc; i++)
        {

            if(strcmp(argv[i], "-f") == 0)
            {

                if(i + 1 >= argc)
                {
                    printf("Please specify frequency for option -f\n");
                    return -1;
                } else 
                {

                    i++;
                    frequency = atoi(argv[i]);
                    if(frequency < 60)
                    {

                        printf("Frequency must be higher than 60\n");
                        return -1;

                    }

                }

            }

        }

    }

    loadROM(*(argv + 1));

    struct timespec ts;

    long delay = 1000000000 / frequency;
    int counter = 0;

    struct timespec start;
    struct timespec end;

    while(systemIsRunning)
    {

        clock_gettime(CLOCK_REALTIME, &start);

        if(counter % (int) round(frequency / 60) == 0)
        {

            counter = 0;
            sound_timer--;
            delay_timer--;

        }

        counter++;

        if(!systemIsPaused || systemStep)
        {

            systemStep = 0;
            executeOP();

        }

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

        clock_gettime(CLOCK_REALTIME, &end);

        if(end.tv_nsec - start.tv_nsec < delay)
        {

            if(end.tv_sec != start.tv_sec)
            {
                ts.tv_nsec = delay - ((999999999 - end.tv_nsec) - start.tv_nsec);
            } else 
            {
                ts.tv_nsec = delay - (end.tv_nsec - start.tv_nsec);
            }

            nanosleep(&ts, NULL);

        }

    }

    return 0;

}

void playBeep()
{

    
    
}