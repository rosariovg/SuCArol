#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#define DISARMED 0
#define ARMED 1
#define PREALARM 2
#define ALARM 3
#include "alarm_handler.h"

class StateMachine{
public:
	StateMachine(AlarmHandler *alarm_handler);
	int getState();
	void spin();
private:
	int state;
	void disarmedRoutine();
	void armedRoutine();
	AlarmHandler* alarm_handler_;
};

#endif