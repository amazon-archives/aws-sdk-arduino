#include "HardwareFunctions.h"
#include <ADXL345.h>

void configureIO() {
	pinMode(EDISON_LED_PIN, OUTPUT);
	pinMode(GROVE_LED_PIN, OUTPUT);
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT);
}

void blinkGroveLed(int times) {
	// blink the LED and beep the buzzer n times to indicate new service
	for (int i = 0; i < times; i++) {
		digitalWrite(GROVE_LED_PIN, HIGH);
		digitalWrite(BUZZER_PIN, HIGH);
		delay(100);
		digitalWrite(GROVE_LED_PIN, LOW);
		digitalWrite(BUZZER_PIN, LOW);
		delay(100);
	}
}

ADXL345 adxl;
double previousXYZ[3] = { 0, 0, 0 };
double currentXYZ[3];
double nThreshold;

void accelerometerInit() {
	adxl.powerOn();
	nThreshold = 0.6; // Need to calibrate it
}

void readAcceleration(double *xyz){
	adxl.getAcceleration(xyz);
}

bool isAccelerometerShaking(double *xyz) {
	bool ret_val = false;
	double absX, absY, absZ;

	absX = abs(previousXYZ[0] - xyz[0]);
	absY = abs(previousXYZ[1] - xyz[1]);
	absZ = abs(previousXYZ[2] - xyz[2]);

	if(absX > nThreshold || absY > nThreshold || absZ > nThreshold){
		ret_val = true;
	}

	previousXYZ[0] = xyz[0];
	previousXYZ[1] = xyz[1];
	previousXYZ[2] = xyz[2];

	return ret_val;
}

void printAcceleration(double *xyz){
 	Serial.println("Acceleration values: ");
	Serial.print(xyz[0]);
	Serial.print(",");
	Serial.print(xyz[1]);
	Serial.print(",");
	Serial.print(xyz[2]);
	Serial.print("\n"); 
}


