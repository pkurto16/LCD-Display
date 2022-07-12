// #include <Arduino.h>
// #include <Wire.h>
// #include "rgb_lcd.h"
// #include <stdlib.h>

// rgb_lcd lcd;
// void setup(){
//     // set up the LCD's number of columns and rows:
//     lcd.begin(16, 2);
//     // initialize the serial communications:
//     Serial.begin(9600);
// }
// void hideAnimation(){
//     for(int i=0; i<36; i++){
//             if(i<16){
//                 lcd.setCursor(i,1);
//                 lcd.write(219);
//                 lcd.setCursor(15-i,0);
//                 lcd.write(219);
//             }
//             if(i>19){
//                lcd.setCursor(35-i,0);
//                 lcd.write(' ');
//                 lcd.setCursor(i-20,1);
//                 lcd.write(' '); 
//             }
//             delay(33);
//         }
// }
// void loop()
// {
//     lcd.clear();
//     lcd.setCursor(0,0);
//     lcd.write("Temp.: 71.2F ");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Humidity: 33.7%");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Light: 2730 LUX");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Water: Soil is");
//     lcd.setCursor(0,1);
//     lcd.write("Moist (352)");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Plant is stable");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Wait ~130 hrs");
//     lcd.setCursor(0,1);
//     lcd.write("before watering");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Temp.: 71.1F ");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Humidity: 33.9%");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Light: 2727 LUX");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Water: Soil is");
//     lcd.setCursor(0,1);
//     lcd.write("Moist (350)");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Plant is stable");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Wait ~130 hrs");
//     lcd.setCursor(0,1);
//     lcd.write("before watering");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Temp.: 71.0F ");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Humidity: 33.8%");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Light: 2732 LUX");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Water: Soil is");
//     lcd.setCursor(0,1);
//     lcd.write("Moist 351");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Plant is stable");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
//     lcd.write("Wait ~130 hrs");
//     lcd.setCursor(0,1);
//     lcd.write("before watering");
//     delay(2000);
//     hideAnimation();
//     lcd.setCursor(0,0);
// }