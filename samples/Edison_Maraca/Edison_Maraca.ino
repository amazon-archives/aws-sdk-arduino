#include <EdisonAWSImplementations.h>
#include <AWSFoundationalTypes.h>
#include "keys.h"

#include "HardwareFunctions.h"
#include "AWShelperFunctions.h"

// It is an Arduino hack to include libraries that are referenced in other libraries. 
// For example WiFi is used in AWS libraries but we still need to include in the main sketch file for it to be recognized.

#include <WiFi.h>
#include <Wire.h>
#include <ADXL345.h>

AWS_Service_ID CurrentService;

void setup() {

	/* Begin serial communication. */
	Serial.begin(9600);
	digitalWrite(BUZZER_PIN, LOW);

	if (true == Edison_Wifi_Setup(ssid, pass)) {
		Serial.println("Connected to wifi");
		printWifiStatus();
	}

	configureIO();
	accelerometerInit();
       
	SNSClient_Setup();
	DynamoBDClient_Setup();
	KinesisClient_Setup();

	CurrentService = DYNAMO_DB;
	indicateServiceThroughLED_blink_Buzzer(DYNAMO_DB);

	Serial.println("Setup complete! Looping main program");
	Serial.println("Initial mode: DynamoDB");      
        
}

unsigned long currentTime = 0;
unsigned long LastPostTime = 0;
enum {
	POSTING_PERIOD_MS = 500
};

int buttonVal = LOW;
int prevButtonVal = LOW;

double xyz[3];

void loop() {                

	readAcceleration(xyz);	
	if (isAccelerometerShaking(xyz)) {
                printAcceleration(xyz);
		// light the onboard LED
		digitalWrite(EDISON_LED_PIN, HIGH);

		currentTime = millis();
		if ((currentTime - LastPostTime) > POSTING_PERIOD_MS) {
			currentTime = millis();
			switch (CurrentService) {
			case 0:
				Serial.println("Posting to Kinesis");
				putKinesis(xyz[0], xyz[1], xyz[2]);
				break;
			case 1:
				Serial.println("Posting to DynamoDB");
				putDynamoDb();
				break;
			case 2:
				Serial.println("Posting to SNS");
				putSns();
				break;
			default:
				Serial.println("Wrong selection");
				break;
			}
			LastPostTime = currentTime;
		}
	} else {
		//Serial.println("Not publishing...");
		digitalWrite(EDISON_LED_PIN, LOW);
	}

	// check if button has been pressed
	buttonVal = digitalRead(BUTTON_PIN);
	if (prevButtonVal == LOW && buttonVal == HIGH) {
		CurrentService = changeService(CurrentService);
	}
	prevButtonVal = buttonVal;
	delay(200);
}

































































