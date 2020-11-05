#include <Adafruit_VL53L0X.h>


//globals 
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int notes[40];
double hz[8] = {110.0, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998, 207.652};
const int buzzer = 12; //buzzer to arduino pin 12

void setup() {
  Serial.begin(9600);
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
}

int counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (counter < 40){
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
    Serial.println(notes[counter]);
    counter++;
  }
  delay(500);
}
