/**
 * US_to_led
 * author: Andrey Skvortsov
 *
 * Program to convert ultrasonic sensor input to LED output
**/

//pin declarations
#define TRANSMIT_USS 8
#define RECEIVE_USS 7
#define LED 2
#define BUZZ 3

//logic constants
#define SOUND_SPEED 34  //in centimeters per millisecond
#define MAX_DISTANCE 30  //in centimeters
#define MIN_DISTANCE 2
#define HIGH_RANGE 5
#define MAX_ANALOG_VAL 255

//global logic variables
unsigned long uss_reading = 0;


void setup() {
	pinMode(LED, OUTPUT);
	pinMode(TRANSMIT_USS, OUTPUT);
	pinMode(RECEIVE_USS, INPUT);
	
	digitalWrite(LED, LOW);
}


void loop() {
	uss_reading = distance();
	control_led(uss_reading);
	control_buzzer(uss_reading);
}


void control_led(unsigned long x) {
	if (x <= MAX_DISTANCE) digitalWrite(LED, HIGH);
	else digitalWrite(LED, LOW);

	return;
}


void control_buzzer(unsigned long x) {
	//"write" will be the fraction of the max analog write value (MAX_ANALOG_VAL) to write to buzzer
	double write = 0;
	if (x <= MIN_DISTANCE + HIGH_RANGE) write = 1;  //if we are in "red zone"
	else if (x <= MAX_DISTANCE) write = 1 - ((double)x / (double)MAX_DISTANCE);  //if not in red zone but still within range
	else write = 0;  //if outside of range
	
	analogWrite(BUZZ, (int)(write * MAX_ANALOG_VAL));  //writes to buzzer appropriate fraction of max analog write value
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
