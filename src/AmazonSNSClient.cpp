#include "AmazonSNSClient.h"
#include "AWSFoundationalTypes.h"
#include <stdlib.h>
#include <application.h>

PublishInput::PublishInput() {
	reset();
}

void PublishInput::reset() {
	targetArnBeenSet = false;
	messageBeenSet = false;
}

bool PublishInput::requiredAreSet() const {
	return targetArnBeenSet && messageBeenSet;
}

void PublishInput::setTargetArn(MinimalString targetArn) {
	targetArnBeenSet = true;
	this->targetArn = targetArn;
}

void PublishInput::setMessage(MinimalString message) {
	messageBeenSet = true;
	this->message = message;
}

MinimalString PublishInput::getTargetArn() const {
	return this->targetArn;
}

MinimalString PublishInput::getMessage() const {
	return this->message;
}

MinimalString PublishInput::serialize() const {
	char* payload;
	
	sprintf(payload, "Action=Publish&TargetArn=%s&Message=%s", targetArn.getCStr(), message.getCStr());
	
	return MinimalString(payload);
}

PublishOutput::PublishOutput() {
	reset();
}

void PublishOutput::reset() {
	messageIdBeenSet = false;
	
	errorType = MinimalString();
	errorMessage = MinimalString();
}

void PublishOutput::setMessageId(MinimalString messageId) {
	messageIdBeenSet = true;
	this->messageId = messageId;
}

MinimalString PublishOutput::getMessageId() const {
	return this->messageId;
}

MinimalString PublishOutput::getErrorType() const {
	return errorType;
}

MinimalString PublishOutput::getErrorMessage() const {
	return errorMessage;
}

AmazonSNSClient::AmazonSNSClient() : AWSClient() {
}

PublishOutput AmazonSNSClient::publish(PublishInput publishInput, ActionError& actionError) {
	actionError = NONE_ACTIONERROR;

	PublishOutput publishOutput;
	
	if (!publishInput.requiredAreSet()) {
		actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
		return publishOutput;
	}
	
	httpS = false;
	MinimalString payload = publishInput.serialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    Serial.println(request);
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return publishOutput;
    }	
    Serial.println(response);

	return publishOutput;
}