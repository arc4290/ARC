char a[16]; //array of characters used as my buffer from serial
char b[16];
char c = 0; //first bytes read from the serial
char t = 0;
int stuff = 20; //maximum size of the buffer
char stuff2 = ';'; //special end character for readBytesUntil()
int out = 0; //used to exit while loop for parsing
int count = 0; //counter for while loop for parsing. also used as the index for string_grab in the loop
char string_grab[32] = ""; //array of characters that are filled from buffer (a)
long int w = 0; //width
long int l = 0; //length
long int p = 0; //position
long int product = 0; //area from dimensions = length*width
String string = "";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //setting up the serial
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) //executes when receiving data
  {
    c = Serial.read(); //reads the first bytes
    if(c == '.'){ //if the first byte is '.', then it knows that it is the dimension data
      int size = Serial.readBytesUntil(stuff2, a, stuff); //reads bytes from serial and puts it into buffer (a). Also gets the amount of bytes from serial (size)
      //Serial.print("Size of recieved characters: ");
      //Serial.println(size); //prints number of bytes sent for validation
      for(int i = 0; i<size; i++){ //for loop used to get ride of anything not a number or a '.'
        //needed this loop because sometimes the disconnect message would get into the buffer (a).
        switch(a[i]){
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
      Serial.print(string_grab); //prints out what was sent from the phone app
      Serial.readBytesUntil(stuff2, b, stuff);
      if(b[0] == 'g'){
        Serial.println();
        Serial.println(b);
        Serial.println("Good to go");
      }
      else{
        Serial.println();
        Serial.println(b);
        Serial.println("Transmission unsuccessful");
      }
      //Serial.print('s');


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
        //Serial.print("Area of land = ");
        //Serial.print(product);
        //Serial.println("ft squared"); //prints area to the serial
        
        if(product > 500){ //if the dimensions exceed 500 ft^2 then error message will display
          //Serial.println("Dimensions are too big.");
          //Serial.println("Please input smaller values.");
          out = 1;
        }
        else{ //if dimensions are fine, then prints out the Dimensions
        /*Serial.print("Width: ");
        Serial.print(w);
        Serial.print("\n");
        Serial.print("Length: ");
        Serial.print(l);
        Serial.print("\n");
        Serial.print("Position: ");
        Serial.print(p);*/
        out = 1;
        }
      }
      //resets all variables in the loop
      w = 0;
      l = 0;
      p = 0;
      out = 0;
      count = 0;
    }
    if(c == 'f') {//if first byte received is 'f', then motors go forward
      Serial.println("forward");
    }
    if(c == 'b') {//if first byte received is 'b', then motors go backwards
      Serial.println("backward");
    }
    if(c == 'r') {//if first byte receieved is 'r', then motors turn right
      Serial.println("right");
    }
    if(c == 'l') {//if first bytes received is 'l', then motors turn left
      Serial.println("left");
    }
    if(c == 's') {//if first byte received is 's', then motors stop
      Serial.println("stop");
    }
    /*if(c == 't'){
      Serial.println("transmission not successful");
    }
    if(c == 'g'){
      Serial.println();
      Serial.println("Good to go");
    }
    if(c == 'd'){
      Serial.println("bleh");
    }*/
      
  }   
  }
  
