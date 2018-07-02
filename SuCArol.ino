/*
Press 3 sec
Ask PIN to arm
Armed


Press tap tap taa-5 sec-aaap
Record new PIN
End with 5 sec press

...

ALARM
Immediately wait for PIN to disarm
 */

/* ----------------- Pinout ------------------ */
int sensorPin = 4;
int buttonPin = 9;
int alarmPin = 13;
//int ledPin = 13;

/* ---------------- Variables ---------------- */
unsigned long alarmcounter = 0, buttoncounter = 0;
unsigned long t0 = 0, t1 = 0;
unsigned long armThresh = 1500, disarmThresh = 100, alarmThresh = 1000;
unsigned long alarmTimeout = 5000;
bool armed = false, preAlert = false, alarm = false;
#define N_SEQ 10
unsigned long savedSequence[N_SEQ], candSequence[N_SEQ];

/* ----------- Forward declarations ---------- */
bool sensorState();
bool buttonState();
void disarmedRoutine();
void armedRoutine();
void waitButtonPress();
bool waitButtonPressFor(unsigned int timeout);
unsigned long unpressedButton();
unsigned long heldButton();
bool listenToSequence();
void giveAlarm();
void shutupAlarm();

/* ------------------------------------------- */
void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(sensorPin, INPUT);
  pinMode(alarmPin, OUTPUT);

  Serial.begin(9600);
  Serial.print("Initialized\n");
}

bool sensorState() {
  return digitalRead(sensorPin);
}

bool buttonState() {
  return digitalRead(buttonPin);
}

void loop() {
  if (!armed) {
    disarmedRoutine();
  }
  else {
    armedRoutine();
  }
}

void disarmedRoutine() {
  waitButtonPress();
  
  unsigned long duration = heldButton();
  if (duration >= armThresh) {
    if (listenToSequence()) {
      Serial.print("Armed.\n");
      //armed = true;
    }
  }
}

void armedRoutine() {
  if (!alarm) {
    if (!preAlert) {
      if (sensorState()) {
        alarmcounter += 100;
        Serial.print("Something moving... ");
        Serial.print(alarmcounter);
        Serial.println();
      }
      else {
        if (alarmcounter > 0){
          alarmcounter --;
          Serial.print("No movement... ");
          Serial.print(alarmcounter);
          Serial.println();
        }
      }

      if (alarmcounter >= alarmThresh){
        alarmcounter = 0;
        t0 = millis();
        Serial.print("Pre-Alert started\n");
        preAlert = true;
        // blinka
      }
    }
    else {
      t1 = millis();
      Serial.print("Alert timeout: ");
      Serial.print(alarmTimeout - (t1 - t0));
      Serial.println();
      if ((t1 - t0) > alarmTimeout) {
        giveAlarm();
      }
    }
  }
  
  /*if (holdButton(disarmThresh)) {
    shutupAlarm();
    Serial.print("Disarmed.\n");
    preAlert = false;
    armed = false;
  }*/
  
}

void waitButtonPress() {
  while (!buttonState()){
    /*Idle*/
  }
}

bool waitButtonPressFor(unsigned int timeout) {
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

unsigned long unpressedButton() {
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

unsigned long heldButton() {
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

bool listenToSequence() {
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

void clearSequence() {
  int i;
  for(i=0; i<N_SEQ; i++) {
    candSequence[i] = 0;
  }
}

void saveSequence() {
  int i;
  for(i=0; i<N_SEQ; i++) {
    savedSequence[i] = candSequence[i];
  }
}

void giveAlarm() {
  Serial.print("ALARM!\n");
  alarm = true;
  digitalWrite(alarmPin, HIGH);
}

void shutupAlarm() {
  alarm = false;
  digitalWrite(alarmPin, LOW);
}

