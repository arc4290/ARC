
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //user input
  int len = 3; //row
  int wid = 5; //col
  
  //create map
  int mapp[len][wid];
  
  //define initial position
  int row, col;
  int start = 4; //1=topleft, 2=topright, 3=bottomleft, 4=bottomright
  
  if(start == 1){
    row = 0;
    col = 0;
  }
  else if(start == 2){
    row = 0;
    col = wid-1;
  }
  else if(start == 3){
    row = len-1;
    col = 0;
  }
  else if(start == 4){
    row = len-1;
    col = wid-1;
  }
  
  //initializing
  for(int i = 0; i < len; i++) for(int j = 0; j < wid; j++) mapp[i][j] = 0;

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
  while(1) {
    
    //going left and right(here going straight)
    if(col-1 < 0){//at leftmost, proceed right
      while(col < wid){mapp[row][col] = 1; col = col + 1;}
    }
    else{//at rightmost, proceed left
      while(col > -1){mapp[row][col] = 1; col = col - 1;}
    }
    
    //adjusting width
    if(col == wid) col = col - 1;
    if(col == -1) col = col + 1;

    //check for number of zeroes in map
    //this is ultimate exit case
    int countPath = 0;
    for(int i = 0; i < len; i++){
      for(int j = 0; j < wid; j++) {
        if(mapp[i][j] == 1) countPath++;
      }
    }
    if(countPath == wid*len) break;

    //going up and down(here implement turning)
    if(godown == 1){ row = row + 1; mapp[row][col] = 1;}
    else{ row = row - 1; mapp[row][col] = 1;}

    //diaplay map
    //Serial.print(row);
    //Serial.println();
    //Serial.print(col);
    //Serial.println();
    for(int i = 0; i < len; i++){
      for(int j = 0; j < wid; j++) {
        Serial.print(mapp[i][j]);
        Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();
    //Serial.println();
    
  }//while(1)
}//void setup

void loop() {}

/*how to write function
int displaymap(int len, int wid){
    int a=len+wid;
    Serial.println(a);
    return a;

    //in setup:
    int a =0;
    a = displaymap(len,wid);
    Serial.println(a);
}*/

/*
int calculateCheckPoint(int currentPosition){
  //calculate next checkpoint
}*/
/*
int calculateDistance(int currentPosition){
  //currentPosition == 2409812;
  checkPoint = calculateCheckPoint(currentPosition);
  //calculate the distance to next checkpoint
  return somedistance;
}*/

/*
int calculateDeg(int currentPosition){
  //currentPosition == 2409812;
  checkPoint = calculateCheckPoint(currentPosition);
  //calculate the degree need to turn to next checkpoint
  return somedegree;
}*/

/*
void goStraight(int distranceTraveled, int degTurned, int currentPosition){
  distanceToNext = calculateDistance(currentPosition);
  while(distanceTraveled != distanceToNext){
    //motor 1 and motor 2 go straight
    //if(degTurned != 0){
      //motor 1 and motor 2 coordinates to turn a certain deg back
      //turnAround();
    }
  }
}*/

/*
void turnAround(int degTurned, int currentPosition){
  degNeedTurn = calculateDeg(currentPosition);
  while(degTurned != degNeedTurn){
    //motor 1 and motor 2 opposite spin to turn
  }
}*/

/*
int obstacleDet(int ultrasonicReading){
  if(ultrasonicReading == yes){
    obstacleAvoid();
  }
  return 1; //obstacle = 1; make next element 2 after passing by or before passing
}*/

/*
void obstacleAvoid(){
  int obstacle = 1;
  while(obstacle == 1){
    //motor turn 1-3 degree
    //motor go forward a little bit
    //motor face back to original direction
    obstacle = obstacleDet();
  }
}*/

/*
 * Here are a list of values needed from corresponding sensor:
 * ultrasonic = ultrasonicReading1, ultrasonicReading2
 * expecting either a number from a range of distance
 * -----im not sure how to use the above data
 * or expecting a 1 if obstacle detected else 0
 * 
 * gyroscope = degTurned
 * expecting degree that the gyroscope measured it has turned
 * 
 * accelerometer = distranceTraveled
 * expecting distance traveled by ARC
 * 
 * GPS = currentPosition
 * expecting some coordinate, or proceed data into degNeedTurn and distanceToNext
 */
