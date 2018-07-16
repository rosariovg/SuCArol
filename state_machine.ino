#include "include/state_machine.h"

StateMachine::StateMachine(AlarmHandler *alarm_handler){
	alarm_handler_ = alarm_handler;
	state = DISARMED;
}

void StateMachine::spin(){
	if (state==DISARMED) {
		disarmedRoutine();
	}
	else {
		armedRoutine();
	}
	alarm_handler_->blinkPattern();
}

void StateMachine::disarmedRoutine(){
	if(alarm_handler_->checkArmRequest()){
		Serial.print("Armed.\n");
  		state = ARMED;
	}
  
}

void StateMachine::armedRoutine() {
	if (state == ARMED) {
		if(alarm_handler_->checkVibrations()){
			state = PREALARM;
		}
	}
    if (state == PREALARM) {
		if(alarm_handler_->checkAlarmTimeout()){
			alarm_handler_->giveAlarm();
			state = ALARM;
		}else{
			state = DISARMED;
		}
    }
}		
  