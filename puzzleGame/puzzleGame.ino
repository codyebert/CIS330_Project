#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;


//Init sensor locations
const int pinButton = 3;
const int pinLed    = 4;
const int potentiometer = A0;

//Probably not used variables
//bool validNum = false; // I dont think this is used
//int count = 0;//I dont think this is used


//Set to true if the user decides to play/is currently playing
boolean play = false;
boolean playing = false;//may not be needed
//Each mini game has an assigned value, a random game is chosen during play
int gameNum = 0;

//Buzzer variables
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 2, 3};
int tempo = 300;

//Display Colors
const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

//Init timer
int timer = 100000;

//Combo breaker
//Target numbers
int nums[] = { 100, 560, 380 };
//Index for target nums and color change
int i = 0;

//Play tone on buzzer
//void playTone(int tone, int duration) {
//    for (long i = 0; i < duration * 1000L; i += tone * 2) {
//        digitalWrite(pinLed, HIGH);
//        delayMicroseconds(tone);
//        digitalWrite(pinLed, LOW);
//        delayMicroseconds(tone);
//    }
//}

//Play note on buzzer
//void playNote(char note, int duration) {
//    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
//    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
//
//    // play the tone corresponding to the note name
//    for (int i = 0; i < 8; i++) {
//        if (names[i] == note) {
//            playTone(tones[i], duration);
//        }
//    }
//}
void changeColor()
{
  //Remove blue color
  lcd.setPWM(REG_BLUE, 0);   
  int value = analogRead(potentiometer);
  
  //Green, within range of target
  if( value < nums[i] +25 && value > nums[i] -25)
  {
    //Sets new colors
    lcd.setPWM(REG_GREEN, 255); 
    lcd.setPWM(REG_RED, 0);     
  }
  //Red, far outside range of target
  else if(value > nums[i]+100 || value < nums[i] -100)
  {   
      //Sets new colors
      lcd.setPWM(REG_RED, 255);
      lcd.setPWM(REG_GREEN, 0);
  }
  //Yellow, within medium range of target
  else if((value > nums[i]+25 && value < nums[i]+100))
  {
      //Sets new colors
       lcd.setPWM(REG_RED, 255);
       lcd.setPWM(REG_GREEN, 255);

  }
  //Yellow, within medium range of target
  else if((value < nums[i]-25 && value > nums[i]-100))
  {
      //Sets new colors
       lcd.setPWM(REG_RED, 255 );
       lcd.setPWM(REG_GREEN, 255 );

  }
}


void comboBreaker()
{
    //Reads rotary angle
    int value = analogRead(potentiometer);   
    int desiredValue;

    //Grabs a target number from array 
    desiredValue = nums[i];
    
    //If within value range
    if((value <= desiredValue+25)&& (value >= desiredValue-25))
    {
      //Adds one to counter
      if(digitalRead(pinButton) )
      {
          //IF three combos have been reached
          if(i == 2)
          {             
             lcd.setCursor(0, 0);
             lcd.print("You win");//Can remove from final product
             //play = false;
            // validNum = true;
             i = 0;
             
             //Adds time to the timer if task completed
             timer += 3000;
             delay(1000);//Can remove from final
            
          }
          else
          {
             i++;      
          }
         }       
      }  
    
    
}
void setup() {
    //Init Display
    lcd.begin(16, 2);//You might be able to add lines and spaces, depending on values entered at initialization
    lcd.setRGB(colorR, colorG, colorB);
    //Init button
    pinMode(pinButton, INPUT);   
}

void loop() {

  //On button activation
  if(digitalRead(pinButton))
  {
      //Clears screen
     lcd.clear();
     
     //Sets user to play status
     play = true; 
     
     //If not playing, set to playing, generate a random number to choose game.
    if(playing == false)
    {
      playing = true;
      gameNum = rand() % 3;
    }
  }
 if(play == true)
  {  
      //Timer
      lcd.setCursor(0,0);
      lcd.print(timer/1000);
      timer -= 3;

      //Game one: Combo breaker
      if(gameNum == 0)
      {
        changeColor();
        comboBreaker();
        lcd.setCursor(0,1);
        lcd.print("GAME 1");//Remove from final product
      }
      //Game two: Simon Says/Morse code
      else if(gameNum == 1)
      {
        changeColor();
        comboBreaker();
        lcd.setCursor(0,1);
        lcd.print("GAME 2"); //Remove from final product   
      }
      //Game three: Simon Says/Morse code
      else if(gameNum == 2)
      {   
        changeColor();
        comboBreaker();
        lcd.setCursor(0,1);
        lcd.print("GAME 3");//Remove from final product
        
      }
      
    
  }
  //Main menu is displayed if user is not currently playing
  else if(play == false )
  {
      lcd.setRGB(colorR, colorG, colorB);  
      mainMenu();
  }
  //If the timer reaches 0, displays "You lose", returns to main menu
  if(timer == 0)
  {
    lcd.clear();
    lcd.print("YOU LOSE BITCH");
    delay(1000);
    lcd.clear();
    play = false;
    playing = false;
    timer =1;
  }
 
 
  

   
}
//Main menu function
void mainMenu()
{
  //Background set to blue
 lcd.setPWM(REG_BLUE, 255); 
 int value = analogRead(potentiometer); 

 //Display Main menu text
 lcd.setCursor(0, 0);
 lcd.print("Choose Your");
 lcd.setCursor(0, 1);
 lcd.print("difficulty");

 //Displays different levels of difficulty based on rotary angle
 //Sets timer based on chosen difficulty
 lcd.setCursor(14, 2);
 if(value < 200)
 {
    timer = 10000;
   lcd.print("5");
 }
 else if(value > 200 && value < 400)
 { 
     timer = 150000;
     lcd.print("4");
 }
 else if(value > 400 && value < 600)
 { 
   timer = 200000;
   lcd.print("3");
 }
 else if(value > 600 && value < 800)
 { 
   timer = 250000;
    lcd.print("2");
 }
 else if(value > 800 && value < 1014)
 {
    timer = 300000;
   lcd.print("1");
   
   
 }
}

