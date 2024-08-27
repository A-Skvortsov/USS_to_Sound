/**
 * US_to_led
 * author: Andrey Skvortsov
 *
 * Program to convert ultrasonic sensor input to LED output
**/

//pin declarations
const int TRANSMIT_USS = 8;
const int RECEIVE_USS = 7;
const int LED = 2;

//logic variables
const int SOUND_SPEED = 34;  //in centimeters per millisecond
const int RANGE = 20;  //in centimeters
int distance = 0;
bool close_enough = false;


void setup() {
	pinMode(LED, OUTPUT);
	pinMode(TRANSMIT_USS, OUTPUT);
	pinMode(RECEIVE_USS, INPUT);
	
	digitalWrite(LED, LOW);
}


void loop() {
	close_enough = is_close_enough();
	if (close_enough) digitalWrite(LED, HIGH);
	else digitalWrite(LED, LOW);
}


bool is_close_enough() {
	distance = SOUND_SPEED * get_travel_time_ms() / 2;
	
	if (distance <= RANGE) return true;
	else return false;
}


unsigned long get_travel_time_ms() {
	digitalWrite(TRANSMIT_USS, LOW);  //to clear the sensor
	digitalWrite(TRANSMIT_USS, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRANSMIT_USS, LOW);
	
	return (pulseIn(RECEIVE_USS, HIGH) / 1000);
}
