#include "AmazonS3Client.h"
#include "AWSFoundationalTypes.h"
#include <stdlib.h>
#include "Utils.h"

// stub for verifying if the signature actually works, needs to get some more love :)

AmazonS3Client::AmazonS3Client() : AWSClient4() {
    this->awsService = "s3";
    this->signedHeaders = "host;range;x-amz-content-sha256;x-amz-date";
    this->queryString = "";
    this->httpS = true;
}

char* AmazonS3Client::get(MinimalString uri, ActionError& actionError) {
    actionError = NONE_ACTIONERROR;

    this->method = "GET";
    setAWSPath(uri.getCStr());
    MinimalString foo = "";
    char* request = createRequest(foo);
    // char* response = sendData(request);
    return request;
}
