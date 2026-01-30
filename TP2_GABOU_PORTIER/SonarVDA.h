#ifndef SonarVDA_h
#define SonarVDA_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TRIGGER_PIN 10
#define ECHO_PIN 8
#define MAX_SENSOR_DISTANCE 500 // Maximum sensor distance can be as high as 500cm, no reason to wait for ping longer than sound takes to travel this distance and back. Default=500
#define US_ROUNDTRIP_CM 57		// Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space. Default=57

#define NO_ECHO 0               // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE or max_cm_distance. Default=0

class SonarVDA {
	public:
		SonarVDA();
		void start();
		void stop();
		int get_last_measure();
		void set_threshold(int nb_cm, void (*userFunc)(void));
		void set_trigger(int nb_cm_min, int nb_cm_max, void (*userFunc)(void), void (*userFunc2)(void));
	private:
		void timer_setup();
};


#endif
