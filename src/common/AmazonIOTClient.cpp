#include "AmazonIOTClient.h"
#include "AWSFoundationalTypes.h"
#include <stdlib.h>
#include "Utils.h"

static const char* SERVICE = "iotdata";
static const char* FORM_TYPE = "application/json";
static const char* PAYLOAD_TEMPLATE = "%s";
// int PAYLOAD_TEMPLATE_LENGTH = 2;
int IOT_EXTRACTED_TIMESTAMP_BUFFER_LENGTH = 17;
int IOT_FORMATTED_TIMESTAMP_BUFFER_LENGTH = 15;

AmazonIOTClient::AmazonIOTClient() : AWSClient2() {
    awsService = SERVICE;
    httpS = true;
}

char* AmazonIOTClient::update_shadow(MinimalString url, MinimalString shadow, ActionError& actionError) {

    actionError = NONE_ACTIONERROR;
    contentType = FORM_TYPE;

    // char* request = createRequest(shadow, url);
    // publishOutput.setResponse(request);

    // char* request = createRequest(url, shadow);
    char* request = createRequest(shadow);
    return request;

    // char* response = sendData(request);
    // delete[] request;
    //
    // if (response == NULL) {
    //     actionError = CONNECTION_ACTIONERROR;
    //     return 999;
    // }
    //
    // int httpStatusCode = findHttpStatusCode(response);
    //
    // if (httpStatusCode == 200) {
    //   return 1;
    // }
    //
    // if (httpStatusCode == 403) {
    //     char* ts = strstr(response, "earlier than ");
    //     int pos = ts - response;
    //
    //     char* newts = new char[IOT_EXTRACTED_TIMESTAMP_BUFFER_LENGTH]();
    //     strncpy(newts, response + pos + 31, IOT_EXTRACTED_TIMESTAMP_BUFFER_LENGTH - 1);
    //     newts[16] = '\0';
    //
    //     char* time = new char[IOT_FORMATTED_TIMESTAMP_BUFFER_LENGTH]();
    //     sprintf(time, "%.8s%.6s", newts, newts + 9);
    //     dateTimeProvider->sync(time);
    //     return 0;
    // }
    //
    // return httpStatusCode;
}
