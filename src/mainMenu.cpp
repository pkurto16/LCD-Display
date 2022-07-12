#include <Arduino.h>
#include "rgb_lcd.h"
#include <stdlib.h>
#include <miniGame.h>
rgb_lcd lcd;
bool playingGames = true;
const int numOfGames = 3;
String gameTexts[numOfGames] = {"Game 1: Galaga ","Game 2: PONG ", "Game 3: Memory "};

void hideAnimation(){
    for(int i=0; i<36; i++){
            if(i<16){
                lcd.setCursor(i,1);
                lcd.write(219);
                lcd.setCursor(15-i,0);
                lcd.write(219);
            }
            if(i>19){
               lcd.setCursor(35-i,0);
                lcd.write(' ');
                lcd.setCursor(i-20,1);
                lcd.write(' '); 
            }
            delay(50);
        }
}
void setup(){
    lcd.begin(16, 2);
    pinMode(6, OUTPUT);
    Serial.begin(9600);
}
bool checkGameSelection(){
    Serial.setTimeout(1);
    String serial = Serial.readStringUntil('1'||'2'||'3'||'4');
    switch (serial.charAt(serial.length()-1)){
        case '1':
        lcd.clear();
        hideAnimation();
        playGalaga(); 
        break;
        case '2':
        lcd.clear();
        hideAnimation();
        playPong(micros());
        break;
        case '3':
        lcd.clear();
        hideAnimation();
        playMemory(micros());
        break;
        case '4': break;
        default: return false;
    }
    return true;
}
void printOneGame(int gameID){
    lcd.setCursor(15,0);
    for(unsigned int i=0; i<gameTexts[gameID].length()-1; i++) lcd.write(gameTexts[gameID].charAt(i));
    for(unsigned int i = 0; i<gameTexts[gameID].length()+14; i++){
        lcd.scrollDisplayLeft();
        if(checkGameSelection()){
            lcd.clear();
            break;
        } 
        delay(100);
    }
    lcd.clear();
}
void scrollAndCheck(){
    lcd.clear();
    for(int i = 0; i<numOfGames; i++){
        printOneGame(i);
    }
}
void loop(){
    lcd.setCursor(4,0);
    lcd.write("WELCOME!");
    lcd.setCursor(2,1);
    lcd.write("CHOOSE GAME:");
    delay(2000);
    hideAnimation();
    while(playingGames){
        scrollAndCheck();
    }
    lcd.clear();
   
}





