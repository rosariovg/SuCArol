#ifndef BLINKER_H
#define BLINKER_H

#define PAUSE 1
#define LONG_PAUSE 2
#define BLINK 3
#define LONG_BLINK 4

class Blinker{
private:
	char* sequence_;
	int sequence_position_;
	unsigned long time_short_ms_;
	unsigned long time_long_ms_;
	unsigned long t_last_switch_;
	int sequence_parsed_[20];
	int length_sequence_;
	int pin_led_;
	bool is_led_on_;
	void parseSequence();
public:
	Blinker(char* sequence, int pin_led, unsigned long time_short_ms = 200, unsigned long time_long_ms = 600);
	void reinitializeBlinker(char* sequence, unsigned long time_short_ms= 200, unsigned long time_long_ms = 200);
	void updateBlinker();
};

#endif 