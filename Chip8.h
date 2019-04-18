#ifndef CHIP8_H_
#define CHIP8_H_

void initialize();
void loadGame(char game[]);
void emulateCycle();
void setKeys();

int drawFlag;

#endif