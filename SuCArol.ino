/*  SuCArol v1.0
 *  Super Compact Alarm ... (?)
 */
#include "include/sucarol.h"

char temp[] = {"="};
Blinker blinker(temp, ledPin);
AlarmHandler alarm_handler(&blinker, sensorPin, buttonPin, ledPin, alarmPin);
StateMachine state_machine(&alarm_handler);

void setup() {
  pinMode(alarm_handler.buttonPin, INPUT);
  pinMode(alarm_handler.sensorPin, INPUT);
  pinMode(alarm_handler.alarmPin, OUTPUT);
  Serial.begin(9600);
  Serial.print("Initialized\n");
  
}



void loop() {
  state_machine.spin();
}




