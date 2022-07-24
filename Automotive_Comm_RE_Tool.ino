// Intended for use with MPX-Toyota&Lexus, CANBUS-Everyone Sane, GMLAN-General Motors (Chevrolet & Holden)
#include <BeanMPX.h>
#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
#define L1 2
#define L2 3
#define L3 4
#define enter A5
#define left A4
#define right A3
#define send A2
boolean L1Val = 0;
boolean wait = 0;      // Functioally an encoder enable
byte mode = 2;         // Which comms mode is selected, 0 = CANBUS, 1 = GMLAN, 2 = MPX
int encoderOutput = 0; // Current value of the encoder, is reset to 0 frequently
int cursorPos = 1;     // Which message fram is being selected
boolean enable = 0;    // Internal while enable flag
short halt = 333;      // Time delay on the encoder

void isr1()
{
  if (wait == 0)
  {
    L1Val = 1;
  }
  if ((L1Val == 1) && (digitalRead(L3) == HIGH))
  {
    wait = 1;
    encoderOutput = encoderOutput + 1;
    // lcd.clear();
    // lcd.print(encoder);
    L1Val = 0;
  }
  if ((L1Val == 1) && (digitalRead(L2) == HIGH))
  {
    wait = 1;
    encoderOutput = encoderOutput - 1;
    // lcd.clear();
    // lcd.print(encoder);
    L1Val = 0;
  }
}

void setup()
{
  lcd.begin(20, 4);
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  pinMode(L3, INPUT);
  pinMode(enter, INPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  pinMode(send, INPUT);
  attachInterrupt(1, isr1, RISING);
  lcd.clear();
  Serial.begin(115200);
}

void loop()
{
  lcd.clear();
  lcd.print("Mode: ");
  bound(0, 2);
  mode = encoderOutput;
  if (mode == 0)
  {
    lcd.print("CANBUS");
    Serial.println("CANBUS");
    /*if(digitalRead(enter)==1){
      while(true){
        lcd.print("meme");
      }
    }*/
  }
  else if (mode == 1)
  {
    lcd.print("GMLAN");
    Serial.println("GMLAN");
  }
  else if (mode == 2)
  {
    lcd.print("MPX");
    Serial.println("MPX");
    if (digitalRead(enter) == 1)
    {
      encoderOutput = 0;
      BeanMPX bean;
      uint8_t mpx[] = {0x62, 0x01, 0x00, 0x00}; // MPX frame to send
      bean.ackMsg((const uint8_t[]){0xFE});     // Messages to acknowledge
      bean.begin();
      Serial.println("BeanMPX");
      // halt = 125;
      lcd.blink();
        lcd.setCursor(3, 1);
        lcd.print("Manual");
        lcd.setCursor(13,1);
        lcd.print("Auto");
      while (true)
      {
        if (digitalRead(left)==HIGH)
        {
          while (true)
          {
            lcd.setCursor(0, 1);
            lcd.print("Curr:");
            lcd.print(mpx[0], HEX);
            lcd.print("            ");
            lcd.setCursor(8, 1);
            lcd.print(mpx[1], HEX);
            lcd.setCursor(11, 1);
            lcd.print(mpx[2], HEX);
            lcd.setCursor(14, 1);
            lcd.print(mpx[3], HEX);
            // lcd.print(encoderOutput, HEX);
            // Serial.println(encoderOutput);
            if (digitalRead(right) == HIGH)
            {
              if (cursorPos == 2)
              {
                cursorPos = 0;
              }
              else
              {
                cursorPos++;
              }
            }
            if (digitalRead(left) == HIGH)
            {
              if (cursorPos == 0)
              {
                cursorPos = 2;
              }
              else
              {
                cursorPos--;
              }
            }
            switch (cursorPos)
            {
            case 0:
              lcd.setCursor(8, 1);
              break;

            case 1:
              lcd.setCursor(11, 1);
              break;

            case 2:
              lcd.setCursor(14, 1);
              break;
            }
            if (digitalRead(enter) == HIGH && cursorPos == 0)
            {
              lcd.noBlink();
              lcd.cursor();
              encoderOutput = 0;
              enable = 1;
              while (enable == 1)
              {
                bound(0, 255);
                mpx[1] = encoderOutput;
                wait = 0;
                lcd.setCursor(8, 1);
                lcd.print(mpx[1], HEX);
                if (digitalRead(left) == HIGH)
                {
                  enable = 0;
                }
              }
              lcd.noCursor();
              lcd.blink();
            }
            if (digitalRead(enter) == HIGH && cursorPos == 1)
            {
              lcd.noBlink();
              lcd.cursor();
              encoderOutput = 0;
              enable = 1;
              while (enable == 1)
              {
                bound(0, 255);
                mpx[2] = encoderOutput;
                wait = 0;
                lcd.setCursor(11, 1);
                lcd.print(mpx[2], HEX);
                if (digitalRead(left) == HIGH)
                {
                  enable = 0;
                }
              }
              lcd.noCursor();
              lcd.blink();
            }
            if (digitalRead(enter) == HIGH && cursorPos == 2)
            {
              lcd.noBlink();
              lcd.cursor();
              encoderOutput = 0;
              enable = 1;
              while (enable == 1)
              {
                bound(0, 255);
                mpx[3] = encoderOutput;
                wait = 0;
                lcd.setCursor(14, 1);
                lcd.print(mpx[3], HEX);
                if (digitalRead(left) == HIGH)
                {
                  enable = 0;
                }
              }
              lcd.noCursor();
              lcd.blink();
            }
            if (digitalRead(send) == HIGH)
            {
              if (!bean.isBusy())
              {
                bean.sendMsg(mpx, sizeof(mpx));
                // Serial.println("MPXSend");
              }
              lcd.setCursor(0, 3);
              lcd.print("Last:");
              lcd.print(mpx[0], HEX);
              lcd.print("            ");
              lcd.setCursor(8, 3);
              lcd.print(mpx[1], HEX);
              lcd.setCursor(11, 3);
              lcd.print(mpx[2], HEX);
              lcd.setCursor(14, 3);
              lcd.print(mpx[3], HEX);
              Serial.println("MPXSend");
            }
            wait = 0;
            // lcd.noCursor();
            delay(halt);
          }
        }
        else if (digitalRead(right)==HIGH){
          //Do something
        }
      }
    }
  }
  wait = 0;
  delay(halt);
}
void bound(int Min, int Max)
{ // So much spaghetti
  if (encoderOutput == Max + 1)
  {
    encoderOutput = Min;
  }
  else if (encoderOutput == Min - 1)
  {
    encoderOutput = Max;
  }
}

/* Code Graveyard
        //Original LCD code for the selectable line
        lcd.setCursor(0, 1);
        lcd.print(mpx[0], HEX);
        lcd.print("                  ");
        lcd.setCursor(3, 1);
        lcd.print(mpx[1], HEX);
        lcd.setCursor(6, 1);
        lcd.print(mpx[2], HEX);
        lcd.setCursor(9, 1);
        lcd.print(mpx[3], HEX);
        lcd.setCursor(12, 1);
        lcd.setCursor(0, 1);


*/