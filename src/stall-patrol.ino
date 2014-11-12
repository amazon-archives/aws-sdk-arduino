#include "AmazonDynamoDBClient.h"
#include "SparkAWSImplementations.h"
#include "AWSFoundationalTypes.h"
#include "keys.h"
#include "SparkButton.h"

/*
 *
 * This sample uses PutItem on a DynamoDB to record every time a button is
 * pressed (or a tilt switch is tilted if using one instead of a button).
 *
 * For this demo to work you must have keys.h/.ccp files that contain your AWS
 * access keys and define "awsSecKey" and "awsKeyID", a DynamoDB table with the
 * name defined by the constant TABLE_NAME with hash and range keys as defined
 * by constants HASH_KEY_NAME/RANGE_KEY_NAME.
 *
 */

/* D2 is the pin connected to the tilt switch. For buttons or tilt switches,
 * have the pin connected to one side of the button and also connected to
 * ground with a resistor. Have the other side of the button connected to
 * 5v/3v. */
const int SWITCH_PIN = D0;

/* Contants describing DynamoDB table and values being used. */
static const char* HASH_KEY_NAME = "stall_id";
static const char* RANGE_KEY_NAME = "time";


// static const char* HASH_KEY_VALUE = "fakestall-01";

const char* TABLE_NAME = "hackday-2014-team6-stalls";
/* Constants for connecting to DynamoDB. */
const char* AWS_REGION = "us-east-1";
const char* AWS_ENDPOINT = "amazonaws.com";

/* 0 or 1 to determine state of the switch */
int motionState = 0;

/* Device independent implementations required for AmazonDynamoDBClient to
 * function. */
SparkHttpClient httpClient;
SparkDateTimeProvider dateTimeProvider;

AmazonDynamoDBClient ddbClient;

PutItemInput putItemInput;
ActionError actionError;

// Spark button
SparkButton button = SparkButton();

void setup() {
    /* Begin serial communication. */
    Serial.begin(9600);
    /* Initialize ddbClient. */
    ddbClient.setAWSRegion(AWS_REGION);
    ddbClient.setAWSEndpoint(AWS_ENDPOINT);
    ddbClient.setAWSSecretKey(awsSecKey);
    ddbClient.setAWSKeyID(awsKeyID);
    ddbClient.setHttpClient(&httpClient);
    ddbClient.setDateTimeProvider(&dateTimeProvider);
    /* Open SWITCH_PIN to be read from. */
    pinMode(SWITCH_PIN, INPUT);

    //Register our Spark function here.
    Spark.function("alert", alertControl);
    button.begin();
}

void loop() {
    int prevMotionState = motionState;
    /* Read the state of the tilt switch. */
    motionState = digitalRead(SWITCH_PIN);
    
    Serial.print("motion state");
    Serial.println(motionState);
    
    /* If state has changed back to low a button press (or a full tilt if using
     * a tilt switch) has passed */
    if (false && prevMotionState == HIGH && motionState == LOW) {
        /* Create an Item. */
        AttributeValue deviceValue;
        deviceValue.setS("fakestall-01");
        
        /* Getting current time for Time attribute. */
        AttributeValue timeValue;
        timeValue.setS(dateTimeProvider.getDateTime());
        
        MinimalKeyValuePair <MinimalString, AttributeValue> att1(HASH_KEY_NAME, deviceValue);
        MinimalKeyValuePair <MinimalString, AttributeValue> att2(RANGE_KEY_NAME, timeValue);
        MinimalKeyValuePair <MinimalString, AttributeValue> itemArray[] = {att1, att2};

        /* Set values for putItemInput. */
        putItemInput.setItem(MinimalMap <AttributeValue> (itemArray, 2));
        putItemInput.setTableName(TABLE_NAME);

        /* perform putItem and check for errors. */
        Serial.println("performing put");
        PutItemOutput putItemOutput = ddbClient.putItem(putItemInput, actionError);
        
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
        delay(2000);
    }
    delay(500);
}

int alertControl(String command)
{
        // Flash Rainbow LEDs three times to indicate we are ready
    for (int i = 1; i<=3; i++) {
            button.rainbow(3);
            button.allLedsOff();
            delay(500);
    }
    return 1;
}

