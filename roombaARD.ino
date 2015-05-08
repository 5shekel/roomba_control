//motor A connected between A01 and A02
//motor B connected between B01 and B02

int STBY = 10; //standby

//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction

//Motor B
int PWMB = 5; //Speed control
int BIN1 = 11; //Direction
int BIN2 = 12; //Direction

int maxData = 0;
boolean started, ended;
int index;
int inData[3];
boolean debug=1;

void setup(){
  Serial.begin(57600);

  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}
void loop() {
  while(Serial.available() > 0)
  {
    char inChar = Serial.read();
    if(inChar == '[') // check for the opening character
    {
      started = true;
      index = 0;
      inData[index] = '\0';
    }
    else if(inChar == ']') // check for the end character
    {
      ended = true;
      break;
    }
    else
    {
      if(started && index < 3) // process all the bits in between
      {
        inData[index++] = inChar;
        inData[index] = '\0';
      }
    }
  }

  if(started && ended)
  {
    if (inData[0] == 'R'){ // process all the data
        if(debug) Serial.println("RIGHT");
		move(1,inData[1],inData[2]);
    } 
    else if(inData[0] == 'L'){
          if(debug) Serial.println("LEFT");
		  move(2,inData[1],inData[2]);
    }
	    else if(inData[0] == 'S'){
          if(debug) Serial.println("STOP");
		  stop();
    }
    if(debug){
      for(int i=1; i<3; i++) { // print the data bits (excluding the '[' and ']'
        Serial.println(inData[i]);
      }
    }

    // Reset for next pass
    inData[0] = '\0';
    index = 0;
    ended = false;
    started = false;
  } 
}
/*
void loop()
{
  while(Serial.available() > 0) {
    maxData = Serial.read();
    Serial.println(maxData);
  }

  if(maxData == 0) stop(); // stop
  if(maxData == 1) { 
    move(1, 255, 1);
    delay(1000);
  } // turn motor 1
  if(maxData == 2) move(2, 255, 1); // turn motor 1
}
*/
/*
void loop(){
 move(1, 255, 1); //motor 1, full speed, left
 move(2, 255, 1); //motor 2, full speed, left
 
 delay(1000); //go for 1 second
 stop(); //stop
 delay(250); //hold for 250ms until move again
 
 move(1, 128, 0); //motor 1, half speed, right
 move(2, 128, 0); //motor 2, half speed, right
 
 delay(1000);
 stop();
 delay(250);
 }
 */
void move(int motor, int speed, char direction){
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }
  else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop(){
  //enable standby  
  digitalWrite(STBY, LOW);
}


