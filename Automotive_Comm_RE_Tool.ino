//Intended for use with MPX-Toyota&Lexus, CANBUS-Everyone Sane, GMLAN-General Motors (Chevrolet & Holden)

#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,8,9,10,11,12);
#define L1 2
#define L2 3
#define L3 4
#define enter A5
#define left A4
#define right A3
boolean L1Val = 0;
boolean wait = 0;
byte mode = 0;
int encoderOutput = 0;
short halt = 333;//Time delay on the encoder

void isr1 ()  {
  if (wait == 0) {
    L1Val = 1;
  }
    if ((L1Val == 1) && (digitalRead(L3) == HIGH)) {
    wait = 1;
    encoderOutput = encoderOutput + 1;
    //lcd.clear();
    //lcd.print(encoder);
    L1Val = 0;
  }
  if ((L1Val == 1) && (digitalRead(L2) == HIGH)) {
    wait = 1;
    encoderOutput = encoderOutput - 1;
    //lcd.clear();
    //lcd.print(encoder);
    L1Val = 0;
  }
}

void setup() {
  lcd.begin(20, 4);
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  pinMode(L3, INPUT);
  pinMode(enter, INPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  attachInterrupt (1, isr1, RISING);
  lcd.clear();
  Serial.begin(9600);
  
}

void loop() {
  lcd.clear();
  lcd.print("Mode: ");
  bound(0,2);
  mode = encoderOutput;
  if(mode == 0){
    lcd.print("CANBUS");
    if(digitalRead(enter)==1){
      while(true){
        lcd.print("meme");
      }
    }
  }
  else if(mode == 1){
    lcd.print("GMLAN");
  }
  else if(mode == 2){
    lcd.print("MPX");
  }
  wait = 0;
  delay(halt);
  
}

void bound(int Min,int Max) {//So much spaghetti
 if (encoderOutput == Max + 1){
    encoderOutput = Min;
  }
  else if (encoderOutput == Min - 1){
    encoderOutput = Max;
  }
  
}
