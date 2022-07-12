#include <Arduino.h>
#include "rgb_lcd.h"
#include <stdlib.h>
#include <miniGame.h>
extern rgb_lcd lcd;
bool chosen=false;
uint8_t memScore = 99;
uint8_t memHighscore;
int pairsLeft = 12;
int memXPos = 2;
int memYPos = 0;
int selectedXPos;
char firstRow[12];
char secondRow[12];
int positionsLeft[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
long memTimeAtFirstInput;
byte selected[8] = {
    B00000,
    B11111,
    B11011,
    B10101,
    B10101,
    B11011,
    B11111,
    B00000};
byte error[8] = {
  B00000,
  B00000,
  B01010,
  B00100,
  B00100,
  B01010,
  B00000,
  B00000
};
void resetPosLeft(){
    for(int i = 0; i<12; i++){
        positionsLeft[i]=i;
    }
}
bool endMemoryGame(){
    bool restart = false;
    lcd.clear();
    String burner = Serial.readString();
    if(memScore>memHighscore) memHighscore=memScore;
    while(!restart){
        lcd.setCursor(6,0);
        lcd.write("GAME");
        lcd.setCursor(6,1);
        lcd.write("OVER");
        delay(1000);
        hideAnimation();
        lcd.setCursor(4,0);
        lcd.write("SCORE:");
        lcd.write((memScore/10)+asciiNumConst);
        lcd.write((memScore%10)+asciiNumConst);
        lcd.setCursor(2,1);
        lcd.write("HIGHSCORE:");
        lcd.write((memHighscore/10)+asciiNumConst);
        lcd.write((memHighscore%10)+asciiNumConst);
        delay(1000);
        hideAnimation();
        lcd.setCursor(5,0);
        lcd.write("TYPE Q");
        lcd.setCursor(3,1);
        lcd.write("TO GO BACK");
        unsigned long timer = millis();
        while(millis()<timer+3000&&!restart){
            String inputs = Serial.readString();
            for( unsigned int i=0; i<inputs.length(); i++) if(inputs.charAt(i)=='q' || inputs.charAt(i)=='Q' ) restart = true;
            delay(50);
        }
        hideAnimation();
    }
    lcd.clear();
    burner = Serial.readString();
    lcd.setCursor(0,0);
    lcd.write("TYPE Q: RESTART");
    lcd.setCursor(0,1);
    lcd.write("TYPE E: TO MENU");
    unsigned long timer = millis();
    bool exit = false;
    restart=false;
    while(millis()<timer+3000&&!restart&&!exit){
        String inputs = Serial.readString();
        for(unsigned int i=0; i<inputs.length(); i++){
            if(inputs.charAt(i)=='e' || inputs.charAt(i)=='E' ){
            lcd.clear(); 
            exit=true;
            }
            if(inputs.charAt(i)=='q' || inputs.charAt(i)=='Q' ){
            lcd.clear(); 
            restart=true;
            }
        }
        delay(50);
    }
    hideAnimation();
    resetPosLeft();
    memScore=50;
    pairsLeft=12;
    if(exit) return exit;
    burner = Serial.readString();
    
    while(!Serial.available()){
    delay(100);
    }
    return false;
}
bool selectionIsBlank()
{
    if (memYPos == 0 && firstRow[memXPos-2] == ' ')
        return true;
    if (memYPos == 1 && secondRow[memXPos-2] == ' ')
        return true;
    return false;
}
void moveMem(int direction)
{
    lcd.setCursor(memXPos, memYPos);
    if(selectionIsBlank()){
        lcd.write(' ');
    }
    else{
        lcd.write((unsigned char)7);
    }
    if (!((memXPos < 3 && direction == 0) || (memXPos > 12 && direction == 1)))
    {
        memXPos += (uint8_t)(100 + direction * 2 - 1);
        memXPos -= 100;
    }

    lcd.setCursor(memXPos, memYPos);
    if(selectionIsBlank()){
        lcd.write((unsigned char) 0);
    }
    else{
        lcd.write(219);
    }
}
void selectTop()
{
    lcd.setCursor(memXPos,memYPos);
    lcd.write(firstRow[memXPos-2]);
    selectedXPos = memXPos;
    memXPos = 2;
    memYPos = 1;
    lcd.setCursor(2,1);
    lcd.write(219);
}
bool correctSelection()
{
    if (firstRow[selectedXPos-2] == secondRow[memXPos-2])
    {
        return true;
    }
    return false;
}
void selectionIsGood()
{   
    delay(800);
    firstRow[selectedXPos-2] = ' ';
    secondRow[memXPos-2] = ' ';
    lcd.setCursor(memXPos,memYPos);
    lcd.write(' ');
    lcd.setCursor(selectedXPos, 0);
    lcd.write(' ');
    pairsLeft--;
}
void selectionIsBad()
{
    delay(800);
    lcd.setCursor(memXPos,memYPos);
    lcd.write((unsigned char)7);
    lcd.setCursor(selectedXPos, 0);
    lcd.write((unsigned char)7);
    memScore--;
}
void selectBottom()
{
    lcd.setCursor(memXPos,memYPos);
    lcd.write(secondRow[memXPos-2]);
    if (correctSelection())
    {
        selectionIsGood();
    }
    else
    {
        selectionIsBad();
    }
    memXPos = 2;
    memYPos = 0;
    lcd.setCursor(2,0);
    lcd.write(219);
    chosen = true;
}
void showX()
{
    for (int i = 0; i < 2; i++)
    {
        lcd.setCursor(memXPos,memYPos);
        lcd.write(' ');
        delay(150);
        lcd.setCursor(memXPos,memYPos);
        lcd.write((unsigned char) 0);
        delay(150);
    }
}
void select()
{
    if (selectionIsBlank())
    {
        showX();
        return;
    }
    if (memYPos == 0)
    {
        selectTop();
    }
    else
    {
        selectBottom();
    }
}
void recieveMemInput()
{
    Serial.setTimeout(1);
    String serial = Serial.readStringUntil('d' || 'a');
    switch (serial.charAt(serial.length() - 1))
    {
    case 'a':
        moveMem(0);
        break;
    case 'd':
        moveMem(1);
        break;
    case '.':
        select();
        break;
    default:
        break;
    }
}
void playMemGame()
{
    lcd.setCursor(2,0);
    lcd.write(219);
    while (!chosen)
    {
        recieveMemInput();
    }
}
bool posInList(int pos)
{
    for (int i : positionsLeft)
    {
        if (i == pos)
        {
            return true;
        }
    }
    return false;
}
void setupGrid()
{
    lcd.setCursor(2, 0);
    for (int i = 0; i < 12; i++)
    {
        firstRow[i] = random(33, 126);
        lcd.write(firstRow[i]);
    }
    lcd.setCursor(2, 1);
    for (int i = 0; i < 12; i++)
    {
        int pos = random(0, 13);
        while (!posInList(pos))
        {
            pos = random(0, 13);
        }
        positionsLeft[pos] = 99;
        secondRow[i] = firstRow[pos];
        lcd.write(secondRow[i]);
    }
}
void hideOnceReady()
{
    delay(500);
    Serial.read();
    while (!Serial.available())
        delay(50);
    lcd.setCursor(2, 0);
    for (int i = 0; i < 12; i++)
    {
        lcd.write((unsigned char)7);
    }
    lcd.setCursor(2, 1);
    for (int i = 0; i < 12; i++)
    {
        lcd.write((unsigned char)7);
    }
}
bool playOnce()
{
    long gameTime=millis();
    setupGrid();
    hideOnceReady();
    while (pairsLeft > 0 && memScore>0&&millis()-gameTime<215000)
    {
        playMemGame();
        chosen = false;
    }
    if(millis()-gameTime>15000) memScore*=1.0-(double)((millis()-gameTime-15000)/200000);
    return endMemoryGame();
}
void waitForMemStartInput()
{
    lcd.setCursor(2, 0);
    lcd.write("TYPE ANY KEY");
    lcd.setCursor(4, 1);
    lcd.write("TO START");
    delay(500);
    Serial.read();
    while (!Serial.available())
    {
        delay(50);
    }
}
void loopMemory()
{
    bool exitGame = false;
    while (!exitGame)
    {
        randomSeed((((micros()/2345)*memTimeAtFirstInput)/159)%98273461);
        exitGame = playOnce();
    }
}
void playMemory(long startTime)
{
    memTimeAtFirstInput = startTime;
    Serial.read();
    lcd.createChar(7, selected);
    lcd.createChar(0,error);
    waitForMemStartInput();
    loopMemory();
}
