#include <Adafruit_VL53L0X.h>
#include <Servo.h>

//globals 
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Servo servoL; //left servo
Servo servoR; //right servo
double hz[8] = {110.0, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998, 207.652};

const int buzzer = 12; //buzzer to arduino pin 9
const int PL = 4;
const int STOP = 5;
const int PLRECORD = 6;
const int RECORD = 7;
const int APPENDREC = 8;
const int IDLEPOWERRESET = 2;
const int LEDRED = 3;
const int LEDGREEN = 9;

int playLive = 0;
int stopB = 0;
int playR = 0;
int record = 0;
int appendR = 0;
int reset = 0;
int posR = 0;
int posL = 0;

int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button
int startPressed = 0;    // the moment the button was pressed
int endPressed = 0;      // the moment the button was released
int holdTime = 0;        // how long the button was hold
int idleTime = 0;        // how long the button was idle

bool stop_button = false;
bool play_Live = false;
bool play_Record = false;
bool recordButton = false;
bool appendRec = false;
bool resetButton = false;

int notes[40];

void setup() {
  Serial.begin(9600);
  
  //wait until serial port opens for native usb devices
  while (!Serial){
    delay(1);
  }

  if (!lox.begin()){
    Serial.println(F("Failed to boot sensor"));
    while(1);
  }
  pinMode(PL, INPUT);
  pinMode(STOP, INPUT);
  pinMode(PLRECORD, INPUT);
  pinMode(RECORD, INPUT);
  pinMode(APPENDREC, INPUT);
  pinMode(IDLEPOWERRESET, INPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  for (int i = 0; i < 40; i++){
    notes[i] = 9;
  }

  servoR.attach(10); //right servo pin
  servoL.attach(11); //left servo pin
}

//move servos
  //if note even move left
  //if note odd move right
  //if note < 4 move up
  //if note >3 move down
void moveServo(int note){

  bool isRight = true; bool isUp = false;
  if (note % 2 == 0){
    isRight = false;
  }
  if (note < 4){
    isUp = true;
  }
  int origPos = 0; //where the servo arm starts
  if (isUp){ //moveup
    if (isRight){
      origPos = posR; //set starting pos
      for (posR; posR <= origPos + 10; posR += 1) { // goes from origPos degrees to origPos + 10 degrees
        servoR.write(posR);              
        delay(15);                       
      }
    }
    else{ //isleft arm
      origPos = posL; //set starting pos
      for (posL; posL <= origPos + 10; posL += 1) { // goes from origPos degrees to origPos + 10 degrees
        servoL.write(posL);              
        delay(15);                       
      }
    }
  }
  else{//movedown
    if (isRight){
      origPos = posR; //set starting pos
      for (posR; posR >= origPos - 10; posR -= 1) { // goes from origPos degrees to origPos - 10 degrees
        servoR.write(posR);              
        delay(15);                       
      }
    }
    else{ //isleft arm
      origPos = posL; //set starting pos
      for (posL; posL >= origPos - 10; posL -= 1) { // goes from origPos degrees to origPos - 10 degrees
        servoL.write(posL);              
        delay(15);                       
      }
    }
  }
}

int counter = 0;
void loop() {
    if (playLive == HIGH){
      play_Live = true;
      play_Record = false;
      stop_button = false;
    }
    if (playR == HIGH){
      play_Record = true;
      play_Live = false;
      stop_button = false;
    }

  
    
    if (record == HIGH){
      recordButton = true;
      stop_button = false;
      
    }
    if (appendR == HIGH){
      appendRec = true;
      recordButton = true;
      stop_button = false;
    }

    
    if (reset != lastButtonState) { // button state changed
       // the button has been just pressed
      if (reset == HIGH) {
          startPressed = millis();
      // the button has been just released
      } else {
          endPressed = millis();
          holdTime = endPressed - startPressed;
          if (holdTime >= 3000) {
              for (int i = 0; i < 40; i++){
                notes[i] = 9;
              } 
          }
          else{
            digitalWrite(LEDRED, LOW);
          }
      }
      posR = 0;
      posL = 0;
      stop_button = true;
    }

    lastButtonState = reset;        // save state for next loop
    /*
    if(reset == HIGH){
      startPressed = millis();
      resetButton = true;
      for (int i = 0; i < 40; i++){
        notes[i] = 9;
      }
    }*/
  // put your main code here, to run repeatedly:
    VL53L0X_RangingMeasurementData_t measure;

    lox.rangingTest(&measure, false);
    playLive = digitalRead(PL);
    stopB = digitalRead(STOP);
    playR = digitalRead(PLRECORD);
    record = digitalRead(RECORD);
    appendR = digitalRead(APPENDREC);
    reset = digitalRead(IDLEPOWERRESET);
    Serial.println("play_Live");
    Serial.println(play_Live);
    Serial.println("stop");
    Serial.println(stop_button);
    Serial.println("play_record");
    Serial.println(play_Record);
    Serial.println("record");
    Serial.println(recordButton);
    Serial.println("append record");
    Serial.println(appendRec);
        Serial.println("");
  /*
  if(playLive && appendRec && !stop_button){
    //find if there is space in notes array
    Serial.println("appendRec");
    int i = 0;
    while(i < 40 && notes[i] != 9){
      i++;
      
      Serial.println(notes[i]);
    }

    //move iterator to end of notes array
    for (i; i < 40; i++){
      int currentNote = 0;
      //if (measure.RangeStatus !=4){
      double measurementMM = measure.RangeMilliMeter;
      if (measurementMM < 50){
          currentNote = 0;
      }
      else if (measurementMM < 100){
        currentNote = 1;
      }
      else if (measurementMM < 150){
          currentNote = 2;
      }
      else if (measurementMM < 200){
          currentNote = 3;
      }
      else if (measurementMM < 250){
          currentNote = 4;
      }
      else if (measurementMM < 300){
          currentNote = 5;
      }
      else if(measurementMM < 350){
          currentNote = 6;
      }
      else {
          currentNote = 7;
      }
      
      notes[i] = currentNote;
      Serial.println(currentNote);
      
     // tone(buzzer, hz[currentNote]);
      delay(500);        // ...for 1 sec
      //noTone(buzzer);     // Stop sound...
      stopB = digitalRead(STOP);
      if (stopB == HIGH){
        stop_button = true;
        play_Live = false;
        play_Record = false;
        recordButton = false;
        appendRec = false;
        break;
      }
      
    }
    
  }*/
  if (play_Live && !stop_button){
      int currentNote = 0;

      if (appendRec == true){
        int i = 0;
        while(i < 40 && notes[i] != 9){
          i++;
          Serial.println(notes[i]);
        }
        counter = i;
      }
      //if (measure.RangeStatus !=4){
      double measurementMM = measure.RangeMilliMeter;
      if (measurementMM < 50){
          currentNote = 0;
      }
      else if (measurementMM < 100){
        currentNote = 1;
      }
      else if (measurementMM < 150){
          currentNote = 2;
      }
      else if (measurementMM < 200){
          currentNote = 3;
      }
      else if (measurementMM < 250){
          currentNote = 4;
      }
      else if (measurementMM < 300){
          currentNote = 5;
      }
      else if(measurementMM < 350){
          currentNote = 6;
      }
      else {
          currentNote = 7;
      }
      if (recordButton == true && counter < 40){
        notes[counter] = currentNote;
        counter++;
      }
      tone(buzzer, hz[currentNote]);
      moveServo(currentNote);
      delay(500);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      stopB = digitalRead(STOP);
      if (stopB == HIGH){
        stop_button = true;
        play_Live = false;
        play_Record = false;
        recordButton = false;
        appendRec = false;
      }
      
   // }
     
  }
  
  else if (play_Record && !stop_button) {
    counter = 0;
    // put your main code here, to run repeatedly:
    while (notes[counter] != 9 && counter < 40){
      int currentNote = notes[counter];
      tone(buzzer, hz[currentNote]);
      moveServo(currentNote);
      delay(500);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      delay(500);
//      Serial.println(notes[i]);
      counter++;
    
      stopB = digitalRead(STOP);
      if (stopB == HIGH){
        stop_button = true;
        play_Live = false;
        play_Record = false;
        recordButton = false;
        appendRec = false;
        break;
      }
    }
  }
  
  //Serial.println("play_record");
    //Serial.println(play_Record);
  delay(500); 
  if (stopB == HIGH){
      stop_button = true;
      play_Live = false;
      play_Record = false;
      recordButton = false;
      appendRec = false;
  }
  //Serial.println("play_record");
//    Serial.println(play_Record);
}
