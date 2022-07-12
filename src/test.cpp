/*#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <stdlib.h>

rgb_lcd lcd;
int leftFactor=0;
void setup(){
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // initialize the serial communications:
    Serial.begin(9600);
}

void loop()
{

    for(int i = 0; i<15; i++){
        lcd.setCursor(0,0);
        lcd.write(219);
        delay(15+leftFactor);
        lcd.setCursor(0,0);
        lcd.write(' ');
        lcd.setCursor(1,0);
        lcd.write(219);
        delay(15-leftFactor);
        lcd.setCursor(1,0);
        lcd.write(' ');
        leftFactor++;
    }
    leftFactor=0;
     for(int i = 0; i<15; i++){
        lcd.setCursor(0,0);
        lcd.write(219);
        delay(15-leftFactor);
        lcd.setCursor(0,0);
        lcd.write(' ');
        lcd.setCursor(1,0);
        lcd.write(219);
        delay(15+leftFactor);
        lcd.setCursor(1,0);
        lcd.write(' ');
        leftFactor++;
    }
    leftFactor=0;
     for(int i = 0; i<15; i++){
        lcd.setCursor(0,0);
        lcd.write(219);
        delay(15+leftFactor);
        lcd.setCursor(0,0);
        lcd.write(' ');
        lcd.setCursor(1,0);
        lcd.write(219);
        delay(15-leftFactor);
        lcd.setCursor(1,0);
        lcd.write(' ');
        leftFactor++;
    }
    leftFactor=0;
    for(int i = 0; i<15; i++){
        lcd.setCursor(0,0);
        lcd.write(219);
        delay(15-leftFactor);
        lcd.setCursor(0,0);
        lcd.write(' ');
        lcd.setCursor(1,0);
        lcd.write(219);
        delay(15+leftFactor);
        lcd.setCursor(1,0);
        lcd.write(' ');
        leftFactor++;
    }
    leftFactor=0;
    
} */