#ifndef AWS_HELPER_FUNCTIONS_H_
#define AWS_HELPER_FUNCTIONS_H_

#include <AmazonSNSClient.h>
#include <AmazonDynamoDBClient.h>
#include <AmazonKinesisClient.h>

enum AWS_Service_ID{
	KINESIS = 0,
	DYNAMO_DB = 1,
	SNS = 2,
	END_NOSERVICE = 3
};

void SNSClient_Setup();
void KinesisClient_Setup();
void DynamoBDClient_Setup();

void putDynamoDb();
void putKinesis(double realAccelX, double realAccelY, double realAccelZ);
void putSns();

void indicateServiceThroughLED_blink_Buzzer(AWS_Service_ID service);
AWS_Service_ID changeService(AWS_Service_ID currentServiceId);

#endif







































































