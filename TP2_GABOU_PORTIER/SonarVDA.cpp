#include "SonarVDA.h"

/*
measure every 60ms using OCR1A
pulse width uses OCR1B
Echo signal is generated "automatically" by fastPWM mode, but on Arduino pin 10 (OC1B) instead of 9
COMPA interrupt is used to detect wrong 
*/

volatile bool echo_on;
volatile int echo_start;
volatile int last_measure;
volatile bool current_measure_ok; // set to false if echo is too long, so we don't update last_measure variable
volatile int threshold, threshold2; // threshold in timer unit (4us basis) under which callback is called.
volatile bool check_threshold, check_threshold2;
volatile bool trigger_wait_min;

SonarVDA::SonarVDA() {
	pinMode(ECHO_PIN, INPUT);
	pinMode(TRIGGER_PIN, OUTPUT);
	echo_on = false;
	echo_start = false;
	last_measure = 0;
	check_threshold = false;
	check_threshold2 = false;
}

void SonarVDA::start() {
	timer_setup();
	echo_on = false;
	echo_start = false;
	trigger_wait_min = true;
	TCCR1B |= (1<<CS11)|(1<<CS10); // prescaler to 64 (4uS/count, 4uS-262.14mS range).
}

void SonarVDA::stop() {
	TCCR1B = 0;
}

int SonarVDA::get_last_measure() {
	return (last_measure<<2) / US_ROUNDTRIP_CM;
}

void (*thresholdFunc)();
void (*thresholdFunc2)();

void SonarVDA::set_threshold(int nb_cm, void (*userFunc)(void)) {
	threshold = (nb_cm * US_ROUNDTRIP_CM) >> 2; // convert cm to timer unit
	thresholdFunc = userFunc;
	check_threshold = true;
}

void SonarVDA::set_trigger(int nb_cm_min, int nb_cm_max, void (*userFunc)(void), void (*userFunc2)(void)) {
	threshold = (nb_cm_min * US_ROUNDTRIP_CM) >> 2; // convert cm to timer unit
	threshold2 = (nb_cm_max * US_ROUNDTRIP_CM) >> 2; // convert cm to timer unit
	thresholdFunc = userFunc;
	thresholdFunc2 = userFunc2;
	check_threshold = true;
	check_threshold2 = true;
}

ISR(TIMER1_COMPA_vect) {
	if (echo_on) { // echo is still on ! not normal
		current_measure_ok = false;
	}
}

ISR(TIMER1_CAPT_vect) {
	if (!echo_on) { // just got raising edge
		echo_start = ICR1;
		TCCR1B &= ~(1<<ICES1); // now get falling edge;
		echo_on = true;
	}
	else { // just got falling edge
		if (!current_measure_ok) {
			current_measure_ok = true;
		}
		else {
			last_measure = ICR1 - echo_start;
			if (check_threshold && trigger_wait_min && (last_measure < threshold)) {
				thresholdFunc();
				if (check_threshold2) {
					trigger_wait_min = false;
				}
			}
			if (check_threshold2 && !trigger_wait_min && (last_measure > threshold2)) {
				thresholdFunc2();
				trigger_wait_min = true;
			}
		}
		TCCR1B |= (1<<ICES1); // now get next raising edge;
		echo_on = false;
	}
}

void SonarVDA::timer_setup() {
	stop();        // Disable Timer1 interrupt.
	TIFR1 = 0xFF; // Clear all interrupts
	TCCR1A = (1<<COM1B1)|(1<<WGM11)|(1<<WGM10);
	TCCR1B = (1<<ICES1)|(1<<WGM13)|(1<<WGM12);  // Start with rising edge of IC, mode Fast PWM
	OCR1A = 15000; // max = 60ms
	OCR1B = 10; // pulse width = 50us
	TIMSK1 = (1<<ICIE1)|(1<<OCIE1A);//|(1<<OCIE1B); // ICIE1 for IC, OCIE1A for 60ms, OCIE1B for trigger pulse width
	TCNT1 = 0;           // Reset Timer1 counter.
}
