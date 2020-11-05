#include <Adafruit_VL53L0X.h>

//globals 
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
double hz[8] = {110.0, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998, 207.652};

const int buzzer = 12; //buzzer to arduino pin 9
const int PL = 4;
const int STOP = 5;
const int PLRECORD = 6;
const int RECORD = 7;
const int APPENDREC = 8;

int playLive = 0;
int stopB = 0;
int playR = 0;
int record = 0;
int appendR = 0;

bool stop_button = false;
bool play_Live = false;
bool play_Record = false;
bool recordButton = false;
bool appendRec = false;

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
  for (int i = 0; i < 40; i++){
    notes[i] = 9;
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
  // put your main code here, to run repeatedly:
    VL53L0X_RangingMeasurementData_t measure;

    lox.rangingTest(&measure, false);
    playLive = digitalRead(PL);
    stopB = digitalRead(STOP);
    playR = digitalRead(PLRECORD);
    record = digitalRead(RECORD);
    appendR = digitalRead(APPENDREC);
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
