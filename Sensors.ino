//------------------------------------//
//     Declartions for Accel/Gyro     //
//------------------------------------//
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
MPU6050 mpu;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
#define OUTPUT_READABLE_QUATERNION
#define OUTPUT_READABLE_WORLDACCEL
#define OUTPUT_READABLE_REALACCEL

//------------------------------------//
//       Declartions for GPS          //
//------------------------------------//
#include <NMEAGPS.h>
NMEAGPS gps;
#define gpsPort Serial1
static gps_fix  fix;

//------------------------------------//
//      Declartions for Servos        //
//------------------------------------//
#include <Servo.h>
Servo ST1, ST2; //ST1 is right, ST2 is left
int spd = 150;  //same speed for all operation
int stp = 90;   //stop at 90
int allDelay = 500; //same delay for all
int turnDelay = 4000; //since 2000 is 90 deg for 150, 4000 is 180


// Define Trig and Echo pin:
#define trigPin 42
#define echoPin 40
#define trigPin2 32
#define echoPin2 30
// Define variables:
long duration;
int distance;
long durationtwo;
int distance2;



//------------------------------------//
//            Constants               //
//------------------------------------//
long x = 0;
double z = 0;
unsigned long timer = millis();


long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;


void setup() {
  //Attaching motors to pins
  ST1.attach(5, 1000, 2000); //5
  ST2.attach(6, 1000, 2000); //6  
  Wire.begin();
  TWBR = 24;
  //Intializing DMP and settiong Accel/Gyro offsets
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setXAccelOffset(-5481); 
  mpu.setYAccelOffset(-1844); 
  mpu.setZAccelOffset(1073);
  mpu.setXGyroOffset(-133);
  mpu.setYGyroOffset(63);
  mpu.setZGyroOffset(-47);
  mpu.setDMPEnabled(true);
  packetSize = mpu.dmpGetFIFOPacketSize();
  fifoCount = mpu.getFIFOCount();
  Serial.begin(9600);

  
  // Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
 
}



void loop() {
  
  if (millis() - timer == 500){
      timer = millis();
      getMpu();
      getUltra(); 
      forward();   
      
//*****************************from old version with two borads******************************//
//      Wire.beginTransmission(3);
//      newPos = posistion;
//      Serial.println(newPos);
//      if (newPos - previousPos > 1){
//        previousPos = newPos;
//        Wire.write("!");
//        Wire.write("1");
//        Wire.endTransmission();  
//      }
//*****************************from old version with two borads******************************//

      z = abs(q.z);
      int a = z*100 + 1;
      //Serial.println(a);
      if (a >= 99){
        Serial.println("stop");
        pause();

      }
      if (distance <= 36 || distance2 <= 36){
        
        Serial.println("obsticle detected");
        pause();
        
//        backward();
//        pause();
//        turnLeft();
//        pause();
      }

  }

//  if(millis() - timer == 7000) {
//      timer = millis();
//      GPSloop();
//  }
//    

}

//------------------------------------//
//       Main Accel/Gyro Function     //
//------------------------------------//
void getMpu(){
  mpu.resetFIFO();
  while (fifoCount < packetSize){
    fifoCount = mpu.getFIFOCount();
  }
  if (fifoCount == 1024){
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));
  }
    else{
      if (fifoCount % packetSize != 0){
        mpu.resetFIFO();
      }
      else{
        while (fifoCount >= packetSize){
          mpu.getFIFOBytes(fifoBuffer,packetSize);
          fifoCount -= packetSize;
        }     
      #ifdef OUTPUT_READABLE_QUATERNION
      // display quaternion values in easy matrix form: w x y z
      mpu.dmpGetQuaternion(&q, fifoBuffer);
//      Serial.print("quat\t");
//      Serial.print(q.w);
//      Serial.print("\t");
//      Serial.print(q.x);
//      Serial.print("\t");
//      Serial.print(q.y);
//      Serial.print("\t");
      //Serial.println(q.z);
      mpu.resetFIFO();
      #endif


      Wire.beginTransmission(0b1101000); 
      Wire.write(0x3B); 
      Wire.endTransmission();
      Wire.requestFrom(0b1101000,6); 
      while(Wire.available() < 6);
      accelX = Wire.read()<<8|Wire.read(); 
      accelY = Wire.read()<<8|Wire.read(); 
      accelZ = Wire.read()<<8|Wire.read(); 
      processAccelData();
      //Outputs accelration without gravity (ax, ay, az)
//      #ifdef OUTPUT_READABLE_WORLDACCEL
//       //display initial world-frame acceleration, adjusted to remove gravity and rotated based on known orientation from quaternion
//      mpu.dmpGetQuaternion(&q, fifoBuffer);
//      mpu.dmpGetAccel(&aa, fifoBuffer);
//      mpu.dmpGetGravity(&gravity, &q);
//      mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
//      mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
//      Serial.print("aworld\t");
//      Serial.print(aaWorld.x);
//      Serial.print("\t");
//      Serial.print(aaWorld.y);
//      Serial.print("\t");
//      Serial.println(aaWorld.z);
//      mpu.resetFIFO();
//      #endif
//      #ifdef OUTPUT_READABLE_REALACCEL
//      // display real acceleration, adjusted to remove gravity
//      mpu.dmpGetQuaternion(&q, fifoBuffer);
//      mpu.dmpGetAccel(&aa, fifoBuffer);
//      mpu.dmpGetGravity(&gravity, &q);
//      mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
//      Serial.print("areal\t");
//      Serial.print(aaReal.x);
//      Serial.print("\t");
//      Serial.print(aaReal.y);
//      Serial.print("\t");
//      Serial.println(aaReal.z);
//      #endif     
      }
    }
}


//------------------------------------//
//     Main GPS Parsing Function      //
//------------------------------------//
static void GPSloop(){
  while (gps.available( gpsPort )){
    fix = gps.read();
    Serial.print( F("Location: ") );
    if (fix.valid.location){
      Serial.print( fix.latitude(), 6 ); // latitude with 6 decimals
      Serial.print( ',' );
      Serial.print( fix.longitude(), 6 );
    }
  }
}



void processAccelData() {
  gForceX = accelX/16384.0;
  gForceY = accelY/16384.0; 
  gForceZ = accelZ/16384.0;
  Serial.print("raw acc\t");
  Serial.print(gForceX);
  Serial.print("\t");
  Serial.print(gForceY);
  Serial.print("\t");
  Serial.println(gForceZ);
  Serial.print("\n");
}



void getUltra(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(30);
  digitalWrite(trigPin, LOW);
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  distance = duration*0.034/2;
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(30);
  digitalWrite(trigPin2, LOW);
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  durationtwo = pulseIn(echoPin2, HIGH);
  // Calculate the distance:
  distance2 = durationtwo*0.034/2;
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.print(" cm   ");
  Serial.print("Distance2 = ");
  Serial.print(distance2);
  Serial.println(" cm");
}



//------------------------------------//
//       Motor control functions      //
//------------------------------------//
void pause(){
  ST1.write(stp);
  ST2.write(stp);
}

void forward(){
  ST1.write(spd);
  ST2.write(spd);
}

void backward(){
  int spd2 = 180-spd;  //equavilent backward speed
  ST1.write(spd2);
  ST2.write(spd2);
}

void turnRight(){
  ST1.write(spd);
  ST2.write(stp);
}

void turnLeft(){
  ST1.write(stp);
  ST2.write(spd);
}
