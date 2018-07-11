#ifndef ALARM_HANDLER_H
#define ALARM_HANDLER_H

#define ALARMTHRES 1000
#define DISARMTHRESH 100
#define ARMTHRES 1500
#define ALARMTIMEOUT 5000
#define N_SEQ 10


class AlarmHandler{
private:
	bool armed = false, preAlert = false, alarm = false;
	unsigned long savedSequence[N_SEQ], candSequence[N_SEQ];
	int alarmcounter = 0;
	int buttoncounter = 0;
	int t0_prealert_;

public:
	AlarmHandler();
	bool getAlarmState();
	bool sensorState();
	bool buttonState();
	void waitButtonPress();
	bool waitButtonPressFor(unsigned int timeout);
	unsigned long unpressedButton();
	unsigned long heldButton();
	bool listenToSequence();
	void giveAlarm();
	void shutupAlarm();
	int getAlarmCounter();
	int sensorPin = 4;
	int buttonPin = 9;
	int alarmPin = 13;
	void incrementAlarmCounter(int value);
	void decrementAlarmCounter(int value);
	bool checkVibrations();
	void clearSequence();
	void saveSequence();
	unsigned long heldButton(unsigned long ) ;
	bool checkArmRequest();
	bool checkAlarmTimeout();
};

#endif