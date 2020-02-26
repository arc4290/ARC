#include <Wire.h>
#include <Servo.h>

Servo ST1, ST2; //ST1 is right, ST2 is left
int spd = 150;  //same speed for all operation
int stp = 90;   //stop at 90
int allDelay = 500; //same delay for all
int turnDelay = 4000; //since 2000 is 90 deg for 150, 4000 is 180
int hl = 0;

void setup(){
  // Start the I2C Bus as Master
  Wire.begin(3); 
  
  ST1.attach(5, 1000, 2000); //5
  ST2.attach(6, 1000, 2000); //6  
 
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
  DDRD |= B00000100;
}
//char c = 'a';

void loop(){
  /*
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(c);              // sends x 
  Wire.endTransmission();    // stop transmitting
  
  c = c + 1;
  if (c =='}') c = 'a';
  delay(500);
  */

  
///////////////////////////////////Request Test 1///////////////////////////////////////////
//delay(1000);
//Wire.requestFrom(4, 9);
//
//      long x = Wire.read();    // receive byte as an long integer
//        if (x > 127) 
//        {  //twos complement
//          x = 256 - x;
//          x *= -1;
//        }
//    Serial.println(x);
///////////////////////////////////Request Test 1///////////////////////////////////////////

//  if(Wire.available()){
//
//    Wire.onReceive(receiveEvent);
//  }
//  else {
//    Wire.begin(5);
//  }
  unsigned long timer = millis();
  while( millis() - timer < 500){
    Wire.begin(3);
    
  }
  timer = millis();
  while(millis() - timer < 1500){
    Wire.begin(5);
  }
  timer = millis();

  hl = digitalRead(2); 
  Serial.println(hl);
  if(hl == 1) {
    Serial.println("D2 high");
    turnRight();
  }
  else if(hl == 0){
    Serial.println("D2 low");
    turnLeft();
  }
  else Serial.println("did not work");
}


void receiveEvent(){
  long x = Wire.read();    // receive byte as an integer
  if (x > 127) {  //twos complement
    x = 256 - x;
    x *= -1;
 
  }
  Storefunct(x); // do stuff function
  Serial.println(x);
  delay(1000);
}

void Storefunct(long a){
 Serial.println(a);
  if (a > 3) {
    Serial.println("over 3");
    PORTD |= B00000100;
  }
  else if (a < 0){
    Serial.println("lower than 3"); 
    PORTD &= B11111011;
  }
}


void pause(){
  //ST1.write(stp);
  //ST2.write(stp);
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
