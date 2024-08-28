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
#define MAX_DISTANCE 30  //in centimeters
#define MIN_DISTANCE 2
#define HIGH_RANGE 5
#define MAX_ANALOG_VAL 255
#define DEBOUNCE 100

//global logic variables
unsigned long uss_reading = 0;
volatile bool on = false;


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
		uss_reading = distance();
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
	if (x <= MAX_DISTANCE) digitalWrite(LED, HIGH);
	else digitalWrite(LED, LOW);

	return;
}


void control_buzzer(unsigned long x) {
	//"write" will be the fraction of the max analog write value (MAX_ANALOG_VAL) to write to buzzer
	double write = 0;
	if (x <= MIN_DISTANCE + HIGH_RANGE) write = 0.5;  //if we are in "red zone"
	else if (x <= MAX_DISTANCE) write = (1 - ((double)x / (double)MAX_DISTANCE)) / 2;  //if not in red zone but still within range
	else write = 0;  //if outside of range
	
	analogWrite(BUZZ, trunc(write * MAX_ANALOG_VAL));  //writes to buzzer appropriate fraction of max analog write value
}


/* distance()
 * Computes distance measured by ultrasonic sensor using standard [distance = speed * time] formula.
*/
unsigned long distance() {
	return SOUND_SPEED * get_travel_time_ms() / 2;
}


unsigned long get_travel_time_ms() {
	digitalWrite(TRANSMIT_USS, LOW);  //to clear the sensor
	digitalWrite(TRANSMIT_USS, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRANSMIT_USS, LOW);
	
	return (pulseIn(RECEIVE_USS, HIGH) / 1000);
}
