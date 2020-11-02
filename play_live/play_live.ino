#include <Adafruit_VL53L0X.h>

//globals 
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
double hz[8] = {110.0, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998, 207.652};
const int buzzer = 9; //buzzer to arduino pin 9

void setup() {
  Serial.begin(115200);
  
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

    lox.rangingTest(&measure, false);

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
    }
  delay(500);
}
