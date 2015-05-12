#ifndef HARDWARE_FUNCTIONS_H_
#define HARDWARE_FUNCTIONS_H_

enum pinConfig{
	MEDIATEK_LED_PIN = 13,
	GROVE_LED_PIN = 3,
	BUZZER_PIN = 6,
	BUTTON_PIN = 7
};

void configureIO();
void blinkGroveLed(int times);

void accelerometerInit();
void readAcceleration(double *xyz);
bool isAccelerometerShaking(double *xyz);
void printAcceleration(double *xyz);

#endif







































































