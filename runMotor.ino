#include <Servo.h>

Servo ST1, ST2; //ST1 is right, ST2 is left
int spd = 150;  //same speed for all operation
int stp = 90;   //stop at 90
int allDelay = 2000; //same delay for all
int turnDelay = 4000; //since 2000 is 90 deg for 150, 4000 is 180

//increase delay for duration
//with sensor operation, delay is not finite

void setup(){
  Serial.begin(115200);
  
  ST1.attach(4, 1000, 2000); //5
  ST2.attach(7, 1000, 2000); //6  
  
}


void loop(){/*
  int data;
  if(Serial.available() > 0){
    Serial.write(Serial.read());  //bluetooth send back data
    data = Serial.read();
    if(data != 0){
      forward();
      pause();
      Serial.println("message received");
    }
  }*/
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
