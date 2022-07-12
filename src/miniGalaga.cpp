#include <Arduino.h>
#include "rgb_lcd.h"
#include <stdlib.h>
#include <miniGame.h>
extern rgb_lcd lcd;
// const char filename[] = "GALAGA.WAV";
// SDWaveFile = waveFile;
// waveFile = SDWaveFile(filename);
// AudioOutI2S.play(waveFile);
char spaceShip;
char monster;
char bullet;
byte monsterArray[8] = {
  B10001,
  B01110,
  B10001,
  B10101,
  B10001,
  B01110,
  B01010,
  B10001
};
byte spaceShipArray[8]={
  B00100,
  B01010,
  B10110,
  B11011,
  B10110,
  B01010,
  B00100,
  B00000
};
byte bulletArray[8] = {
  B00000,
  B00000,
  B11110,
  B10001,
  B11110,
  B00000,
  B00000,
  B00000
};

int posData[] = {11,9999,9999,9999,9999,9999}; //first 2 nums are x coordinate x.y second 2 are y = x.y the last 2 are xvel,yvel This initial value is the pos of the player. Pleyers don't have velocity.
char matchingEntities[] = {spaceShip,bullet,bullet,monster,monster,monster};
unsigned long timeAtLastShot = 0;
unsigned long timeAtLastSpawn = 0;
int highscore = 0;
int score = 0;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    analogWrite(6, 1);
    delayMicroseconds(tone);
    analogWrite(6, 0);
    delayMicroseconds(tone);
  }
}
void resetVars(){
    score = 0;
    posData[0]=11;
    for(int i = 1; i<6; i++){
        posData[i]=9999;
    }
}
bool endGalagaGame(){
    playTone(487,100);
    bool restart = false;
    lcd.clear();
    String burner = Serial.readString();
    if(score>highscore) highscore=score;
    for(int i = 1; i<5; i++) posData[i]=9999;
    posData[0]=11;
    while(!restart){
        lcd.setCursor(6,0);
        lcd.write("GAME");
        lcd.setCursor(6,1);
        lcd.write("OVER");
        delay(1000);
        hideAnimation();
        lcd.setCursor(4,0);
        lcd.write("SCORE:");
        lcd.write((score/10)+48);
        lcd.write((score%10)+48);
        lcd.setCursor(2,1);
        lcd.write("HIGHSCORE:");
        lcd.write((highscore/10)+48);
        lcd.write((highscore%10)+48);
        delay(1000);
        hideAnimation();
        lcd.setCursor(5,0);
        lcd.write("TYPE Q");
        lcd.setCursor(3,1);
        lcd.write("TO GO BACK");
        unsigned long timer = millis();
        while(millis()<timer+3000&&!restart){
            String inputs = Serial.readString();
            for(unsigned int i=0; i<inputs.length(); i++) if(inputs.charAt(i)=='q' || inputs.charAt(i)=='Q' ) restart = true;
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
    resetVars();
    if(exit) return exit;
    
    characterStartAnimation();
    burner = Serial.readString();
    
    while(!Serial.available()){
    delay(100);
    }
    return false;
}
void move(int direction){ //1 is up, 2 is down, 3 is right, 4 is left
    int xPos = posData[0]/10;
    int yPos = posData[0]%10;
    lcd.setCursor(xPos,yPos);
    lcd.write(' ');

    if(direction/3<1){
        yPos+=direction*2-3;
        if(yPos<0) yPos=0;
        if(yPos>1) yPos=1;
    }
    else{
        xPos-= direction*2-7;
        if(xPos<1) xPos=1;
        if(xPos>14) xPos=14; 
    }

    lcd.setCursor(xPos, yPos);
    posData[0] = 10*xPos+yPos;
    lcd.write(spaceShip);
    
}
void shoot(){
    if(timeAtLastShot<millis()-333){
        timeAtLastShot=millis();
        if(posData[1]==9999){
            posData[1]=posData[0]*100+21;
            playTone(1500,40);
        }
        else{
            if(posData[2]==9999){
                posData[2]=posData[0]*100+21;
                playTone(1500,40);
            }
        }
        
    }
    
}
int drawEntitiesToPos(int entityData, int entityNum){
    int xPos = entityData/1000;
    int yPos = (entityData/100)%10;
    int xVel = ((entityData)%100)/10; //max xVel is 2 0 is negative 2 is positive 1 is zero
    int yVel = (entityData)%10; //max yVel is 2. 0 is negative 2 is positive 1 is zero
    if(!(posData[0]/10==xPos&&posData[0]%10==yPos))
    lcd.setCursor(xPos,yPos);
    lcd.write(' ');
    xPos += xVel-1;
    if(xPos<1||xPos>15) return 9999; //returning 9999 destroys the entity
    yPos = (yPos+yVel-1)%2;
    lcd.setCursor(xPos,yPos);
    lcd.write(matchingEntities[entityNum]);
    return 1000*xPos+100*yPos+10*xVel+yVel;
}
void findNewPosOfEntities(){
    for(int i = 1; i<6; i++){
        if(posData[i]!=9999&&posData[i]/1000<16&&posData[i]/1000>-1){
            posData[i] = drawEntitiesToPos(posData[i], i);
        } 
        
    }
    lcd.setCursor((posData[0]/10),posData[0]%10);
    lcd.write(spaceShip);
}
void recieveInput(){
    Serial.setTimeout(1);
    String serial = Serial.readStringUntil('w'||'s'||'d'||'a'||'.');
    switch (serial.charAt(serial.length()-1)){
        case 'w': move(1); break;
        case 's': move(2); break;
        case 'd': move(3); break;
        case 'a': move(4); break;
        case '.': shoot(); break;
        default: break;
    }
}
extern void waitForStartInput(){
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
void characterStartAnimation(){
    lcd.clear();
    for(int i = 0; i<50; i++){
        lcd.setCursor(1,1);
        lcd.write(' ');
        delay(33-i/3);
        lcd.setCursor(1,1);
        lcd.write(spaceShip);
        delay(33*(1+pow(i,2)/3333));
    }
}
void checkNewEnemySpawn(){
    if(timeAtLastSpawn<millis()-300){
        randomSeed((((millis()/2345)*timeAtLastShot)/159)%98273461);
        timeAtLastSpawn=millis();
        if(random(0,1000)>333){
            if(posData[3]==9999){
                if(random(0,100)<50){
                    posData[3]=15000+100*(random(0,100)%2)+1;
                } 
                else{
                    posData[3]=1000+100*(random(0,100)%2)+21;
                } 
            }
            else{
                if(posData[4]==9999){
                    if(random(0,100)<50){
                        posData[4]=15000+100*(random(0,100)%2)+1;
                    } 
                    else{
                        posData[4]=1000+100*(random(0,100)%2)+21;
                    }
                }
                else{
                    if(posData[5]==9999){
                    if(random(0,100)<50){
                        posData[5]=15000+100*(random(0,100)%2)+1;
                    } 
                    else{
                        posData[5]=1000+100*(random(0,100)%2)+21;
                    }
                }
                }
            }
        }
    }
}
void destroyAnimation(int position, int bulletPos){
    lcd.setCursor(bulletPos/10,bulletPos%10);
    lcd.write(' ');
    lcd.setCursor(position/10,position%10);
    lcd.write(' ');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('`');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write(136);
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('°');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('^');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('*');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('+');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('t');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('F');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('A');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write(219);
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write('#');
    delay(33);
    lcd.setCursor(position/10,position%10);
    lcd.write(' ');

}
void destroyOverlappingEntities(){
    for(int i = 1; i<3; i++){
        for(int j = 3; j<6; j++){
            if((posData[i]/100==posData[j]/100 || posData[i]/100==(posData[j]/100)-10 ) && posData[i]!=9999){
                destroyAnimation(posData[j]/100, posData[i]/100);
                posData[i]=9999;
                posData[j]=9999;
                score++;
                playTone(320,100);
                
            }
        }
    }
}
bool checkPlayerDestruction(){
    if(posData[0]==posData[3]/100||posData[0]==posData[4]/100){
        destroyAnimation(posData[0], posData[0]);
        return true;

    }
    return false;
}
void updateScore(){
    lcd.setCursor(0,0);
    lcd.write((score/10)+48);
    lcd.setCursor(0,1);
    lcd.write((score%10)+48);
}
void loopGalaga(){
    bool exitGame = false;
    while(!exitGame){
        long time = millis();
        while(millis()-time<100){
            if(Serial.available()) recieveInput();
        }
        checkNewEnemySpawn();
        findNewPosOfEntities();
        destroyOverlappingEntities();
        if(checkPlayerDestruction()){
            exitGame = endGalagaGame();
        }
        updateScore();
    }
}
void createChars(){
    lcd.createChar(0, monsterArray);
    lcd.createChar(1,spaceShipArray);
    lcd.createChar(2, bulletArray);
    monster = (unsigned char) 0;
    spaceShip = (unsigned char) 1;
    bullet = (unsigned char) 2;
}
void rewriteMatchingEntities(){
    matchingEntities[0] = spaceShip;
    matchingEntities[1] = bullet;            
    matchingEntities[2] = bullet;
    matchingEntities[3] = monster;
    matchingEntities[4] = monster;
    matchingEntities[5] = monster;
}
void playGalaga(){
    createChars();
    rewriteMatchingEntities();
    waitForStartInput();
    characterStartAnimation();
    String burner = Serial.readString();
    while(!Serial.available()) delay(100);
    loopGalaga();
}





