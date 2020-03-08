#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int systemIsRunning;
int drawFlag;

unsigned char delay_timer;
unsigned char sound_timer;

unsigned short opcode;
// 0x000 - 0x1FF - Chip 8 interpreter
// 0x200 - 0xFFF - program ROM and RAM
unsigned char memory[4096];
unsigned char V[16];
unsigned short I;
unsigned short pc;

unsigned short stack[16];
unsigned short sp;

unsigned char gfx[64 * 32];

unsigned char key[16];

int count = 0;

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

void initChip8()
{

    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    memset(gfx, 0, sizeof gfx);
    memset(stack, 0, sizeof stack);
    memset(V, 0, sizeof V);
    memset(memory, 0, sizeof memory);

    delay_timer = 0;
    sound_timer = 0;

    drawFlag = 1;
    systemIsRunning = 1;

    for(int i = 0; i < 80; i++)
    {

        memory[i] = chip8_fontset[i];

    }

    printf("Initialized emulator\n");

}

void loadROM(char* file)
{

    printf("Loading file %s\n", file);

    FILE * rom = fopen(file, "rb");

    if(!rom)
    {

        printf("Could not open file %s\n", file);
        return;

    }

    fseek(rom, 0, SEEK_END);
    int rom_size = ftell(rom);
    rewind(rom);

    if(rom_size > 4096 - 512)
    {

        printf("Rom %s is too large, %d > 4096 - 512\n", file, rom_size);
        return;

    }

    char * rom_data = (char *) malloc (rom_size * sizeof(char));

    if(rom_data == NULL)
    {

        printf("Could not get memory for rom data\n");

    }

    fread(rom_data, sizeof(char), rom_size, rom);

    for(int i = 0; i < rom_size; i++)
    {

        memory[i + 512] = rom_data[i];

    }

    free(rom_data);
    rom_data = NULL;

    printf("Loaded file %s\n", file);

}

void executeOP()
{

    if(pc > 4096 || pc+1 > 4096)
    {
        printf("Trying to access unreachable memory!\n");
        systemIsRunning = 0;
        return;
    }

    opcode = memory[pc] << 8 | memory[pc + 1];

    printf("0x%X: 0x%X\n", pc, opcode);

    pc += 2;

    count++;

    if(count > 10)
    {
        count = 0;
        delay_timer--;
    }

    switch(opcode & 0xF000)
    {

        case 0x0000:
            switch(opcode)
            {

                case 0x00E0:
                //Opcode: 00E0 - CLS
                //Clear the display.
                    memset(gfx, 0, sizeof gfx);
                    drawFlag = 1;
                break;

                case 0x00EE:
                //Opcode: 00EE - RET
                //Return from a subroutine.
                //The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
                    pc = stack[sp];
                    sp--;
                break;

            }
        break;

        case 0x1000:
        //Opcode: 1nnn - JP addr
        //Jump to location nnn.
        //The interpreter sets the program counter to nnn.
            pc = opcode & 0x0FFF;
        break;

        case 0x2000:
        //Opcode: 2nnn - CALL addr
        //Call subroutine at nnn.
        //The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
            sp++;
            stack[sp] = pc;
            pc = opcode & 0x0FFF;
        break;
        
        case 0x3000:
        //Opcode: 3xkk - SE Vx, byte
        //Skip next instruction if Vx == kk.
        //The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            {
                pc += 2;
            }
        break;

        case 0x4000:
        //Opcode: 4xkk - SNE Vx, byte
        //Skip next instruction if Vx != kk.
        //The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            {
                pc += 2;
            }
        break;

        case 0x5000:
        //Opcode: 5xy0 - SE Vx, Vy
        //Skip next instruction if Vx = Vy.
        //The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            {
                pc += 2;
            }
        break;

        case 0x6000:
        //Opcode: 6xkk - LD Vx, byte
        //Set Vx = kk.
        //The interpreter puts the value kk into register Vx.
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        break;

        case 0x7000:
        //Opcode: 7xkk - ADD Vx, byte
        //Set Vx = Vx + kk.
        //Adds the value k to the value of register Vx, then stores the result in Vx.
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        break;

        case 0x8000:
            switch(opcode & 0x000F)
            {

                case 0x000:
                //Opcode: 8xy0 - LD Vx, Vy
                //Set Vx = Vy.
                //Stores the value of register Vy in register Vx.
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                break;

                case 0x001:
                //Opcode: 8xy1 - OR Vx, Vy
                //Set Vx = Vx OR Vy.
                //Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the correspondin bits from two values,
                //and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                break;

                case 0x002:
                //Opcode: 8xy2 - AND Vx, Vy
                //Set Vx = Vx AND Vy.
                //Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corresponding bits from two values,
                //and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                break;

                case 0x003:
                //Opcode: 8xy3 - XOR Vx, Vy
                //Set Vx = Vx XOR Vy
                //Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corresponding bits from
                //two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                break;

                case 0x004:
                //Opcode: 8xy4 - ADD Vx, Vy
                //Set Vx = Vx + Vy, set VF = carry.
                //The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255) VF is set to 1, otherwise 0. Only the lowest 8
                //bits of the result are kept, and stored in Vx.
                {
                    int result = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
                    V[0xF] = result > 255;
                    V[(opcode & 0x0F00) >> 8] = result & 0xFF;
                }
                break;

                case 0x005:
                //Opcode: 8xy5 - SUB Vx, Vy
                //Set Vx = Vx - Vy, set VF = NOT borrow.
                //If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx. 
                {
                    unsigned char result = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                    V[0xF] = V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = result;
                }
                break;

                case 0x006:
                //Opcode: 8xy6 - SHR Vx {, Vy}
                //Set Vx = Vx SHR 1.
                //If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. The Vx is divided by 2.
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 1;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] / 2;
                break;

                case 0x007:
                //Opcode: 8xy7 - SUBN Vx, Vy
                //Set Vx = Vy - Vx, set VF = NOT borrow.
                //If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
                {
                    unsigned char result = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    V[0xF] = V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8];
                    V[(opcode & 0x0F00) >> 8] = result;
                }
                break;

                case 0x00E:
                //Opcode: 8xyE - SHL Vx {, Vy}
                //Set Vx = Vx SHL 1.
                //If the most significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x80;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00)] * 2;
                break;

            }
        break;

        case 0x9000:
        //Opcode: 9xy0 - SNE Vx, Vy
        //Skip the next instruction if Vx != Vy.
        //The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            {
                pc += 2;
            }
        break;

        case 0xA000:
        //Opcode: Annn - LD I, addr
        //Set I = nnn.
        //The value of register I is set to nnn.
            I = opcode & 0x0FFF;
        break;

        case 0xB000:
        //Opcode: Bnnn - JP V0, addr
        //Jump to location nnn + V0.
        //The program counter is set to nnn plus the value of V0.
            pc = (opcode & 0x0FFF) + V[0];
        break;

        case 0xC000:
        //Opcode: Cxkk - RND Vx, byte
        //Set Vx = random byte AND kk
        //The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for
        //more information on AND.
        {
            char random = rand() % 256;
            V[(opcode & 0x0F00) >> 8] = random & (opcode & 0x00FF);
        }
        break;

        case 0xD000:
        //Opcode: Dxyn - DRW Vx, Vy, nibble
        //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        //The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates
        //(Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite
        //is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for
        //more information on XOR.
        {
            int x = V[(opcode & 0x0F00) >> 8];
            int y = V[(opcode & 0x00F0) >> 4];
            int n = opcode & 0x000F;

            V[0xF] = 0;

            for(int yline = 0; yline < n; yline++)
            {

                for(int xline = 0; xline < 8; xline++)
                {

                    if((memory[I + yline] & (0x80 >> xline)) != 0)
                    {
                        if(gfx[(x + xline + ((y + yline) * 64))] == 1)
                        {
                            V[0xF] = 1;
                        }
                        gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }

                }

            }

            drawFlag = 1;
        }
        break;

        case 0xE000:
            switch(opcode & 0x00FF)
            {

                case 0x009E:
                //Opcode: Ex9E - SKP Vx
                //Skip next instruction if key with the value of Vx is pressed.
                //Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
                    if(key[(opcode & 0x0F00) >> 8] == 1)
                    {
                        pc += 2;
                    }
                break;

                case 0x00A1:
                //Opcode: ExA1 - SKNP Vx
                //Skip the next instruction if key with the value of Vx is not pressed.
                //Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
                    if(key[(opcode & 0x0F00) >> 8] == 0)
                    {
                        pc += 2;
                    }
                break;

            }
        break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {

                case 0x0007:
                //Opcode: Fx07 - LD Vx, DT
                //Set Vx = delay time value.
                //The value of DT is placed into Vx.
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                break;

                case 0x000A:
                //Opcode: Fx0A - LD Vx, k
                //Wait for a key press, store the value of the key in Vx.
                //All execution stops until a key is pressed, then the value of that key is stored in Vx.
                    //TODO: Implement Fx0A
                break;

                case 0x0015:
                //Opcode: Fx15 - LD DT, Vx
                //Set delay timer = Vx.
                //DT is set equal to the value of Vx.
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                break;

                case 0x0018:
                //Opcode: Fx18 - LD ST, Vx
                //Set sound timer = Vx.
                //ST is set equal to the value of Vx.
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                break;

                case 0x001E:
                //Opcode: FX1E - ADD I, Vx
                //Set I = I + Vx.
                //The values of I and Vx are added, and the results are stored in I.
                    I = I + V[(opcode & 0x0F00) >> 8];
                break;

                case 0x0029:
                //Opcode: Fx29 - LD F, Vx
                //Set I = location of sprite for digit Vx.
                //The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
                    I = 5 * V[(opcode & 0x0F00) >> 8];
                break;

                case 0x0033:
                //Opcode: Fx33 - LD B, Vx
                //Store BCD representation of Vx in memory locations I, I+1 and I+2.
                //The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and
                //the ones digit at location I+2.
                    memory[I] = (V[(opcode & 0x0F00) >> 8] / 100) % 10;
                    memory[I+1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I+2] = V[(opcode & 0x0F00) >> 8] % 10;
                break;

                case 0x0055:
                //Opcode: Fx55 - LD [I], Vx
                //Store registers V0 to Vx in memory starting at location I.
                //The interpreter copies the value of registers V0 through Vx into memory, starting at the address in I.
                    for(int i = 0; i < ((opcode & 0x0F00) >> 8) + 1; i++)
                    {
                        memory[I + i] = V[i];
                    }
                break;

                case 0x0065:
                //Opcode: Fx65 - LD Vx, [I]
                //Read register V0 though Vx from memory starting at location I.
                //The interpreter reads values from memory starting at location I into registers V0 through Vx.
                    for(int i = 0; i < ((opcode & 0x0F00) >> 8) + 1; i++)
                    {
                        V[i] = memory[I + i];
                    }
                break;

            }
        break;

        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            systemIsRunning = 0;
        break;

    }

}