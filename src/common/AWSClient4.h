/*
 * AWSClient2.h
 *
 *  Base classes for services to create an HTTP request.
 *
 */

#ifndef AWSCLIENT4_H_
#define AWSCLIENT4_H_

#include "DeviceIndependentInterfaces.h"
#include "AWSFoundationalTypes.h"

/* Total number of headers. */
static const int HEADER_COUNT4 = 7;
/* Size of the awsDate string. */
static const int AWS_DATE_LEN4 = 8;
/* Size of the awsTime string. */
static const int AWS_TIME_LEN4 = 6;
/* Size of sha hashes and signatures in hexidecimal. */
static const int HASH_HEX_LEN4 = 64;

/* Base class for an AWS Service Client. Creates http and https request in raw
 * http format or as a curl command. */
class AWSClient4 {
    /* Name of region, eg. "us-east-1" in "kinesis.us-east-1.amazonaws.com". */
    char* awsRegion;
    /* Endpoint, eg. "amazonaws.com" in "kinesis.us-east-1.amazonaws.com". */
    char* awsEndpoint;
    /* Domain, optional, eg. "A2MBBEONHC9LUG.iot.us-east-1.amazonaws.com". */
    char* awsDomain;
    /* Path, optional eg. "/things/foobar/shadow", eg for iot-data. */
    char* awsPath;
    /* The user's AWS Secret Key for accessing the AWS Resource. */
    char* awsSecKey;
    /* The user's AWS Access Key ID for accessing the AWS Resource. */
    char* awsKeyID;
    /* GMT date in yyyyMMdd format. */
    char awsDate[AWS_DATE_LEN4 + 1];
    /* GMT time in HHmmss format. */
    char awsTime[AWS_TIME_LEN4 + 1];

    /* The payload of the httprequest to be created */
    MinimalString payload;

    // /* Add the headers that will be signed to the headers array. Called before
    //  * createStringToSign. */
    // void initSignedHeaders();
    // /* Create the canonical request and the string to sign as described. Return
    //  * value must be deleted by caller. */
    char* createStringToSign(char* canonical_request);
    // /* Given the string to sign, create the signature (a 64-char cstring).
    //  * Return value must be deleted by caller. */
    char* createSignature(const char* toSign);
    char* createRequestHeaders(char* signature);
    // /* Add the headers that will not be signed to the headers array. Called
    //  * after createSignature. */
    // void initUnsignedHeaders(const char* signature);
    // /* Contains all of the work to be done before headersToRequest or
    //  * headersToCurlRequest are called. Takes the payload to be sent and the
    //  * GMT date in yyyyMMddHHmmss format. */
    // void createRequestInit(MinimalString &reqPayload);
    // /* Clean up after headersToRequest or headersToCurlRequest are called. */
    // void createRequestCleanup();
    // /* Using the headers array, create a raw http request. */
    // char* headersToRequest(void);

protected:
    char* method;
    char* uri;
    char* queryString;
    char* headers;
    char* signedHeaders;
    char* payloadHash;

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
    // /* Generates the host based on subdomain, service, etc */
    char* createHost(void);
    /* Creates a raw http request, given the payload and current GMT date in
     * yyyyMMddHHmmss format. Should be exposed to user by extending class.
     * Returns 0 if client is unititialized. */
    char* createRequest(MinimalString &payload);
    char* createCanonicalRequest();
    char* createCanonicalHeaders();

    /* Sends http data. Returns http response, or null on error. */
    char* sendData(const char* data);
    /* Empty constructor. Must also be initialized with init. */
    AWSClient4();

public:
    /* Setters for values used by createRequest and createCurlRequest. Must
     * be set or create[Curl]Request will return null. */
    /* Generates the host based on subdomain, service, etc */
    char* createHostString(void);
    void setAWSRegion(const char * awsRegion);
    void setAWSEndpoint(const char * awsEndpoint);
    void setAWSDomain(const char * awsDomain);
    void setAWSPath(const char * awsPath);
    void setAWSSecretKey(const char * awsSecKey);
    void setAWSKeyID(const char * awsKeyID);
    void setHttpClient(IHttpClient* httpClient);
    void setDateTimeProvider(IDateTimeProvider* dateTimeProvider);
    ~AWSClient4(void);
};

#endif /* AWSCLIENT4_H_ */
