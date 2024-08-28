/**
 * US_to_led
 * author: Andrey Skvortsov
 *
 * Program to convert ultrasonic sensor input to LED output
**/

//imported libraries
#include <math.h>

//pin declarations
#define TRANSMIT_USS 8
#define RECEIVE_USS 7
#define LED 4
#define BUZZ 3
#define BTN 2

//logic constants
#define SOUND_SPEED 34  //in centimeters per millisecond
#define MAX_DISTANCE 100  //in centimeters (max. distance from sensor to trigger buzzer)
#define MIN_DISTANCE 2  //min. distance from sensor to trigger buzzer (obsolete?)
#define HIGH_RANGE 5  //range around MIN_DISTANCE within which MAX_VOLUME should be outputted from buzzer
#define DEBOUNCE 100  //delay for button debouncing
#define MAX_VOLUME 255  //max. volume to be outputted from buzzer
#define MIN_VOLUME 10  //min. volume to be outputted from buzzer (if buzzer triggered. Otherwise output 0)

//global logic variables
unsigned long uss_reading = 0;  //stores time reading from ultrasonic sensor
volatile bool on = false;  //toggles on/off of the whole I/O system


void setup() {
	pinMode(LED, OUTPUT);
	pinMode(TRANSMIT_USS, OUTPUT);
	pinMode(RECEIVE_USS, INPUT);
	pinMode(BTN, INPUT);
	
	digitalWrite(LED, LOW);
	attachInterrupt(digitalPinToInterrupt(BTN), btn_press, FALLING);
}


void loop() {
	delay(DEBOUNCE);
	
	if (on) {		
		uss_reading = getDistance();
		control_led(uss_reading);
		control_buzzer(uss_reading);
	}
}


void btn_press() {
	on = !on;
	digitalWrite(LED, LOW);
	analogWrite(BUZZ, 0);
}


void control_led(unsigned long x) {
	//maybe add: if (x <= MIN_DISTANCE + HIGH_RANGE) blink one time; (blinks would repeat with loop())
	if (x <= MAX_DISTANCE) digitalWrite(LED, HIGH);
	else digitalWrite(LED, LOW);

	return;
}


void control_buzzer(unsigned long x) {
	//"write" will be the fraction of the max analog write value (MAX_ANALOG_VAL) to write to buzzer
	double write = 0;
	if (x <= MIN_DISTANCE + HIGH_RANGE) write = MAX_VOLUME;  //if in "red zone"
	else if (x <= MAX_DISTANCE) write = map(x, MIN_DISTANCE, MAX_DISTANCE, MAX_VOLUME, MIN_VOLUME);  //if in range
	//note: if not in range, default write value is 0
	
	analogWrite(BUZZ, write);
}


/* getDistance()
 * Computes distance measured by ultrasonic sensor using standard [distance = speed * time] formula.
*/
unsigned long getDistance() {
	digitalWrite(TRANSMIT_USS, LOW);  //to clear the sensor
	digitalWrite(TRANSMIT_USS, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRANSMIT_USS, LOW);
	
	return (SOUND_SPEED * pulseIn(RECEIVE_USS, HIGH) / 2000);  // div. by 1000 to convert microseconds to milliseconds, div. by 2 due to ultrasonic sensor logic (distance must be divided by 2)
}
