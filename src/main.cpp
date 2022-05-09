#include <Arduino.h>
#include <Servo.h>

int8_t ir[] = { 45, 25, 27, 29, 31, 33,35,37,39,41,43 };

int mot_ports[] = { 3, 4, 5, 7, 8,9,11,12,13 };
int opened[] = { -90, 110,-110, -140, 110, -180, 180, 110, -110 };
int closed[] = { 160, 40, 165, 170, 40, 175, 0, 50, 200 };
int8_t prev = (int8_t)127;
Servo motors[9];

void door(uint8_t which, uint8_t oc) {
  int angle = oc == 1 ? opened[which] + closed[which] : closed[which];
  motors[which].write(angle);
  if (which == 1 || which == 4 || which == 7) {
    int8_t assoc[] = { 0,2,0,0,5,0,0,8 };
    int dual = oc == 1 ? opened[assoc[which]] + closed[assoc[which]] : closed[assoc[which]];
    motors[assoc[which]].write(dual);
  }
}

int8_t leds[] = { 22, 24, 26, 28, 30, 32 }; //0 == hall
void lon(int8_t which) { digitalWrite(leds[which], HIGH); }
void loff(int8_t which) { digitalWrite(leds[which], LOW); }

/*
 * #####################
 * #####################
 * ##                 ##
 * ##  IR DETECTIONS  ##
 * ##                 ##
 * #####################
 * #####################
 */

void sensor0()
{
  if (prev == 1) {
    door(0, 0);
    loff(1);
    prev = -1;
  }
  else if (prev == -2) {
    door(0, 1);
    loff(0);
    lon(1); 
    prev = 0;
  }

}

void sensor1()
{
  if(prev != 0) return;
  door(0, 1);
  lon(0);
  prev = 1;
}
void sensor2()
{
  if (prev != -1) return;
  door(1, 1);
  lon(2);
  prev = 2;
}
void sensor3()
{
  if(prev != 2) return;
  loff(0);
  door(3, 1);
  delay(400);
  door(1, 0);
  lon(3);
  prev = 3;
}
void sensor4()
{
  if (prev != 3) return;
  door(4, 1);
  delay(300);
  lon(0);
  loff(2);
  door(3, 0);
  prev = 4;
}
void sensor5()
{
  if(prev != 4) return;
  door(4, 0);
  loff(3);
  prev = 5;
}
void sensor6()
{
  if (prev!=5) return;
  door(6, 1);
  lon(4);
  delay(50);
  loff(0);
  prev = 6;
}
void sensor7()
{
  if (prev != 6) return;
  door(6, 1);
  lon(0);
  prev = 7;
}
void sensor8(){
  if (prev != 7) return;
  door(6, 0);
  loff(4);
  prev=8;
}
void sensor9()
{
  if (prev == 10) {
    door(7, 0);
    loff(5);
    prev = -2;
  }
  else if (prev == 8) {
    lon(5);
    loff(0);
    door(7, 1);
    prev = 9;
  }
}
void sensor10()
{
  if (prev != 9) return;
  door(7, 1);
  lon(0);
  prev = 10;
}


void setup()
{ 
  for (int8_t i = 0; i < 9; i++)
  {
    motors[i].attach(mot_ports[i]);
    motors[i].write(closed[i]);
    delay(200);
  }
  for (int8_t i = 0; i < 6; i++)
  {
    pinMode(leds[i], OUTPUT);
    loff(i);
  }
  for (int8_t i = 0; i < 11; i++)
  {    
    pinMode(ir[i], INPUT);
  }
  Serial.begin(115200);
  while(!Serial);

  delay(200);
  prev = -2;
  for (int8_t i = 0; i < 3; i++)
  {
    for (int8_t j = 0; j < 6; j++)
    {
      lon(j);
    }
    delay(500);
    for (int8_t j = 0; j < 6; j++)
    {
      loff(j);
    }
    delay(500);
  }
  lon(0);
}

void loop()
{
  if (digitalRead(ir[0]) == LOW) {
    sensor0();
  }

  if (digitalRead(ir[1]) == LOW) {
    sensor1();
  }

  if (digitalRead(ir[2]) == LOW) {
    sensor2();
  }

  if (digitalRead(ir[3]) == LOW) {
    sensor3();
  }

  if (digitalRead(ir[4]) == LOW) {
    sensor4();
  }

  if (digitalRead(ir[5]) == LOW) {
    sensor5();
  }

  if (digitalRead(ir[6]) == LOW) {
    sensor6();
  }

  if (digitalRead(ir[7]) == LOW) {
    sensor7();
  }
  if (digitalRead(ir[8]) == LOW) {
    sensor8();
  }
  if (digitalRead(ir[9]) == LOW) {
    sensor9();
  }
  if (digitalRead(ir[10]) == LOW) {
    sensor10();
  }
}
