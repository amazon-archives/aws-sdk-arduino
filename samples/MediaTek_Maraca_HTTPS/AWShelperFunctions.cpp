#include <MtkAWSImplementations.h>
#include "AWShelperFunctions.h"
#include "HardwareFunctions.h"
#include "keys.h"
#include <Utils.h>

AmazonSNSClient snsClient;
AmazonDynamoDBClient ddbClient;
AmazonKinesisClient kClient;

/* Device independent implementations required for AmazonDynamoDBClient to
 * function. */
MtkHttpClient httpClient;
//kinesis
MtkHttpCurlClient httpCurlClient;
MtkDateTimeProvider dateTimeProvider;

PublishInput publishInput;
GetItemInput getItemInput;
PutItemInput putItemInput;
AttributeValue hashKey;
AttributeValue rangeKey;
ActionError actionError;

void SNSClient_Setup() {
	snsClient.setAWSRegion(AWS_REGION);
	snsClient.setAWSEndpoint(AWS_ENDPOINT);
	snsClient.setAWSSecretKey(awsSecKey);
	snsClient.setAWSKeyID(awsKeyID);
	snsClient.setHttpClient(&httpClient);
	snsClient.setDateTimeProvider(&dateTimeProvider);
}

void DynamoBDClient_Setup() {
	ddbClient.setAWSRegion(AWS_REGION);
	ddbClient.setAWSEndpoint(AWS_ENDPOINT);
	ddbClient.setAWSSecretKey(awsSecKey);
	ddbClient.setAWSKeyID(awsKeyID);
	ddbClient.setHttpClient(&httpClient);
	ddbClient.setDateTimeProvider(&dateTimeProvider);
}

void KinesisClient_Setup() {
	kClient.setAWSRegion(AWS_REGION);
	kClient.setAWSEndpoint(AWS_ENDPOINT);
	kClient.setAWSSecretKey(awsSecKey);
	kClient.setAWSKeyID(awsKeyID);
	kClient.setHttpClient(&httpClient);
	kClient.setDateTimeProvider(&dateTimeProvider);
}

void putDynamoDb() {
	// Put device & datestamp record in DynamoDB table
	// Create an Item.
	AttributeValue deviceIdValue;
	deviceIdValue.setS(HASH_KEY_VALUE);
	AttributeValue timeValue;
	// Getting current time for Time attribute.
	timeValue.setN(dateTimeProvider.getDateTime());
	AttributeValue deviceValue;
	deviceValue.setS(ATTRIBUTE_VALUE);

	MinimalKeyValuePair<MinimalString, AttributeValue> att1(HASH_KEY_NAME, deviceIdValue);
	MinimalKeyValuePair<MinimalString, AttributeValue> att2(RANGE_KEY_NAME, timeValue);
	MinimalKeyValuePair<MinimalString, AttributeValue> att3(ATTRIBUTE_NAME, deviceValue);
	MinimalKeyValuePair<MinimalString, AttributeValue> itemArray[] = { att1,
			att2, att3 };

	// Set values for putItemInput.
	putItemInput.setItem(MinimalMap<AttributeValue>(itemArray, 3));
	putItemInput.setTableName(TABLE_NAME);

	// perform putItem and check for errors.
	PutItemOutput putItemOutput = ddbClient.putItem(putItemInput, actionError);
	switch (actionError) {
	case NONE_ACTIONERROR:
		Serial.println("DynamoDB PutItem succeeded!");
		break;
	case INVALID_REQUEST_ACTIONERROR:
		Serial.print("ERROR: ");
		Serial.println(putItemOutput.getErrorMessage().getCStr());
		break;
	case MISSING_REQUIRED_ARGS_ACTIONERROR:
		Serial.println("ERROR: Required arguments were not set for PutItemInput");
		break;
	case RESPONSE_PARSING_ACTIONERROR:
		Serial.println("ERROR: Problem parsing http response of PutItem");
		break;
	case CONNECTION_ACTIONERROR:
		Serial.println("ERROR: Connection problem");
		break;
	}
}

void putKinesis(double realAccelX, double realAccelY, double realAccelZ) {
// light the grove LED to indicate posting to AWS service

	PutRecordInput putRecordInput;
	putRecordInput.setStreamName(streamName);

	char buffer[255];
	String dataSource;
	dataSource = String("{\"device_id\":\"");
	dataSource += HASH_KEY_VALUE;
	dataSource += String("\",\"time\":");
	dataSource += dateTimeProvider.getDateTime();
	dataSource += String(",\"device\":\"");
        dataSource += ATTRIBUTE_VALUE;
        dataSource += String("\",\"sensors\":[{\"telemetryData\": {\"xval\":");
	sprintf(buffer, "%f", realAccelX);
	dataSource += buffer;
	dataSource += String(",\"yval\":");
	sprintf(buffer, "%f", realAccelY);
	dataSource += buffer;
	dataSource += String(",\"zval\":");
	sprintf(buffer, "%f", realAccelZ);
	dataSource += buffer;
	dataSource += String("}}]}");
	dataSource.toCharArray(buffer, 255);
	Serial.println(buffer);
        Serial.println(dateTimeProvider.getDateTime());
	char* data = base64Encode(buffer);
	putRecordInput.setData(data);
	delete[] data;
	putRecordInput.setPartitionKey(partitionKey);

// simple blind putRecord, no way to read output of System call for now
	PutRecordOutput putRecordOutput = kClient.putRecord(putRecordInput, actionError);

switch (actionError) {
	case NONE_ACTIONERROR:
		Serial.println("Kinesis record posted to Stream");
                Serial.println("Sequence ID: ");
                Serial.println(putRecordOutput.getSequenceNumber().getCStr());
		break;
	case INVALID_REQUEST_ACTIONERROR:
		Serial.print("ERROR: ");
		Serial.println(putRecordOutput.getErrorMessage().getCStr());
		break;
	case MISSING_REQUIRED_ARGS_ACTIONERROR:
		Serial.println("ERROR: Required arguments were not set for PutRecordInput");
		break;
	case RESPONSE_PARSING_ACTIONERROR:
		Serial.println("ERROR: Problem parsing http response of PutRecord");
		break;
	case CONNECTION_ACTIONERROR:
		Serial.println("ERROR: Connection problem");
		break;
	}
}

void putSns() {

// set datetime as the message to TARGET_ARN
	MinimalString dateTime(dateTimeProvider.getDateTime());
	dateTimeProvider.sync(dateTime.getCStr());

// Delivery attempt to Amazon SNS.
	publishInput.setMessage(dateTime);
	publishInput.setTargetArn(TARGET_ARN);
	PublishOutput publishOutput = snsClient.publish(publishInput, actionError);
	Serial.print("Sent message ID: ");
	Serial.println(publishOutput.getMessageId().getCStr());
}



void indicateServiceThroughLED_blink_Buzzer(AWS_Service_ID service) {

	int blinkCount = 0;

	switch (service) {

	case KINESIS:
		blinkCount = 1;
		break;

	case DYNAMO_DB:
		blinkCount = 2;
		break;

	case SNS:
		blinkCount = 3;
		break;

	default:
		blinkCount = 10;
		break;
	}
	blinkGroveLed(blinkCount);

}


AWS_Service_ID changeService(AWS_Service_ID currentServiceId) {
	AWS_Service_ID nextServiceId;

	Serial.print("Button pressed - changing mode. New mode: ");

	nextServiceId = (AWS_Service_ID)(currentServiceId + 1);
	if (nextServiceId >= END_NOSERVICE) {
		nextServiceId = KINESIS;
	}

	indicateServiceThroughLED_blink_Buzzer(nextServiceId);

	switch (nextServiceId) {
	case 0:
		Serial.println("Kinesis");
		break;
	case 1:
		Serial.println("DynamoDB");
		break;
	case 2:
		Serial.println("SNS");
		break;
	}
	return nextServiceId;
}


































































