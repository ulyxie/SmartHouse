#include <Servo.h>
#define len(x) (sizeof(x) / sizeof(x[0]))

//int opened[] = {40 ,170,40 ,170,180,180,50 ,110, 95};
#define max 6
int opened[] = {-90 ,110,-140 ,110,180,180,-110 ,-110, -110};
int closed[] = {160,40 ,170,40 ,0,50 ,165,175,200};
int mot_ports[] = { 3, 4, 5, 7, 8, 9,11,12,13 };
Servo motors[9];

void door(uint8_t which, uint8_t openclose) {
  int angle = openclose==1? opened[which]+closed[which]:closed[which];
  motors[which].write(angle);
  if(which == 1 || which == 3 || which == 5) {
    int8_t assoc[] = { 0,6,0,7,0,8 };
    int dual = openclose==1? opened[assoc[which]]+closed[assoc[which]]:closed[assoc[which]];
    motors[assoc[which]].write(dual);
  }
}

void setup(){
  Serial.begin(115200);
  while(!Serial);
  for (int8_t i = 0; i < 9; i++)
  {
    motors[i].attach(mot_ports[i]);
    motors[i].write(closed[i]);
    delay(200);
  }
}
void loop(){
  while(Serial.available()>0){
    char read[3];
    Serial.readBytes(read, 3);
    for (uint8_t i = 0; i < len(read); i+=2) {
      uint8_t which=read[i];
      uint8_t oc=read[i+1];
      door(which, oc);
    }
  }
}