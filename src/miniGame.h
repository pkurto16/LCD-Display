#include <Arduino.h>
#include "rgb_lcd.h"
#include <stdlib.h>
extern rgb_lcd lcd;
const uint8_t asciiNumConst = 48;
int drawEntitiesToPos(int entityData, int entityNum);
void destroyEntity(int entityNum);
void findNewPosOfEntities();
void move(int direction);
void shoot();
void recieveInput();
char chooseCharacter();
void characterStartAnimation();
void setup();
extern void hideAnimation();
void playTone(int tone, int duration);
void playGalaga();
void playPong(long startTime);
void playMemory(long startTime);



