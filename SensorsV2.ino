
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
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

float currVelocity = 0;
float prevVelocity = 0;
float posistion = 0;
float currAccel;
float prevAccel = 0;
unsigned long prevTime = 0;
unsigned long currTime;


//Gps Stuff
#include <NMEAGPS.h>
#include <GPSport.h>

#include <Streamers.h>
NMEAGPS gps;
#define gpsPort Serial1


static gps_fix  fix;


#define OUTPUT_READABLE_QUATERNION
#define OUTPUT_READABLE_WORLDACCEL

long x = 0;
double z = 0;
unsigned long sampletimer = millis();
unsigned long timer = millis();
int IMUarray[100];
long newPos = 0;
long previousPos = 0;


void setup() {

    Wire.begin();
    Wire.beginTransmission(0b1101000);
    Wire.write(0x1B);
    Wire.write(0b00000010);
    Wire.endTransmission();
    TWBR = 24;
    mpu.initialize();
    mpu.dmpInitialize();
    mpu.setXAccelOffset(-5481); 
    mpu.setYAccelOffset(-1844); 
    mpu.setZAccelOffset(1073);
    mpu.setXGyroOffset(-133);
    mpu.setYGyroOffset(63);
    mpu.setZGyroOffset(-47);
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
    fifoCount = mpu.getFIFOCount();

    Serial.begin(9600);
    gpsPort.begin(9600);



}

void loop() {
  if (millis() - timer == 200){
      timer = millis();
      getMpu();
      Wire.beginTransmission(3);
//      newPos = posistion;
//      Serial.println(newPos);
//      if (newPos - previousPos > 1){
//        previousPos = newPos;
//        Wire.write("!");
//        Wire.write("1");
//        Wire.endTransmission();  
//      }
      z = abs(q.z);
      int a = z*100 + 1;
      Serial.println(a);
      if (a == 100){
        Wire.write(97);
        Wire.write(1);
        Wire.endTransmission();
      }
      else {
        Wire.write(0);
        Wire.endTransmission();
      }
  }
     
//GPSloop();

}

void getMpu(){
//mpu.resetFIFO();
while (fifoCount < packetSize) {     
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
  
          
//          Wire.beginTransmission(0b1101000); 
//          Wire.write(0x3B); 
//          Wire.endTransmission();
//          Wire.requestFrom(0b1101000,6); 
//          while(Wire.available() < 6);
//          accelX = Wire.read()<<8|Wire.read(); 
//          accelY = Wire.read()<<8|Wire.read(); 
//          accelZ = Wire.read()<<8|Wire.read(); 
//          processAccelData();
          

//          
//          //Outputs accelration without gravity (ax, ay, az)
//          #ifdef OUTPUT_READABLE_WORLDACCEL
//              // display initial world-frame acceleration, adjusted to remove gravity
//              // and rotated based on known orientation from quaternion
//             
//              mpu.dmpGetQuaternion(&q, fifoBuffer);
//              mpu.dmpGetAccel(&aa, fifoBuffer);
//              mpu.dmpGetGravity(&gravity, &q);
//              mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
//              mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
//              Serial.print("aworld\t");
//              Serial.print(aaWorld.x);
//              Serial.print("\t");
//              Serial.print(aaWorld.y);
//              Serial.print("\t");
//              Serial.println(aaWorld.z);
//              mpu.resetFIFO();
//          #endif 

        }
      }
}


void getIMUSample(){
  for (byte i = 0; i < 100; i = i + 1) {
    getMpu();
    IMUarray[i] = aaWorld.x;
    Serial.println(IMUarray[i]);
  }
}


void resetIMUSample(){
  for (byte i = 0; i < 100; i = i + 1) {
    IMUarray[i] = 0;
  }
}



void processAccelData() {
  gForceX = accelX/16384.0;
  gForceY = accelY/16384.0; 
  gForceZ = accelZ/16384.0;



//          Serial.print("XAcceleration=\t");
//          Serial.print(gForceX);
//          Serial.print("\t");
         
//          Serial.print(" Y=");
//          Serial.print(gForceY);
//          Serial.print(" Z=");
//          Serial.println(gForceZ);
          
//            currTime = millis();
//
//            currAccel = gForceX * 9.80665;
//            //velocity integration = p
//            currVelocity = (currAccel + prevAccel)/2*(currTime - prevTime)/1000; //1000 to convert to sec
//            //prevAccel = currAccel;
//            
//            //position integration
//            posistion += (currVelocity + prevVelocity)/2*(currTime - prevTime)/1000;
//            prevVelocity = currVelocity;
//            
//            prevTime = currTime;
          
//            Serial.print("Velocity\t");
//            Serial.print(currVelocity);
//            Serial.print("\t");
//         
//            Serial.print("Position\t");
//            Serial.print(posistion);
//            Serial.print("\n");
}



//------------------------------------
//  This is the main GPS parsing loop.

void GPSloop()
{
  while (gps.available(gpsPort))
  {
    
    fix = gps.read();

    if (fix.valid.location)
    {
      Serial.println();

      Serial.print("Latitude= "); 
      Serial.print(fix.latitude(), 8);
      Serial.print(" Longitude= "); 
      Serial.println(fix.longitude(), 8);
    
    }
    
  }

} // GPSloop
