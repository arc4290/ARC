
#include <Servo.h>

Servo ST1, ST2; //ST1 is right, ST2 is left
int spd = 150;  //same speed for all operation
int stp = 90;   //stop at 90
int allDelay = 2000; //same delay for all

//increase delay for duration
//with sensor operation, delay is not finite

void setup(){
  ST1.attach(5, 1000, 2000);
  ST2.attach(6, 1000, 2000);
  turnLeft();
  pause();
}
void loop(){
  //turnLeft();
}

void pause(){
  ST1.write(stp);
  ST2.write(stp);
  delay(allDelay);
}

void forward(){
  ST1.write(spd);
  ST2.write(spd);
  delay(allDelay);
}

void backward(){
  int spd2 = 180-spd;  //equavilent backward speed
  ST1.write(spd2);
  ST2.write(spd2);
  delay(allDelay);
}

void turnRight(){
  ST1.write(spd);
  ST2.write(stp);
  delay(allDelay);
}

void turnLeft(){
  ST1.write(stp);
  ST2.write(spd);
  delay(allDelay);
}
