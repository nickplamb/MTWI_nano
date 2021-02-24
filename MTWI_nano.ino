// LiquidCrystal - Version: Latest 
#include <LiquidCrystal.h>
//#include "MusicNotes.h"
/*
Minute to win it one minte timer
*/


//inputs and output pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int stsp = A0; //Start / stop button
//const int rst = A1; //Reset button
const int buzz = 7; //buzzer pin
int notes[] = {800,600,200,200,200,349};
unsigned long timerDuration = 60000; //in milliseconds //1 minute
int secondsRemaining = 60; //
unsigned long startTime = 0; //Time that start/stop button initially pressed
unsigned long endTime = 0; 
bool runningTimer = true;
int finalCountdown = 10; //added to updateCountdown function
int switchThreshold = 1000; //threshold for testing if switch is high or low

bool isStarted = false;
//bool stspPreviousState = LOW;


void setup()
{
  // initialize serial and wait for port to open:
  Serial.begin(115200); //origionally 9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("starting up");
  lcd.begin(16, 2);
  lcd.clear();
  displayMessage("     MINUTE", "   TO WIN IT!");
  //pinMode(stsp, INPUT_PULLUP); //HIGH when open, LOW when pressed
  //pinMode(rst, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
  buzzerOff();
  delay(1500);
}

void loop()
{
  unsigned long currentTime = millis();
  bool isSwitchedOn = (analogRead(stsp) > switchThreshold);
  
  if(isStarted && !isSwitchedOn) 
  {
    //stop countdown
    displayMessage("     MINUTE", "   TO WIN IT!");
    isStarted = false;
  }
  else if (isSwitchedOn && !isStarted)
  {
    displayMessage("     MINUTE", "   TO WIN IT!");
    delay(1000);
    isStarted = true;
    preCountdown();
    resetTimer();
  }
  else if (isSwitchedOn && isStarted)
  {
    if (currentTime <= endTime)
    { 
      updateCountdown(currentTime);
    }
    
    if (secondsRemaining == 0) {
      
      for(int i = 0; i <= 8; i++)
      {
        buzzerOn(0,100);
        delay(100);
      }
      secondsRemaining = 60;
      displayMessage("MINUTE TO WIN IT", "   Time's Up!");
    } else if (buzz == HIGH && secondsRemaining <= 58 && secondsRemaining > 0){
      buzzerOff();
    } 
  }
}


void resetTimer()
{
  startTime = millis();
  buzzerOff();
  endTime = startTime + timerDuration;
  secondsRemaining = timerDuration / 1000;
  
  displayMessage(" Time remaining", String(secondsRemaining));
}

void preCountdown()
{    
  for (int i = 3; i > 0; i--)
  {
    if (!(analogRead(stsp) > switchThreshold)) { return; }
    
    displayMessage("Minute To Win It", "Begin In " + String(i) + "...");
    buzzerOn(2,250);
    delay(750);
  }
  
  if (!(analogRead(stsp) > switchThreshold)) { return; }
  buzzerOn(1,1000);
}

void updateCountdown(unsigned long currentTime)
{
  unsigned long timeElapsed = currentTime - startTime;
  int newSecondsRemaining = ceil((timerDuration - timeElapsed)/1000);

  
  if (newSecondsRemaining < secondsRemaining)
  {
    secondsRemaining = newSecondsRemaining;
    displayMessage("Time Remaining", String(secondsRemaining));
    if (secondsRemaining <= finalCountdown/2 ) {
      buzzerOn(1,100);
    } 
    else if (secondsRemaining <= finalCountdown){
      buzzerOn(2,100);
    }
  }
  
}

void buzzerOn(int note, int duration) 
{
  tone(buzz, notes[note]);
  delay(duration);
  buzzerOff();
}

void buzzerOff()
{
  noTone(buzz);
}


void displayMessage(String line1, String line2)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}
