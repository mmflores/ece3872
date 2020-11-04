#include <Adafruit_VL53L0X.h>


//globals 
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int notes[40];
double hz[8] = {110.0, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998, 207.652};
const int buzzer = 9; //buzzer to arduino pin 9
bool stop_button = true;
bool play_Live = true;

const int NUM_BUTTONS = 6;
enum button_enum {
  playButton,
  appendRecButton,
  eraseRecButton,
  stopButton,
  playLiveButton,
  playRecButton
};

const int pins[NUM_BUTTONS] = {
  2,           // playButton
  3,           // appendButton
  4,           // eraseButton
  5,
  6,
  7
};

bool pushed[NUM_BUTTONS];

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 40; i++){
    notes[i] = 0;
  }

  //wait until serial port opens for native usb devices
  while (!Serial){
    delay(1);
  }

  if (!lox.begin()){
    Serial.println(F("Failed to boot sensor"));
    while(1);
  }

  //init buttons
  for (int i = 0; i < NUM_BUTTONS; i++){
      pinMode (pins[i], INPUT_PULLUP);
      pushed[i] = false;
    }
    
}

//erase recording
void eraseRec(){
  for (int i = 0; i < 40; i++){
      notes[i] = 0;
  }
}

//append recording if space in array
void appendRec(){
  int counter = 0;
  //find end of last recording
  while (notes[counter] != 0){
    counter++;
  }
  
  // put your main code here, to run repeatedly:
  if (counter < 40 && stop_button == false){
    VL53L0X_RangingMeasurementData_t measure;

    lox.rangingTest(&measure, false);

    if (measure.RangeStatus !=4){
      double measurementMM = measure.RangeMilliMeter;
      
      if (measurementMM < 50){
        notes[counter] = 1;
      }
      else if (measurementMM < 100){
        notes[counter] = 2; 
      }
      else if (measurementMM < 150){
        notes[counter] = 3;
      }
      else if (measurementMM < 200){
        notes[counter] = 4;
      }
      else if (measurementMM < 250){
        notes[counter] = 5;
      }
      else if (measurementMM < 300){
        notes[counter] = 6;
      }
      else if(measurementMM < 350){
        notes[counter] = 7;
      }
      else {
        notes[counter] = 8;
      }
    }
    int currentNote = notes[counter]-1;
    tone(buzzer, hz[currentNote]);
    delay(500);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    counter++;
  }
}

//play previously recorded notes array
void playRec(){
    for ( int i = 0; i < 40; i++){
      if (stop_button == false){
        int currentNote = notes[i]-1;
        tone(buzzer, hz[currentNote]);
        delay(500);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        Serial.println(notes[i]);
      }
  }
}

//records notes and plays live
void rec(){
  int counter = 0;
  while (counter < 40 && stop_button == false){
    VL53L0X_RangingMeasurementData_t measure;

    lox.rangingTest(&measure, false);

    if (measure.RangeStatus !=4){
      double measurementMM = measure.RangeMilliMeter;
      
      if (measurementMM < 50){
        notes[counter] = 1;
      }
      else if (measurementMM < 100){
        notes[counter] = 2; 
      }
      else if (measurementMM < 150){
        notes[counter] = 3;
      }
      else if (measurementMM < 200){
        notes[counter] = 4;
      }
      else if (measurementMM < 250){
        notes[counter] = 5;
      }
      else if (measurementMM < 300){
        notes[counter] = 6;
      }
      else if(measurementMM < 350){
        notes[counter] = 7;
      }
      else {
        notes[counter] = 8;
      }
    }
    int currentNote = notes[counter]-1;
    tone(buzzer, hz[currentNote]);
    delay(500);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    counter++;
  }
}

//plays live
void playLive(){
   int currentNote = 0;
   VL53L0X_RangingMeasurementData_t measure;
   while(stop_button = false){
     lox.rangingTest(&measure, false);
     if (measure.RangeStatus !=4){
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
      }
      tone(buzzer, hz[currentNote]);
      delay(500);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
   }
}

int counter = 0;
void loop() {
     for (int i = 0; i < NUM_BUTTONS; i++){
        bool previous = pushed[i];
        pushed[i] = digitalRead(pins[i]) == LOW;
  
        if (pushed[i] != previous){
            if (pushed[i]){                                            
                switch (i){
                  case stopButton:
                    stop_button = true;
                    break;
                  case playButton:
                    // do action for blue button being set
                    if (play_Live){
                      playLive();
                    }
                    else{
                      rec();
                    }
                    break;
                  case appendRecButton:
                    stop_button = true;
                    appendRec();
                    break;
                  case eraseRecButton:
                    eraseRec();
                    break;
                  case playLiveButton:
                    play_Live = !play_Live;
                    break;
                  case playRecButton:
                    play_Live = false;
                    break;
              }
            }
            else{ // button just release
                switch (i){
                  if (pushed[i]){                                            
                    switch (i){
                      case stopButton:
                        stop_button = true;
                        break;
                      case playButton:
                        stop_button = false;
                        if(play_Live){
                          playLive();
                        }
                        else{
                          rec();
                        }
                        break;
                      case appendRecButton:
                        stop_button = false;
                        appendRec();
                        break;
      
                      case eraseRecButton:
                        eraseRec();
                        break;
                      case playLiveButton:
                        play_Live = !play_Live;
                        break;
                      case playRecButton:
                        stop_button = false;
                        playRec();
                        break;
                    }
                 }
              }
        }
     }
    }
    delay(500);
}
