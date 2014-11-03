#include "AmazonSNSClient.h"
#include "AWSFoundationalTypes.h"
#include <stdlib.h>
#include "Utils.h"

static const char* SERVICE = "sns";
static const char* FORM_TYPE = "application/x-www-form-urlencoded";
static const char* PAYLOAD_TEMPLATE = "Action=Publish&TargetArn=%s&Message=%s&Version=2010-03-31\n";
int PAYLOAD_TEMPLATE_LENGTH = 54;
int MESSAGEID_BUFFER_LENGTH = 37;
int EXTRACTED_TIMESTAMP_BUFFER_LENGTH = 17;
int FORMATTED_TIMESTAMP_BUFFER_LENGTH = 15;

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
    char* payload = new char[PAYLOAD_TEMPLATE_LENGTH + message.length() + targetArn.length() + 1]();
	
    sprintf(payload, PAYLOAD_TEMPLATE, targetArn.getCStr(), message.getCStr());
	
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

AmazonSNSClient::AmazonSNSClient() : AWSClient2() {
    awsService = SERVICE;
}

PublishOutput AmazonSNSClient::publish(PublishInput publishInput, ActionError& actionError) {
    actionError = NONE_ACTIONERROR;

    PublishOutput publishOutput;
	
    if (!publishInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
	return publishOutput;
    }
	
    contentType = FORM_TYPE;
    MinimalString payload = publishInput.serialize();
	
    char* request = createRequest(payload);
    char* response = sendData(request);
    delete[] request;
    
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return publishOutput;
    }

    int httpStatusCode = findHttpStatusCode(response);
	
    if (httpStatusCode == 200) {
        char* msgidIdx = strstr(response, "<MessageId>");
        int msgidPos = msgidIdx - response;

        char* msgid = new char[MESSAGEID_BUFFER_LENGTH]();
        strncpy(msgid, response + msgidPos + 11, MESSAGEID_BUFFER_LENGTH - 1);
        msgid[37] = '\0';
		
        publishOutput.setMessageId(msgid);
        return publishOutput;
    }
    
    if (httpStatusCode == 403) {
        char* ts = strstr(response, "earlier than ");
        int pos = ts - response;
    	
        char* newts = new char[EXTRACTED_TIMESTAMP_BUFFER_LENGTH]();
        strncpy(newts, response + pos + 31, EXTRACTED_TIMESTAMP_BUFFER_LENGTH - 1);
        newts[16] = '\0';
    	
        char* time = new char[FORMATTED_TIMESTAMP_BUFFER_LENGTH]();
        sprintf(time, "%.8s%.6s", newts, newts + 9);
        dateTimeProvider->sync(time);
    }
    
    return publishOutput;
}
