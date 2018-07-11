#include "include/alarm_handler.h"

AlarmHandler::AlarmHandler(){};

void AlarmHandler::giveAlarm() {
  Serial.print("ALARM!\n");
  alarm = true;
  digitalWrite(alarmPin, HIGH);
}

void AlarmHandler::shutupAlarm() {
  alarm = false;
  digitalWrite(alarmPin, LOW);
}

bool AlarmHandler::listenToSequence() {
  clearSequence();
  Serial.print("Ready to listen to a sequence.\n");

  if (!waitButtonPressFor(5000)){
    Serial.print("Listening timeout.\n");
    return false;
  }
  //t0 = millis();
  int i;
  for(i=0; i<N_SEQ; i+=2) {
    unsigned long pressed = heldButton();
    if (pressed >= 3000) {
      Serial.print("Ended sequence.\n");
      break;
    }
    candSequence[i] = pressed;
    unsigned long unpressed = unpressedButton();
    if (unpressed >= 3000) {
      Serial.print("Failed sequence.\n");
      return false;
    }    
    candSequence[i+1] = unpressed;
  }
  Serial.print("Entered sequence: [ ");
  for(i=0; i<N_SEQ; i++) {
    Serial.print(candSequence[i]);
    Serial.print("ms ");
  }
  Serial.print("]\n");
  return true;
}

unsigned long AlarmHandler::heldButton() {
  unsigned long t0 = 0, t1 = 0;
  t0 = millis();
  while (buttonState()) {
    buttoncounter ++;
    Serial.print("Button pressed: ");
    Serial.print(buttoncounter);
    Serial.println();
  }
  buttoncounter = 0;
  t1 = millis();
  Serial.print("Button pressed for ");
  Serial.print(t1-t0);
  Serial.print("ms\n");
  return t1-t0;
}


void AlarmHandler::waitButtonPress() {
  while (!buttonState()){
    /*Idle*/
  }
}

bool AlarmHandler::waitButtonPressFor(unsigned int timeout) {
  unsigned long t0 = 0, t1 = 0;
  t0 = millis();
  while (!buttonState()){
    t1 = millis();
    if ((t1 - t0) > timeout) {
      return false;
    }
  }
  return true;
}

unsigned long AlarmHandler::unpressedButton() {
  unsigned long t0 = 0, t1 = 0;
  t0 = millis();
  while (!buttonState()){
    /*Idle*/
  }
  t1 = millis();
  Serial.print("Button unpressed for ");
  Serial.print(t1-t0);
  Serial.print("ms\n");
  return t1-t0;
}

bool AlarmHandler::sensorState() {
  return digitalRead(sensorPin);
}

bool AlarmHandler::buttonState() {
  return digitalRead(buttonPin);
}

bool AlarmHandler::getAlarmState(){
	return alarm;
}

int AlarmHandler::getAlarmCounter(){
	return alarmcounter;
}

void AlarmHandler::incrementAlarmCounter(int value){
	alarmcounter += value;
}
void AlarmHandler::decrementAlarmCounter(int value){
	alarmcounter -= value;
}

bool AlarmHandler::checkVibrations(){
	if (sensorState()) {
		incrementAlarmCounter(100);
		Serial.print("Something moving... ");
		Serial.print(alarmcounter);
		Serial.println();
	}
	else {
    	if (alarmcounter > 0){
    		decrementAlarmCounter(1);
      		Serial.print("No movement... ");
      		Serial.print(alarmcounter);
      		Serial.println();
    	}
  	}
  	if (alarmcounter >= ALARMTHRES){
		alarmcounter = 0;
	    t0_prealert_ = millis();
	    Serial.print("Pre-Alert started\n");
	    return 1;
	    // blinka
	} else{
		return 0;
	}
}



void AlarmHandler::clearSequence() {
  int i;
  for(i=0; i<N_SEQ; i++) {
    candSequence[i] = 0;
  }
}

void AlarmHandler::saveSequence() {
  int i;
  for(i=0; i<N_SEQ; i++) {
    savedSequence[i] = candSequence[i];
  }
}





bool AlarmHandler::checkArmRequest(){
	waitButtonPress();
	unsigned long duration = heldButton();
	if (duration >= ARMTHRES) {
		if (listenToSequence()) {
		  return 1;
		}
	}
	return 0;
}

bool AlarmHandler::checkAlarmTimeout(){
	int t1 = millis();
	Serial.print("Alert timeout: ");
	Serial.print(ALARMTIMEOUT - (t1 - t0_prealert_));
	Serial.println();
	//TODO: disable alarm
	if ((t1 - t0_prealert_) > ALARMTIMEOUT) {
		return 1;
	}
	
}