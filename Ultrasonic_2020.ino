
// Define Trig and Echo pin:
#define trigPin 13
#define echoPin 12
#define trigPin2 10
#define echoPin2 9
// Define variables:
long duration;
int distance;
long durationtwo;
int distance2;


void setup() {
  // put your setup code here, to run once:
  // Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

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
