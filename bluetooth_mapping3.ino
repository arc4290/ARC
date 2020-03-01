#include <Servo.h>

Servo ST1, ST2; //ST1 is right, ST2 is left
int spd = 150;  //same speed for all operation
int stp = 90;   //stop at 90
int allDelay = 500; //same delay for all
int turnDelay = 1000; //since 2000 is 90 deg for 150, 4000 is 180

int mission_check = 0;
char a[16]; //array of characters used as my buffer from serial
char b[16];
int neg_check = 0;
char c = 0; //first byte read from the serial
char d = 0;
int stuff = 20; //maximum size of the buffer
char stuff2 = ';'; //special end character for readBytesUntil()
int out = 0; //used to exit while loop for parsing
int out2 = 0;
int count = 0; //counter for while loop for parsing. also used as the index for string_grab in the loop
char string_grab[32] = ""; //array of characters that are filled from buffer (a)
long int w = 0; //width
long int l = 0; //length
long int p = 0; //position
long int product = 0; //area from dimensions = length*width
long int row;
long int col;
int start;
int turnR = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //setting up the serial

  ST1.attach(5, 1000, 2000); //5
  ST2.attach(6, 1000, 2000); //6
}

void loop() {
  w = 0;
  l = 0;
  p = 0;
  while(out2 == 0 && mission_check == 0){
    look_for_stuff();
  }
  mission_check = 1;
  out2 = 0;
  
if(product <= 500 && product != 0 && product > 0 && neg_check == 0 && mission_check == 1){
  start = p;
  int mapp[l][w];
  
  forward();
  backward();
  pause();
  pause();
  pause();

  if(start == 1){
    row = 0;
    col = 0;
    turnR = 1;
  }
  else if(start == 2){
    row = 0;
    col = w-1;
    turnR = 0;
  }
  else if(start == 3){
    row = l-1;
    col = 0;
    turnR = 0;
  }
  else if(start == 4){
    row = l-1;
    col = w-1;
    turnR = 1;
  }
  //initializing
  for(int i = 0; i < l; i++) for(int j = 0; j < w; j++) mapp[i][j] = 0;

  /*diaplaying
  for(int i = 0; i < len; i++){
    for(int j = 0; j < wid; j++) {
      Serial.print(mapp[i][j]);
      Serial.print(' ');
    }
    Serial.println();
  }*/
  
  //skeleton starts here
  int godown = 0;
  if(row-1 < 0){ //check if ARC is at topmost row
    godown = 1;
  }

  //insert motor functions here-forward();
  while(d != 's') {
    if(Serial.available() > 0){
       d = Serial.read();
    }
    if(d == 's'){
      break;
    }
    //going left and right(here going straight)
    if(col-1 < 0){//at leftmost, proceed right
      while(col < w){
        mapp[row][col] = 1; 
        col = col + 1;
        if(Serial.available() > 0){
        d = Serial.read();
        }
        if(d == 's'){
          break;
        } 
        forward();
        pause();
        }
    }
    else{//at rightmost, proceed left
      while(col > -1){
        mapp[row][col] = 1; 
        col = col - 1;
        if(Serial.available() > 0){
        d = Serial.read();
        }
        if(d == 's'){
          break;
        }
        forward();
        pause();
        }
    }
    
    //adjusting width
    if(col == w) col = col - 1;
    if(col == -1) col = col + 1;

    //check for number of zeroes in map
    //this is ultimate exit case
    int countPath = 0;
    for(int i = 0; i < l; i++){
      for(int j = 0; j < w; j++) {
        if(mapp[i][j] == 1) countPath++;
      }
    }
    if(countPath == w*l){
      mission_check = 0;
      break;
    }

    //going up and down(here implement turning)
    //implementing 180 deg turn going up or down
    if(godown == 1){ 
      row = row + 1; 
      mapp[row][col] = 1;
      if(turnR == 1){
        if(Serial.available() > 0){//reads bytes while mapping is executing
        d = Serial.read();
        }
        if(d == 's'){//if user hits "stop" on app then breaks loop
          break;
        }
        turnRight();
        turnR = 0;
      }
      else{
        if(Serial.available() > 0){//reads bytes while mapping is executing
        d = Serial.read();
        }
        if(d == 's'){ //if user hits "stop" on app then breaks loop
          break;
        }
        turnLeft();
        turnR = 1;
      }
      pause();
      }
    else{ 
      row = row - 1; 
      mapp[row][col] = 1;
      if(turnR == 1){
        if(Serial.available() > 0){ //reads bytes while mapping is executing
        d = Serial.read();
        }
        if(d == 's'){ //if user hits "stop" on app then breaks loop
          break;
        }
        turnRight();
        turnR = 0;
      }
      else{
        if(Serial.available() > 0){ //reads bytes while mapping is executing
        d = Serial.read();
        }
        if(d == 's'){ //if user hits "stop" on app then breaks loop
          break;
        }
        turnLeft();
        turnR = 1;
      }
      pause();
      }

    //diaplay map
    //Serial.print(row);
    //Serial.println();
    //Serial.print(col);
    //Serial.println();
    for(int i = 0; i < l; i++){
      for(int j = 0; j < w; j++) {
        Serial.print(mapp[i][j]);
        Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();
  
  }
  d = 0;
}
else{
  Serial.println("inputs were not valid");
  neg_check = 0; //resets negative check for new inputs
  mission_check = 0;
}
}

void look_for_stuff(){
    c = Serial.read(); //reads the first bytes
    if(c == '.'){ //if the first byte is '.', then it knows that it is the dimension data
      int size = Serial.readBytesUntil(stuff2, a, stuff); //reads bytes from serial and puts it into buffer (a). Also gets the amount of bytes from serial (size)
      //Serial.println();
      //Serial.print("Size of recieved characters: ");
      //Serial.println(size); //prints number of bytes sent for validation
      for(int i = 0; i<size; i++){ //for loop used to get ride of anything not a number or a '.'
        //needed this loop because sometimes the disconnect message would get into the buffer (a).
        switch(a[i]){
          case '-':
          Serial.println("Cannot have negative numbers");
          neg_check = 1; //makes it impossible to execute mapping with negative number
          break;
          case '1':
          string_grab[i] = a[i];
          break;
          case '2':
          string_grab[i] = a[i];
          break;
          case '3':
          string_grab[i] = a[i];
          break;
          case '4':
          string_grab[i] = a[i];
          break;
          case '5':
          string_grab[i] = a[i];
          break;
          case '6':
          string_grab[i] = a[i];
          break;
          case '7':
          string_grab[i] = a[i];
          break;
          case '8':
          string_grab[i] = a[i];
          break; 
          case '9':
          string_grab[i] = a[i];
          break;
          case '0':
          string_grab[i] = a[i];
          break; 
          case '.':
          string_grab[i] = a[i];
          break;
          default:
          break;
      }
      }
      //Serial.println(string_grab); //prints out what was sent from the phone app
      


      //catch gui confirm
      Serial.print(string_grab); //prints out what was sent from the phone app
      Serial.readBytesUntil(stuff2, b, stuff);
      if(b[0] == 'g'){
        Serial.println();
        //Serial.println(b);
        Serial.println("Transmission Successful");
        while(out != 1){ //while loop for parsing the data
          while(string_grab[count] != '.'){ //if the index is not a '.' then convert to integer
            w = w + (string_grab[count] - '0'); //integer conversion
            count = count + 1; //increment counter
            w = w*10; //increase by a decade for next index
           }
          w = w/10; //shift it back
          count = count + 1; //increment counter
          while(string_grab[count] != '.'){ //if the index is not a '.' then convert to integer
            l = l + (string_grab[count] - '0'); //integer conversion
            count = count + 1; //increment counter
            l = l*10; //increase by a decade for next index
          }
          l = l/10; //shift it back
          count = count + 1; //increment counter
          while(string_grab[count] != '.'){ //if the index is not a '.' then convert to integer
            switch(string_grab[count]){ //only expecting a '1', '2', '3', or a '4' so switch case is used
              case '1': //if '1' is read from string_grab
              p = (string_grab[count] - '0'); //integer conversion
              count = count + 1; //increment counter
              break;
              case '2': //if '2' is read from string_grab
              p = (string_grab[count] - '0'); //integer conversion
              count = count + 1; //increment counter
              break;
              case '3': //if '3' is read from string_grab
              p = (string_grab[count] - '0'); //integer conversion
              count = count + 1; //increment counter
              break;
              case '4': //if '4' is read from string_grab
              p = (string_grab[count] - '0'); //integer conversion
              count = count + 1; //increment counter
              break;
              default:  //if expected value is not there
              count = count + 1; //increment counter
              break;
          }
          }
          product = w*l; //calculate the area from the dimensions
        
        if(product > 500){ //if the dimensions exceed 500 ft^2 then error message will display
          Serial.println("Dimensions are too big.");
          Serial.println("Please input smaller values.");
          out = 1;
        }
        else{ //if dimensions are fine, then prints out the Dimensions
        Serial.print("Width: ");
        Serial.print(w);
        Serial.print("\n");
        Serial.print("Length: ");
        Serial.print(l);
        Serial.print("\n");
        Serial.print("Position: ");
        Serial.println(p);
        Serial.print("Area of land = ");
        Serial.print(product);
        Serial.println("ft squared"); //prints area to the serial
        out = 1;
        }
      }
      //resets all variables in the loop
      memset(string_grab, 0, sizeof(string_grab)); //clears the string_grab variable for new inputs
      out = 0;
      count = 0;
      //exit loop calling function
      out2 = 1;
      }   
      else{
        Serial.println();
        Serial.println(b);
        Serial.println("Transmission Unsuccessful"); 
        memset(string_grab, 0, sizeof(string_grab)); //clears the string_grab variable for new inputs
        out2 = 1;
      }
    }
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
  ST1.write(stp);
  ST2.write(spd);
  delay(turnDelay);
  //delay(allDelay);
}

void turnLeft(){
  ST1.write(spd);
  ST2.write(stp);
  delay(turnDelay);
  //delay(allDelay);
}

  
