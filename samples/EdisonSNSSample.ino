/*
 *  This sample publishes a message to an Amazon SNS target (topic or endpoint).
 *	
 *  The first attempt will always fail because of the clock implementation on the Edison.
 *  The library uses the timestamp in the AWS response to seed the clock for subsequent attempts.
 *
 *  For this demo to work, add your AWS keys in "awsSecKey" and "awsKeyID", create an SNS topic 
 *  or endpoint and populate the TARGET_ARN with the topic or endpoint ARN.
 *
 */

#include <AmazonSNSClient.h>
#include <EdisonAWSImplementations.h>
#include <AWSFoundationalTypes.h>
#include <WiFi.h>

/* AWS credentials */
const char* awsSecKey = "";
const char* awsKeyID = "";

/* Wi-Fi */
char ssid[] = ""; //  your network SSID (name) 
char pass[] = "";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

/* Constants for connecting to Amazon SNS. */
static const char* TARGET_ARN = "";  // replace each ':' with '%3A'
static const char* AWS_REGION = "";  // us-west-2 etc
static const char* AWS_ENDPOINT = "amazonaws.com";

/* Light the LED while a message is transmitted. */
int led = 13;

/* Device independent implementations required for AmazonSNSClient to function. */
EdisonHttpClient httpClient;
EdisonDateTimeProvider dateTimeProvider;

/* AWS objects. */
AmazonSNSClient snsClient;
PublishInput publishInput;
ActionError actionError;

void setup() {
    /* Begin serial communication. */
    Serial.begin(9600);
    delay(5000);

    pinMode(led, OUTPUT);

    /* Require a Wi-Fi shield. */
    if (WL_NO_SHIELD == WiFi.status()) {
        Serial.println("Wi-Fi shield not present."); 
		
        /* Do not continue. */
        while(true);
    } 

    String fv = WiFi.firmwareVersion();	
    if (fv != "1.1.0") {
        Serial.println("Please upgrade the Wi-Fi firmware");
    }
  	
    /* Attempt to connect to the Wi-fi network. */
    while (status != WL_CONNECTED) { 
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);

        /* Connect to WPA/WPA2 network. Change this line if using open or WEP network. */
        status = WiFi.begin(ssid, pass);
  
        /* Wait 10 seconds for connection. */
        delay(10000);
    }
	
    Serial.println("Connected to wifi");
    printWifiStatus();

    /* Initialize Amazon SNS client. */
    snsClient.setAWSRegion(AWS_REGION);
    snsClient.setAWSEndpoint(AWS_ENDPOINT);
    snsClient.setAWSSecretKey(awsSecKey);
    snsClient.setAWSKeyID(awsKeyID);
    snsClient.setHttpClient(&httpClient);
    snsClient.setDateTimeProvider(&dateTimeProvider);
}

void loop() {
    /* Repeat SNS attempt every 60s. */
    delay(60000);

    /* Light up the LED. */
    digitalWrite(led, HIGH);
    Serial.println("Start Loop");

    MinimalString dateTime(dateTimeProvider.getDateTime());
    dateTimeProvider.sync(dateTime.getCStr());

    /* Delivery attempt to Amazon SNS. */
    publishInput.setMessage(dateTime);
    publishInput.setTargetArn(TARGET_ARN);
    PublishOutput publishOutput = snsClient.publish(publishInput, actionError);
    Serial.println(publishOutput.getMessageId().getCStr());

    /* Finish loop and turn off the LED. */
    Serial.println(dateTime.getCStr());
    Serial.println("Finished loop");
    delay(1000);
    digitalWrite(led, LOW);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

