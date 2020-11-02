#include <Adafruit_VL53L0X.h>


//globals 
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int notes[40];
double hz[8] = {110.0, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998, 207.652};
const int buzzer = 9; //buzzer to arduino pin 9

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
}

void loop() {
  // put your main code here, to run repeatedly:
    VL53L0X_RangingMeasurementData_t measure;
    for ( int i = 0; i < 40; i++){
      int currentNote = notes[i]-1;
      tone(buzzer, hz[currentNote]);
      delay(500);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      Serial.println(notes[i]);
  }
  delay(500);
}
