#include <Adafruit_VL53L0X.h>

//globals 
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
double hz[8] = {110.0, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998, 207.652};

const int buzzer = 12; //buzzer to arduino pin 9
const int PL = 4;
const int STOP = 5;
const int PLRECORD = 6;

int playLive = 0;
int stopB = 0;
int playR = 0;

bool stop_button = false;
bool play_Live = false;
bool play_Record = false;

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
}

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
  // put your main code here, to run repeatedly:
    VL53L0X_RangingMeasurementData_t measure;

    lox.rangingTest(&measure, false);
    playLive = digitalRead(PL);
    stopB = digitalRead(STOP);
    playR = digitalRead(PLRECORD);
    Serial.println("play_Live");
    Serial.println(play_Live);
    Serial.println("stop");
    Serial.println(stop_button);
    Serial.println("play_record");
    Serial.println(play_Record);
        Serial.println("");

  if (play_Live && !stop_button){
    if (measure.RangeStatus !=4){
      double measurementMM = measure.RangeMilliMeter;
      
      if (measurementMM < 50){
          tone(buzzer, hz[0]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      else if (measurementMM < 100){
          tone(buzzer, hz[1]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      else if (measurementMM < 150){
          tone(buzzer, hz[2]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      else if (measurementMM < 200){
          tone(buzzer, hz[3]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      else if (measurementMM < 250){
          tone(buzzer, hz[4]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      else if (measurementMM < 300){
          tone(buzzer, hz[5]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      else if(measurementMM < 350){
          tone(buzzer, hz[6]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      else {
          tone(buzzer, hz[7]); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
      }
      stopB = digitalRead(STOP);
      if (stopB == HIGH){
        stop_button = true;
        play_Live = false;
        play_Record = false;
      }
    }
  }
  else if (play_Record && !stop_button) {
    int notes[40];
    for (int i = 0; i < 40; i++){
      notes[i] = 5;
    }
    // put your main code here, to run repeatedly:
    VL53L0X_RangingMeasurementData_t measure;
    for ( int i = 0; i < 40; i++){
      int currentNote = notes[i]-1;
      tone(buzzer, hz[currentNote]);
      delay(500);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      Serial.println(notes[i]);
      stopB = digitalRead(STOP);
      if (stopB == HIGH){
        stop_button = true;
        play_Live = false;
        play_Record = false;
        break;
      }
    }
  }
  Serial.println("play_record");
    Serial.println(play_Record);
  delay(500); 
  if (stopB == HIGH){
      stop_button = true;
      play_Live = false;
      play_Record = false;
  }
  Serial.println("play_record");
    Serial.println(play_Record);
}
