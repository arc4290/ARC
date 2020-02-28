
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


//Gps Stuff
#include <NMEAGPS.h>
NMEAGPS gps;
#define gpsPort Serial1


static gps_fix  fix;


#define OUTPUT_READABLE_QUATERNION
#define OUTPUT_READABLE_WORLDACCEL

long x = 0;
double z = 0;

unsigned long timer = millis();

void setup() {
  
    
    Wire.begin();
    TWBR = 24;
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

    Serial.begin(115200);



}

void loop() {
  

  
  
  if (millis() - timer == 2000){
      timer = millis();
      getMpu();
      Wire.beginTransmission(3);
      z = q.z;
      Wire.write(z);
      Wire.endTransmission();
      
      }
//  if(millis() - timer == 7000) {
//      timer = millis();
//      GPSloop();
//  }
//    





}

void getMpu(){
mpu.resetFIFO();
while (fifoCount < packetSize) {

        //insert here your code
       
       


        fifoCount = mpu.getFIFOCount();

    }

    if (fifoCount == 1024) {
   
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
       
    }
    else{
   
      if (fifoCount % packetSize != 0) {
       
          mpu.resetFIFO();
           
        }
      else{
   
            while (fifoCount >= packetSize) {
           
                mpu.getFIFOBytes(fifoBuffer,packetSize);
                fifoCount -= packetSize;
               
            }   
              
             #ifdef OUTPUT_READABLE_QUATERNION
              // display quaternion values in easy matrix form: w x y z
              mpu.dmpGetQuaternion(&q, fifoBuffer);
              Serial.print("quat\t");
              Serial.print(q.w);
              Serial.print("\t");
              Serial.print(q.x);
              Serial.print("\t");
              Serial.print(q.y);
              Serial.print("\t");
              Serial.println(q.z);
              mpu.resetFIFO();
          #endif
  
          
         
          //Outputs accelration without gravity (ax, ay, az)
          #ifdef OUTPUT_READABLE_WORLDACCEL
              // display initial world-frame acceleration, adjusted to remove gravity
              // and rotated based on known orientation from quaternion
             
              mpu.dmpGetQuaternion(&q, fifoBuffer);
              mpu.dmpGetAccel(&aa, fifoBuffer);
              mpu.dmpGetGravity(&gravity, &q);
              mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
              mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
              Serial.print("aworld\t");
              Serial.print(aaWorld.x);
              Serial.print("\t");
              Serial.print(aaWorld.y);
              Serial.print("\t");
              Serial.println(aaWorld.z);
              mpu.resetFIFO();
          #endif
           
    }
   
   }

  
}



//static void doSomeWork()
//{
//  // Print all the things!
//
//  trace_all( DEBUG_PORT, gps, fix );
//
//} // doSomeWork



//------------------------------------
//  This is the main GPS parsing loop.

static void GPSloop()
{
  
  while (gps.available( gpsPort )) {
    fix = gps.read();
    Serial.print( F("Location: ") );
    if (fix.valid.location) {
      Serial.print( fix.latitude(), 6 );
      Serial.print( ',' );
      Serial.print( fix.longitude(), 6 );
    }
  }

} // GPSloop
