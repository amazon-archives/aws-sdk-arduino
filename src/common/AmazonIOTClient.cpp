#include "AmazonIOTClient.h"
#include "AWSFoundationalTypes.h"
#include <stdlib.h>
#include "Utils.h"

AmazonIOTClient::AmazonIOTClient() : AWSClient4() {
    this->awsService = "iotdata";
    this->contentType = "application/json";
    this->signedHeaders = "content-type;host;x-amz-content-sha256;x-amz-date";
    this->uri = "/";
    this->queryString = "";
    this->httpS = true;
}

char* AmazonIOTClient::update_shadow(MinimalString shadow, ActionError& actionError) {
    actionError = NONE_ACTIONERROR;

    this->method = "POST";
    char* request = createRequest(shadow);
    // return request;
    char* response = sendData(request);
    return response;
}
