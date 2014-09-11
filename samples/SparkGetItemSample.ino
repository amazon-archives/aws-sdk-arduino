#include "AmazonDynamoDBClient.h"
#include "SparkAWSImplementations.h"
#include "AWSFoundationalTypes.h"
#include "keys.h"
#include <stdlib.h>

/*
 *
 *  This sample uses GetItem on a DynamoDB table to retrieve the state of a RGB
 * LED. Pressing the button gets the saved color values from 0-255.
 *
 * For this demo to work you must have keys.h/.ccp files that contain your AWS
 * access keys and define "awsSecKey" and "awsKeyID", a DynamoDB table with the
 * name defined by the constant TABLE_NAME with hash and range keys as defined
 * by constants HASH_KEY_NAME/RANGE_KEY_NAME, and and item in that table with
 * attributes as defined by HASH_KEY_VALUE/RANGE_KEY_VALUE and number
 * attributes R G and B.
 *
 */

/* D2 is the pin connected to the button/switch. For buttons or tilt switches,
 * have the pin connected to one side of the button and also connected to
 * ground with a resistor. Have the other side of the button connected to
 * 5v/3v. */
static const int SWITCH_PIN = D2;
/* Contants describing DynamoDB table and values being used. */
static const char* HASH_KEY_NAME = "DemoName";
static const char* HASH_KEY_VALUE = "Colors";
static const char* RANGE_KEY_NAME = "id";
static const char* RANGE_KEY_VALUE = "1";
static const char* TABLE_NAME = "AWSArduinoSDKDemo";
static const int KEY_SIZE = 2;
/* Constants for connecting to DynamoDB. */
static const char* AWS_REGION = "us-east-1";
static const char* AWS_ENDPOINT = "amazonaws.com";

/* 0 or 1 to determine state of the switch */
int switchState = 0;

/* Device independent implementations required for AmazonDynamoDBClient to
 * function. */
SparkHttpClient httpClient;
SparkDateTimeProvider dateTimeProvider;

AmazonDynamoDBClient ddbClient;

GetItemInput getItemInput;
AttributeValue hashKey;
AttributeValue rangeKey;
ActionError actionError;

void setup() {
    /* Begin serial communication. */
    Serial.begin(9600);
    /* Take control of spark's built-int rgb led. */
    RGB.control(true);
    /* Initialize ddbClient. */
    ddbClient.setAWSRegion(AWS_REGION);
    ddbClient.setAWSEndpoint(AWS_ENDPOINT);
    ddbClient.setAWSSecretKey(awsSecKey);
    ddbClient.setAWSKeyID(awsKeyID);
    ddbClient.setHttpClient(&httpClient);
    ddbClient.setDateTimeProvider(&dateTimeProvider);
    /* Open SWITCH_PIN to be read from. */
    pinMode(SWITCH_PIN, INPUT);
}

void loop() {
    int prevSwitchState = switchState;
    /* Read the state of the button/switch. */
    switchState = digitalRead(SWITCH_PIN);

    /* If state has changed back to low a full button press has passed */
    if (prevSwitchState == HIGH && switchState == LOW) {
        /* Set the string and number values for the range and hash Keys,
         * respectively. */
        hashKey.setS(HASH_KEY_VALUE);
        rangeKey.setN(RANGE_KEY_VALUE);

        /* Create key-value pairs out of the hash and range keys, and create
         * a map out off them, which is the key. */
        MinimalKeyValuePair < MinimalString, AttributeValue
                > pair1(HASH_KEY_NAME, hashKey);
        MinimalKeyValuePair < MinimalString, AttributeValue
                > pair2(RANGE_KEY_NAME, rangeKey);
        MinimalKeyValuePair<MinimalString, AttributeValue> keyArray[] = { pair1,
                pair2 };
        getItemInput.setKey(MinimalMap < AttributeValue > (keyArray, KEY_SIZE));

        /* Looking to get the R G and B values */
        MinimalString attributesToGet[] = { "R", "G", "B" };
        getItemInput.setAttributesToGet(
                MinimalList < MinimalString > (attributesToGet, 3));

        /* Set other values. */
        getItemInput.setTableName(TABLE_NAME);

        /* Perform getItem and check for errors. */
        GetItemOutput getItemOutput = ddbClient.getItem(getItemInput,
                actionError);
        switch (actionError) {
        case NONE_ACTIONERROR:
            Serial.println("GetItem succeeded!");
            {
                /* Get the "item" from the getItem output. */
                MinimalMap < AttributeValue > attributeMap =
                        getItemOutput.getItem();
                AttributeValue av;
                /* Get the rgb values and set the led with them. */
                attributeMap.get("R", av);
                int R = atoi(av.getN().getCStr());
                attributeMap.get("G", av);
                int G = atoi(av.getN().getCStr());
                attributeMap.get("B", av);
                int B = atoi(av.getN().getCStr());
                RGB.color(R, G, B);
            }
            break;
        case INVALID_REQUEST_ACTIONERROR:
            Serial.print("ERROR: ");
            Serial.println(getItemOutput.getErrorMessage().getCStr());
            break;
        case MISSING_REQUIRED_ARGS_ACTIONERROR:
            Serial.println(
                    "ERROR: Required arguments were not set for GetItemInput");
            break;
        case RESPONSE_PARSING_ACTIONERROR:
            Serial.println("ERROR: Problem parsing http response of GetItem\n");
            break;
        case CONNECTION_ACTIONERROR:
            Serial.println("ERROR: Connection problem");
            break;
        }

        /* wait to not double-record */
        delay(750);
    }
    delay(50);
}
