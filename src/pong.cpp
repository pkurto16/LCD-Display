#include <Arduino.h>
#include "rgb_lcd.h"
#include <stdlib.h>
#include <miniGame.h>
extern rgb_lcd lcd;
int pongHighscore = 0;
int pongScore = 0;
int deathCount = 0;
long pongTimeAtFirstInput;
uint8_t enemyPaddleMiddle;
uint8_t playerPaddleMiddle;
uint8_t lastPos;

byte ball[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000};
byte enemyPaddleTop[8] = {
    0b11001,
    0b11001,
    0b11001,
    0b11001,
    0b11001,
    0b00001,
    0b00001,
    0b00001};
byte enemyPaddleBottom[8] = {
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001};
byte playerPaddleTop[8] = {
    0b10011,
    0b10011,
    0b10011,
    0b10011,
    0b10011,
    0b10000,
    0b10000,
    0b10000};
byte playerPaddleBottom[8] = {
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000};
byte heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

uint8_t ballXVel = 108; // velocity of 0 is at 128
uint8_t ballYVel = 142; // less than 128 is backwards and 10 actual velocity= 1 pixel/sec of movement
double ballXPos = 68;
double ballYPos = 12;

bool endPongGame(){
    bool restart = false;
    lcd.clear();
    String burner = Serial.readString();
    if(pongScore>pongHighscore) pongHighscore=pongScore;
    while(!restart){
        lcd.setCursor(6,0);
        lcd.write("GAME");
        lcd.setCursor(6,1);
        lcd.write("OVER");
        delay(1000);
        hideAnimation();
        lcd.setCursor(4,0);
        lcd.write("SCORE:");
        lcd.write((pongScore/10)+asciiNumConst);
        lcd.write((pongScore%10)+asciiNumConst);
        lcd.setCursor(2,1);
        lcd.write("HIGHSCORE:");
        lcd.write((pongHighscore/10)+asciiNumConst);
        lcd.write((pongHighscore%10)+asciiNumConst);
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
    deathCount = 0;
    hideAnimation();
    if(exit) return exit;
    pongScore=0;
    burner = Serial.readString();
    
    while(!Serial.available()){
    delay(100);
    }
    return false;
}
void increaseVel()
{
    if(ballXVel<128) {
        ballXVel-=random(0,4);
    }
    else{
        ballXVel+=random(0,4);
    }
    if(ballYVel<128) {
        ballYVel-=random(0,4);
    }
    else{
        ballYVel+=random(0,4);
    }
}
bool checkBallLoc()
{
    if (ballYPos + (ballYVel - 128) / 10 < 0)
    {
        ballYVel = 256 - ballYVel;
        ballYPos = 0;
    }
    if (ballYPos + (ballYVel - 128) / 10 > 15)
    {
        ballYVel = 256 - ballYVel;
        ballYPos = 15;
    }
    if ((uint8_t)ballXPos + (ballXVel - 128) / 10 < 24)
    {
        if((uint8_t)ballYPos+110-playerPaddleMiddle>=13||(uint8_t)ballYPos+110-playerPaddleMiddle<=7){
            return false;
        }
        ballXVel = 256 - ballXVel;
        ballXPos = 24;
        increaseVel();
    }
    if ((uint8_t)ballXPos + (ballXVel - 128) / 10 > 70)
    {
        if((uint8_t)ballYPos+110-enemyPaddleMiddle>=13||(uint8_t)ballYPos+110-enemyPaddleMiddle<=7){
            return false;
        }
        ballXVel = 256 - ballXVel;
        ballXPos = 70;
        increaseVel();
    }
    return true;
}
void clearBall()
{
    for (int i = 0; i < 8; i++)
    {
        ball[i] = 0b00000;
    }
}
void iterateBall()
{
    clearBall();
    lastPos = (uint8_t)ballXPos / 6 + 100 * ((uint8_t)ballYPos / 8);
    ballXPos += (ballXVel - 128) / 10;
    ballYPos += (ballYVel - 128) / 10;
    if (lastPos != (uint8_t)ballXPos / 6 + 100 * ((uint8_t)ballYPos / 8))
    {
        lcd.setCursor(lastPos % 100, lastPos / 100);
        lcd.write(" ");
    }
    uint8_t pixelYPos = (uint8_t)ballYPos % 8;
    byte ballDisplay = 0b00000;
    switch ((uint8_t)ballXPos % 6)
    {
    case 0:
        ballDisplay = 0b10000;
        break;
    case 1:
        ballDisplay = 0b01000;
        break;
    case 2:
        ballDisplay = 0b00100;
        break;
    case 3:
        ballDisplay = 0b00010;
        break;
    case 4:
        ballDisplay = 0b00001;
        break;
    default:
        break;
    }
    if (pixelYPos < 8)
        ball[pixelYPos] = ballDisplay;
    lcd.createChar(0, ball);
    lcd.setCursor((uint8_t)ballXPos / 6, (uint8_t)ballYPos / 8);
    lcd.write((unsigned char)0);
}
void initChars()
{
    lcd.createChar(1, enemyPaddleTop);
    lcd.createChar(2, enemyPaddleBottom);
    lcd.createChar(3, playerPaddleTop);
    lcd.createChar(4, playerPaddleBottom);
}
void moveEnemyPaddle()
{
    if (random(0, 100) > 3)
    {
        uint8_t ballMiddle = (uint8_t)ballYPos + 100;
        uint8_t paddleMiddle = enemyPaddleMiddle;
        if (ballMiddle > enemyPaddleMiddle)
        {
            if(ballYVel>=148||ballYVel<=108){
                paddleMiddle = enemyPaddleMiddle + 2;
            }
            else{
                paddleMiddle = enemyPaddleMiddle + 1;
            }
        }
        if (ballMiddle < enemyPaddleMiddle)
        {
            if(ballYVel>=148||ballYVel<=108){
                paddleMiddle = enemyPaddleMiddle - 2;
            }
            else{
                paddleMiddle = enemyPaddleMiddle - 1;
            }
        }
        if (paddleMiddle < 103)
        {
            paddleMiddle = 103;
        }
        if (paddleMiddle > 113)
        {
            paddleMiddle = 113;
        }
        for (int i = 0; i < 8; i++)
        {
            enemyPaddleTop[i] = 0b00001;
            enemyPaddleBottom[i] = 0b00001;
        }
        for (int i = 0; i < 17; i++)
        {
            if (paddleMiddle - i > 96 && paddleMiddle - i < 104)
            {
                if (i < 8)
                {
                    enemyPaddleTop[i] = 0b11001;
                }
                if (i > 8)
                {
                    enemyPaddleBottom[i - 9] = 0b11001;
                }
            }
        }
        enemyPaddleMiddle = paddleMiddle;
    }
}
uint8_t checkForMove(){
    String inputs = Serial.readString();
    uint8_t wCount = 0;
    uint8_t sCount = 0;
    for(unsigned int i = 0; i<inputs.length(); i++){
        if(inputs.charAt(i)=='w'||inputs.charAt(i)=='W') wCount++;
        if(inputs.charAt(i)=='s'||inputs.charAt(i)=='S') sCount++;
    }
    if(wCount>3){
        wCount=3;
    }
    if(sCount>3){
        sCount=3;
    }
    if(wCount>sCount) return wCount*-1;
    if(wCount<sCount) return sCount;
    return 0;
}
void movePlayerPaddle()
{
    uint8_t paddleMiddle = playerPaddleMiddle+checkForMove();;
    if (paddleMiddle < 103)
    {
        paddleMiddle = 103;
    }
    if (paddleMiddle > 113)
    {
        paddleMiddle = 113;
    }
    for (int i = 0; i < 8; i++)
    {
        playerPaddleTop[i] = 0b10000;
        playerPaddleBottom[i] = 0b10000;
    }

    for (int i = 0; i < 17; i++)
    {
        if (paddleMiddle - i > 96 && paddleMiddle - i < 104)
        {
            if (i < 8)
            {
                playerPaddleTop[i] = 0b10011;
            }
            if (i > 8)
            {
                playerPaddleBottom[i - 9] = 0b10011;
            }
        }
    }
    playerPaddleMiddle = paddleMiddle;
}
void drawNewPaddles()
{
    lcd.setCursor(3, 0);
    lcd.write((unsigned char)3);
    lcd.setCursor(3, 1);
    lcd.write((unsigned char)4);
    lcd.setCursor(12, 0);
    lcd.write((unsigned char)1);
    lcd.setCursor(12, 1);
    lcd.write((unsigned char)2);
}
void iteratePaddles()
{
    moveEnemyPaddle();
    movePlayerPaddle();
    initChars();
}
void iterateOnce()
{
    iteratePaddles();
    iterateBall();
}
void writeGameInfo(){
    lcd.setCursor(0,0);
    for(int i = 0; i<3-deathCount; i++) lcd.write((unsigned char) 6);
    lcd.setCursor(0,1);
    lcd.write('S');
    if(pongScore<10){
        lcd.write(pongScore+asciiNumConst);
    }
    else{
        lcd.write(pongScore/10+asciiNumConst);
        lcd.write(pongScore%10+asciiNumConst);
    }
}
void playAPoint()
{
    bool ballInPlay = true;
    writeGameInfo();
    randomSeed((((micros()/2345)*pongTimeAtFirstInput)/159)%98273461);
    while (ballInPlay)
    {
        iterateOnce();
        ballInPlay = checkBallLoc();
        delay(150);
    }
}
void playScoredMessage(){
    lcd.setCursor(3,0);
    lcd.write("YOU SCORED");
    lcd.setCursor(0,1);
    lcd.write("TYPE TO CONTINUE");
    delay(500);
    Serial.read();
    while(!Serial.available()) delay(50);
}
void playDeathMessage(){
    lcd.setCursor(4,0);
    lcd.write("YOU DIED");
    lcd.setCursor(0,1);
    lcd.write("TYPE TO CONTINUE");
    Serial.read();
    delay(500);
    while(!Serial.available()) delay(50);
}
void resetBall(){
    ballXVel = random(100,118);
    ballYVel = random(138,150);
    ballXPos = 68;
    ballYPos = random(0,14);
    clearBall();
}
void loopPong()
{
    bool exitGame = false;
    while (!exitGame)
    {
        drawNewPaddles();
        playAPoint();
        lcd.clear();
        if(ballXPos>50){
            pongScore++;
            playScoredMessage();
        }
        else{
            deathCount++;
            if (deathCount > 2)
            {
                hideAnimation();
                exitGame = endPongGame();
            }
            else{
                playDeathMessage();
            }
        }
        lcd.clear();
        resetBall();
    }
}
void waitForStartInputPong(){
    lcd.setCursor(2,0);
    lcd.write("TYPE ANY KEY");
    lcd.setCursor(4,1);
    lcd.write("TO START");
    delay(500);
    Serial.read();
    while(!Serial.available()){
        delay(50);
    }
}
void playPong(long startTime)
{
    pongTimeAtFirstInput = startTime;
    lcd.createChar(6, heart);
    waitForStartInputPong();
    delay(100);
    lcd.clear();
    loopPong();
}
