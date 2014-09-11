/* This sample uses PutItem and GetItem on a DynamoDB table to save the state
 * of a RGB LED. Pressing one button gets the saved color values from 0-255 and
 * another button saves the current color values. Three additional buttons
 * cycle the brightness of each color of the LED. This demo can be used with
 * multiple devices on the same table and item in order to synchronize their
 * colors.
 *
 * For this demo to work you must have keys.h/.ccp files that contain your AWS
 * access keys and define "awsSecKey" and "awsKeyID", a DynamoDB table with the
 * name defined by the constant TABLE_NAME with hash and range keys as defined
 * by constants HASH_KEY_NAME/RANGE_KEY_NAME, and and item in that table with
 * attributes as defined by HASH_KEY_VALUE/RANGE_KEY_VALUE and number
 * attributes R G and B. */

#include "GalileoAWSImplementations.h"
#include "AmazonDynamoDBClient.h"
#include "AWSFoundationalTypes.h"
#include "keys.h"
#include <Ethernet.h>

/* Constants for incrementing color. */
static const int INCREMENT = 50;
static const int MAX_COLOR = 255;
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

/* Pins for the R G and B leads to a RGB LED. Be sure to use a resistor with
 * each lead. */
static const int R_LED_PIN = 6;
static const int G_LED_PIN = 9;
static const int B_LED_PIN = 10;

/* For buttons or tilt switches, have the pin connected to one side of the button and also
 * connected to ground with a resistor. Have the other side of the button
 * connected to 5v/3v. */
/* The pin connected to the button for putItem. */
static const int PUT_SWITCH_PIN = 2;
/* The pin connected to the button for putItem. */
static const int GET_SWITCH_PIN = 4;
/* The pin connected to the buttons to change the brightness of the RGB LED. */
static const int R_SWITCH_PIN = 7;
static const int G_SWITCH_PIN = 8;
static const int B_SWITCH_PIN = 12;

/* 0 or 1 to determine state of the switches */
int putSwitchState = 0;
int getSwitchState = 0;
int rSwitchState = 0;
int gSwitchState = 0;
int bSwitchState = 0;

/* Color LEDs values 0-255. */
int R = 0;
int G = 0;
int B = 0;

/* Device independent implementations required for AmazonDynamoDBClient to
 * function. */
GalileoHttpClient httpClient;
GalileoDateTimeProvider dateTimeProvider;

AmazonDynamoDBClient ddbClient;

/* Reused objects. */
GetItemInput getItemInput;
PutItemInput putItemInput;
AttributeValue hashKey;
AttributeValue rangeKey;
ActionError actionError;

void refreshColor() {
    analogWrite(R_LED_PIN, R);
    analogWrite(G_LED_PIN, G);
    analogWrite(B_LED_PIN, B);
}

void setup() {
    /* Begin serial communication. */
    Serial.begin(9600);
    /* Ethernet is supposed to start up when the Galileo is powered on with the
     * ethernet cable in. This can fail, so calling this command ensures it is
     * started. */
    system("ifup eth0");
    /* Initialize ddbClient. */
    ddbClient.setAWSRegion(AWS_REGION);
    ddbClient.setAWSEndpoint(AWS_ENDPOINT);
    ddbClient.setAWSSecretKey(awsSecKey);
    ddbClient.setAWSKeyID(awsKeyID);
    ddbClient.setHttpClient(&httpClient);
    ddbClient.setDateTimeProvider(&dateTimeProvider);
    /* Open switchPins to be read from and led pins to write to. */
    pinMode(PUT_SWITCH_PIN, INPUT);
    pinMode(GET_SWITCH_PIN, INPUT);
    pinMode(R_SWITCH_PIN, INPUT);
    pinMode(G_SWITCH_PIN, INPUT);
    pinMode(B_SWITCH_PIN, INPUT);
    pinMode(R_LED_PIN, OUTPUT);
    pinMode(G_LED_PIN, OUTPUT);
    pinMode(B_LED_PIN, OUTPUT);
}

void loop() {
    int prevPutSwitchState = putSwitchState;
    int prevGetSwitchState = getSwitchState;
    int prevRSwitchState = rSwitchState;
    int prevGSwitchState = gSwitchState;
    int prevBSwitchState = bSwitchState;
    /* Read the state of switches/buttons pins. */
    putSwitchState = digitalRead(PUT_SWITCH_PIN);
    getSwitchState = digitalRead(GET_SWITCH_PIN);
    rSwitchState = digitalRead(R_SWITCH_PIN);
    gSwitchState = digitalRead(G_SWITCH_PIN);
    bSwitchState = digitalRead(B_SWITCH_PIN);

    /* For any awitch/button, if the state has changed from high to low, the
     * button has been pressed (or the tilt switch has been tilted. */
    if (prevRSwitchState == HIGH && rSwitchState == LOW) {
        /* Increase the brightness of the color (Red in this case) by INCREMENT
         * amount, topping out at (MAX_COLOR). */
        R = (R + INCREMENT) % (MAX_COLOR + 1);
        /* If the brightness just went over max color and wrapped around, set
         * the brightness to 0. */
        R = R < INCREMENT ? 0 : R;
        refreshColor();
    }
    if (prevGSwitchState == HIGH && gSwitchState == LOW) {
        G = (G + INCREMENT) % (MAX_COLOR + 1);
        G = G < INCREMENT ? 0 : G;
        refreshColor();
    }
    if (prevBSwitchState == HIGH && bSwitchState == LOW) {
        B = (B + INCREMENT) % (MAX_COLOR + 1);
        B = B < INCREMENT ? 0 : B;
        refreshColor();
    }
    if (prevPutSwitchState == HIGH && putSwitchState == LOW) {
        /* Create an Item. */
        char numberBuffer[4];
        AttributeValue demoName;
        demoName.setS(HASH_KEY_VALUE);
        AttributeValue entryNumber;
        entryNumber.setN(RANGE_KEY_VALUE);

        /* For R G and B, create an AttributeValue, convert the number to a
         * string (AttributeValue object represents numbers as strings), and
         * use setN to apply that value to the AttributeValue. */
        AttributeValue rAttributeValue;
        sprintf(numberBuffer, "%d", R);
        rAttributeValue.setN(numberBuffer);
        AttributeValue gAttributeValue;
        sprintf(numberBuffer, "%d", G);
        gAttributeValue.setN(numberBuffer);
        AttributeValue bAttributeValue;
        sprintf(numberBuffer, "%d", B);
        bAttributeValue.setN(numberBuffer);

        /* Create the Key-value pairs and make an array of them. */
        MinimalKeyValuePair < MinimalString, AttributeValue
                > att1(HASH_KEY_NAME, demoName);
        MinimalKeyValuePair < MinimalString, AttributeValue
                > att2(RANGE_KEY_NAME, entryNumber);
        MinimalKeyValuePair < MinimalString, AttributeValue
                > att3("R", rAttributeValue);
        MinimalKeyValuePair < MinimalString, AttributeValue
                > att4("G", gAttributeValue);
        MinimalKeyValuePair < MinimalString, AttributeValue
                > att5("B", bAttributeValue);
        MinimalKeyValuePair<MinimalString, AttributeValue> itemArray[] = { att1,
                att2, att3, att4, att5 };

        /* Set values for putItemInput. */
        putItemInput.setItem(MinimalMap < AttributeValue > (itemArray, 5));
        putItemInput.setTableName(TABLE_NAME);

        /* perform putItem and check for errors. */
        PutItemOutput putItemOutput = ddbClient.putItem(putItemInput,
                actionError);
        switch (actionError) {
        case NONE_ACTIONERROR:
            Serial.println("PutItem succeeded!");
            break;
        case INVALID_REQUEST_ACTIONERROR:
            Serial.print("ERROR: ");
            Serial.println(putItemOutput.getErrorMessage().getCStr());
            break;
        case MISSING_REQUIRED_ARGS_ACTIONERROR:
            Serial.println(
                    "ERROR: Required arguments were not set for PutItemInput");
            break;
        case RESPONSE_PARSING_ACTIONERROR:
            Serial.println("ERROR: Problem parsing http response of PutItem");
            break;
        case CONNECTION_ACTIONERROR:
            Serial.println("ERROR: Connection problem");
            break;
        }
        /* wait to not double-record */
        delay(750);
    }

    if (prevGetSwitchState == HIGH && getSwitchState == LOW) {
        /* Set the string and number values for the range and hash Keys, respectively. */
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
                R = atoi(av.getN().getCStr());
                attributeMap.get("G", av);
                G = atoi(av.getN().getCStr());
                attributeMap.get("B", av);
                B = atoi(av.getN().getCStr());
                refreshColor();
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
