int systemIsRunning;
int systemIsPaused;
int systemStep;
int drawFlag;

unsigned char sound_timer;
unsigned char delay_timer;
unsigned char key[16];
unsigned char gfx[64 * 32];

void initChip8();
void loadROM(char* file);
void executeOP();