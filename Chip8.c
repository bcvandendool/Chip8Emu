#include <stdio.h>

// 35 opcodes of 2 bytes
unsigned short opcode;
// 4k memory
unsigned char memory[4096];
// 15 general purpose 8-bit registers V0-V1E
// 16th register for carry flag
unsigned char V[16];

// index register and program counter
unsigned short I;
unsigned short pc;

unsigned char delay_timer;
unsigned char sound_timer;

// stack and stack pointer
unsigned short stack[16];
unsigned short sp;

// key state for keypad
unsigned char key[16];

unsigned char gfx[64 * 32];

unsigned char chip8_fontset[80] = 
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initialize()
{

    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    for(int i = 0; i < 2048; i++)
    {
        gfx[i] = 0;
    }

    for(int i = 0; i < 16; i++)
    {
        stack[i] = 0;
        V[i] = 0;
    }

    for(int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }

    for(int i = 0; i < 80; i++)
    {
        memory[i] = chip8_fontset[i];
    }

    delay_timer = 0;
    sound_timer = 0;

}

void loadGame(char game[])
{

    // use fopen in binary mode

}

void emulateCycle()
{

    opcode = memory[pc] << 8 | memory[pc + 1];

    switch(opcode & 0xF000)
    {

        case 0xA000:

        break;

        default:
            printf("Unknown opcode: 0x%X\n", opcode);

    }

    if(delay_timer > 0)
    {
        delay_timer--;
    }

    if(sound_timer > 0)
    {
        if(sound_timer == 1)
        {
            printf("BEEP!\n");
        }
        sound_timer--;
    }

}

void setKeys()
{

    
    
}