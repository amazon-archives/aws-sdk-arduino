/*
 * AWSClient.h
 *
 *  Base classes for services to create an HTTP request.
 *
 *  Created on: Jul 8, 2014
 *      Author: hoffmaj
 */

#ifndef AWSCLIENT_H_
#define AWSCLIENT_H_

#include "DeviceIndependentInterfaces.h"
#include "AWSFoundationalTypes.h"

/* Total number of headers. */
static const int HEADER_COUNT = 7;
/* Size of the awsDate string. */
static const int AWS_DATE_LEN = 8;
/* Size of the awsTime string. */
static const int AWS_TIME_LEN = 6;
/* Size of sha hashes and signatures in hexidecimal. */
static const int HASH_HEX_LEN = 64;

/* Base class for an AWS Service Client. Creates http and https request in raw
 * http format or as a curl command. */
class AWSClient {
    /* Name of region, eg. "us-east-1" in "kinesis.us-east-1.amazonaws.com". */
    char* awsRegion;
    /* Endpoint, eg. "amazonaws.com" in "kinesis.us-east-1.amazonaws.com". */
    char* awsEndpoint;
    /* The user's AWS Secret Key for accessing the AWS Resource. */
    char* awsSecKey;
    /* The user's AWS Access Key ID for accessing the AWS Resource. */
    char* awsKeyID;
    /* GMT date in yyyyMMdd format. */
    char awsDate[AWS_DATE_LEN + 1];
    /* GMT time in HHmmss format. */
    char awsTime[AWS_TIME_LEN + 1];
    /* Number of headers created. */
    int headersCreated;
    /* Array of the created http headers. */
    char* headers[HEADER_COUNT];
    /* Array of string lengths of the headers in the "headers" array. */
    int headerLens[HEADER_COUNT];
    /* The payload of the httprequest to be created */
    MinimalString payload;

    /* Add the headers that will be signed to the headers array. Called before
     * createStringToSign. */
    void initSignedHeaders();
    /* Create the canonical request and the string to sign as described. Return
     * value must be deleted by caller. */
    char* createStringToSign(void);
    /* Given the string to sign, create the signature (a 64-char cstring).
     * Return value must be deleted by caller. */
    char* createSignature(const char* toSign);
    /* Add the headers that will not be signed to the headers array. Called
     * after createSignature. */
    void initUnsignedHeaders(const char* signature);
    /* Contains all of the work to be done before headersToRequest or
     * headersToCurlRequest are called. Takes the payload to be sent and the
     * GMT date in yyyyMMddHHmmss format. */
    void createRequestInit(MinimalString &reqPayload);
    /* Clean up after headersToRequest or headersToCurlRequest are called. */
    void createRequestCleanup();
    /* Using the headers array, create a raw http request. */
    char* headersToRequest(void);
    /* Using the headers array, create a http request in curl command
     * format. */
    char* headersToCurlRequest(void);

protected:
    /* Used to keep track of time. */
    IDateTimeProvider* dateTimeProvider;
    /* Used to send http to the server. */
    IHttpClient* httpClient;
    /* true if https is to be used, false if http is to be used. */
    bool httpS;
    /* Name of service, eg. "kinesis" in "kinesis.us-east-1.amazonaws.com". */
    const char* awsService;
    /* Content type of payload, eg. "application/x-amz-json-1.1". */
    const char* contentType;
    /* The request's target, i.e. "Kinesis_20131202.PutRecord". */
    const char* target;
    /* Creates a raw http request, given the payload and current GMT date in
     * yyyyMMddHHmmss format. Should be exposed to user by extending class.
     * Returns 0 if client is unititialized. */
    char* createRequest(MinimalString &payload);
    /* Creates a http request in curl format, given the payload and current GMT
     * date in yyyyMMddHHmmss format. Should be exposed to user by extending
     * class. Returns 0 if client is unititialized. */
    char* createCurlRequest(MinimalString &payload);
    /* Sends http data. Returns http response, or null on error. */
    char* sendData(const char* data);
    /* Empty constructor. Must also be initialized with init. */
    AWSClient();

public:
    /* Setters for values used by createRequest and createCurlRequest. Must
     * be set or create[Curl]Request will return null. */
    void setAWSRegion(const char * awsRegion);
    void setAWSEndpoint(const char * awsEndpoint);
    void setAWSSecretKey(const char * awsSecKey);
    void setAWSKeyID(const char * awsKeyID);
    void setHttpClient(IHttpClient* httpClient);
    void setDateTimeProvider(IDateTimeProvider* dateTimeProvider);
    ~AWSClient(void);
};

#endif /* AWSCLIENT_H_ */
