#include "include/blinker.h"

void Blinker::parseSequence(){

	int i = 0;
	while(sequence_[i] != '\n' || i == 20){
		switch(sequence_[i]){
			case '-' : sequence_parsed_ [i] = PAUSE;
			case '=' : sequence_parsed_ [i] = LONG_PAUSE;
			case '.' : sequence_parsed_[i] = BLINK;
			case ':' : sequence_parsed_[i] = LONG_BLINK;
		} 
		i++;
	}
	length_sequence_ = i;
}

void Blinker::updateBlinker(){
	unsigned long t = millis();
	int state = sequence_parsed_[sequence_position_];
	unsigned long threshold;
	if(state == PAUSE || state== BLINK){
		threshold = time_short_ms_;
	} else{
		threshold = time_long_ms_;
	}
	if (t - t_last_switch_ > threshold){
		t_last_switch_ = t;
		sequence_position_ = (sequence_position_ + 1)% length_sequence_;
		if(state == PAUSE || state == LONG_PAUSE){
			digitalWrite(pin_led_, LOW);
			is_led_on_ = false;
		}else{
			digitalWrite(pin_led_, HIGH);
			is_led_on_ = true;
		}
	}
};

void Blinker::reinitializeBlinker(char* sequence, unsigned long time_short_ms, unsigned long time_long_ms){
	digitalWrite(pin_led_, LOW);
	is_led_on_ = false;
	sequence_ = sequence;
	sequence_position_ = 0;
	time_short_ms_ = time_short_ms;
	time_long_ms_ = time_long_ms;
	parseSequence();
	t_last_switch_ = millis();
}

Blinker::Blinker(char* sequence, int pin_led, unsigned long time_short_ms , unsigned long time_long_ms){
	pin_led_ = pin_led;
	digitalWrite(pin_led_, LOW);
	is_led_on_ = false;
	sequence_ = sequence;
	sequence_position_ = 0;
	time_short_ms_ = time_short_ms;
	time_long_ms_ = time_long_ms;
	parseSequence();
	t_last_switch_ = millis();
};