#include "pitches.h"
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "EEPROMAnything.h"

#define BuzzPin 8

#define ButtonPin 2
int buttonState = 0; 

#define IR_PIN0 A0 // Sensor 0 biggest     (0.25)
#define IR_PIN1 A1 // Sensor 1             (0.5)
#define IR_PIN2 A2 // Sensor 2             (0.01)
#define IR_PIN3 A3 // Sensor 3 smallest    (0.1)

int pin3Val = 0;
int pin2Val = 0;
int pin1Val = 0;
int pin0Val = 0;

byte pin3Count = 0;
byte pin2Count = 0;
byte pin1Count = 0;
byte pin0Count = 0;

//Servo servo_left; 
Servo servo_right; 
int angle = 0;  

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
bool ledState = true;
bool needFresh = false;

struct amountsStruct{
  int count3;
  int count2;
  int count1;
  int count0;
  float total;
}configuration;

amountsStruct amounts; 
float target = 5;

String tmpString = "";
String tmpString1 = "";

void setup(void){
  Serial.begin(9600);

  pinMode(BuzzPin, OUTPUT); // buzzer
  pinMode(IR_PIN0, INPUT);  // *****
  pinMode(IR_PIN1, INPUT);  // * IR pin 0 1 2 3
  pinMode(IR_PIN2, INPUT);  // *    big > > > > small
  pinMode(IR_PIN3, INPUT);  // *****

  pinMode(ButtonPin, INPUT_PULLUP); // button to switch the display

  EEPROM_readAnything(0, amounts);

  if(amounts.total < 0 || (target - amounts.total) <= 0){
    initEEPROM();
  }
   
  lcd.init();               // initialize the lcd 
  lcd.backlight();
  showLCD();

  //servo_left.attach(6);     // left lock
  servo_right.attach(7);    // right lock
  
  delay(5000);
}

void loop() {

  while (true){
    // Read in the ADC and convert it to a voltage:
    pin3Val = analogRead(IR_PIN3);
    pin2Val = analogRead(IR_PIN2);
    pin1Val = analogRead(IR_PIN1);
    pin0Val = analogRead(IR_PIN0);

    // IR Sensor Debug Output
    /*
    Serial.print(pin3Val);
    Serial.println(" - Pin A3");
    
    Serial.print(pin2Val);
    Serial.println(" - Pin A2");
    
    Serial.print(pin1Val);
    Serial.println(" - Pin A1");
    
    Serial.print(pin0Val);
    Serial.println(" - Pin A0");
    
    tmpString = String(pin3Val) + " ";
    tmpString = tmpString + pin2Val + " " ;
    tmpString = tmpString + pin1Val + " " + pin0Val;
    
    Serial.println(tmpString);
    */
    
    if(pin3Val < 790){
      Serial.print(pin3Val);
      Serial.println(" - Pin A3! Coin Detected!");
      amounts.count3++;
      needFresh = true;
      break;
    }
    if(pin2Val < 850){
      Serial.print(pin2Val);
      Serial.println(" - Pin A2! Coin Detected!");
      amounts.count2++;
      needFresh = true;
      break;
    }
    if(pin1Val < 740){
      Serial.print(pin1Val);
      Serial.println(" - Pin A1! Coin Detected!");
      amounts.count1++;
      needFresh = true;
      break;
    }
    if(pin0Val < 810){
      Serial.print(pin0Val);
      Serial.println(" - Pin A0! Coin Detected!");
      amounts.count0++;
      needFresh = true;
      break;
    }

    buttonState = digitalRead(ButtonPin);
    if(buttonState == LOW){
      ledState = !ledState;
      lcd.clear();
      showLCD();
      delay(20);
    }
  }
  
  if(needFresh){
    needFresh = false;
    amounts.total = amounts.count0*0.25 + amounts.count1*0.05 + amounts.count2*0.01 + amounts.count3*0.1;
    EEPROM_writeAnything(0, amounts);
    showLCD();
    coinSound();

    if ( (target - amounts.total) <= 0 ){
      odetoJoy();
      unlockBox();
      while (true){
        lcd.clear();
        lcd.setCursor(0,0);       
        lcd.print("Congratulations!");
        lcd.setCursor(0,1);
        tmpString = "You Save " + String(amounts.total) +"$!";
        lcd.print(tmpString);
        delay(5000);
        lcd.clear();
        lcd.setCursor(0,0);       
        lcd.print("Please Reboot");
        lcd.setCursor(0,1);
        lcd.print("To Restart!");
        delay(5000);

      }
    }
    
  }

  buttonState = digitalRead(ButtonPin);
  if(buttonState == LOW){
    ledState = !ledState;
    lcd.clear();
    showLCD();
    delay(500);
  }
  delay(10);
}

void coinSound(){
    tone(BuzzPin, BUZZ_B5, 300);
    delay(100);
    tone(BuzzPin, BUZZ_E6, 300);
    delay(1000);
    noTone(BuzzPin);
}
void greenMushroomShoud(){
    tone(BuzzPin, BUZZ_E6, 300);
    delay(150);
    tone(BuzzPin, BUZZ_G6, 300);
    delay(150);
    tone(BuzzPin, BUZZ_E7, 300);
    delay(150);
    tone(BuzzPin, BUZZ_C7, 300);
    delay(150);
    tone(BuzzPin, BUZZ_D7, 300);
    delay(150);
    tone(BuzzPin, BUZZ_G7, 300);

    delay(1000);
    noTone(BuzzPin);
}
void odetoJoy (){
  tone(BuzzPin, BUZZ_E6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_E6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_F6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_G6, 250); 
	delay(250);
	
	tone(BuzzPin, BUZZ_G6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_F6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_E6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_D6, 250); 
	delay(250);
	
	tone(BuzzPin, BUZZ_C6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_C6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_D6, 250); 
	delay(250);
	tone(BuzzPin, BUZZ_E6, 250); 
	delay(250);
	
	tone(BuzzPin, BUZZ_D6, 350); 
	delay(350);
	tone(BuzzPin, BUZZ_C6, 200); 
	delay(200);
	tone(BuzzPin, BUZZ_C6, 300); 
	delay(300);
}

void lockBox(){
  //servo_left.write(90);
  servo_right.write(90);
  delay(500);
}

void unlockBox(){
  //servo_left.write(0);
  servo_right.write(180);
  delay(500);
}

String formatMyNum(int number){
  //Serial.println(number);
  if(number < 10)
    return "  "+String(number);
  else if(number < 100 && number >= 10)
    return " "+String(number);
  else
    return String(number);
}
void initEEPROM(){
  amounts.count0 = 0;
  amounts.count1 = 0;
  amounts.count2 = 0;
  amounts.count3 = 0;
  amounts.total = 0;
  EEPROM_writeAnything(0, amounts);
  EEPROM_readAnything(0, amounts);
}

void showLCD(){
  if(ledState == true){
    tmpString = "25:"+ formatMyNum(amounts.count0) + "   10:" + formatMyNum(amounts.count3);
    tmpString1 = " 5:"+ formatMyNum(amounts.count1) + "    1:" + formatMyNum(amounts.count2);
    lcd.setCursor(0,0);       // show each amount (default)
    lcd.print(tmpString);
    lcd.setCursor(0,1);
    lcd.print(tmpString1);
  }
  else{
    tmpString = "Total: "+ String(amounts.total);
    tmpString1 = "Still Need: "+ String(target - amounts.total);
    lcd.setCursor(0,0);       // show each amount (default)
    lcd.print(tmpString);
    lcd.setCursor(0,1);
    lcd.print(tmpString1);
  }
  
}