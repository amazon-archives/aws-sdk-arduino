/*
 * AWSClient.cpp
 *
 *  See AWSClient.h for description.
 *  See http://docs.aws.amazon.com/AmazonS3/latest/API/sig-v4-header-based-auth.html
 *
 */

#include "AWSClient4.h"
#include "Utils.h"
#include "DeviceIndependentInterfaces.h"
#include "AWSFoundationalTypes.h"
#include "sha256.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//
// /* Constants string, formats, and lengths. */
// static const char* CANONICAL_FORM_POST_LINE = "POST\n/\n\n";
// static const int CANONICAL_FORM_POST_LINE_LEN = 8;
// static const char* HTTPS_REQUEST_POST_LINE =
//         "POST https://%s/%s\n";
// // static const char* HTTPS_REQUEST_POST_LINE =
// //         "POST https://%s/%s HTTP/1.1\n";
// static const int HTTPS_REQUEST_POST_LINE_LEN = 19;
// static const char* HTTP_REQUEST_POST_LINE = "POST http://%s/%s HTTP/1.1\n";
// static const int HTTP_REQUEST_POST_LINE_LEN = 27;
// static const char* TO_SIGN_TEMPLATE =
//         "AWS4-HMAC-SHA256\n%sT%sZ\n%s/%s/%s/aws4_request\n%s";
// static const int TO_SIGN_TEMPLATE_LEN = 36;
// static const char* CONTENT_LENGTH_HEADER = "content-length:%d";
// static const int CONTENT_LENGTH_HEADER_LEN = 15;
// static const char* HOST_HEADER = "host:%s";
// static const int HOST_HEADER_LEN = 7;
// static const char* CONNECTION_HEADER = "Connection:close";
// static const int CONNECTION_HEADER_LEN = 16;
// static const char* CONTENT_TYPE_HEADER = "content-type:%s";
// static const int CONTENT_TYPE_HEADER_LEN = 13;
// static const char* X_AMZ_DATE_HEADER = "x-amz-date:%sT%sZ";
// static const int X_AMZ_DATE_HEADER_LEN = 13;
// static const char* AUTHORIZATION_HEADER =
//         "Authorization: AWS4-HMAC-SHA256 Credential=%s/%s/%s/%s/aws4_request, SignedHeaders=%s, Signature=%s";
// static const int AUTHORIZATION_HEADER_LEN = 87;
// static const char* SIGNED_HEADERS =
//         "content-length;content-type;host;x-amz-date";
// static const int SIGNED_HEADERS_LEN = 43;
//
AWSClient4::AWSClient4() {
    /* Null until set in init method. */
    awsRegion = 0;
    awsEndpoint = 0;
    awsSecKey = 0;
    awsKeyID = 0;
    httpClient = 0;
    dateTimeProvider = 0;
    method = 0;
    uri = 0;
}

void AWSClient4::setAWSRegion(const char * awsRegion) {
    int len = strlen(awsRegion) + 1;
    this->awsRegion = new char[len]();
    strcpy(this->awsRegion, awsRegion);
}
void AWSClient4::setAWSEndpoint(const char * awsEndpoint) {
    int len = strlen(awsEndpoint) + 1;
    this->awsEndpoint = new char[len]();
    strcpy(this->awsEndpoint, awsEndpoint);
}
void AWSClient4::setAWSDomain(const char * awsDomain) {
    int len = strlen(awsDomain) + 1;
    this->awsDomain = new char[len]();
    strcpy(this->awsDomain, awsDomain);
}
void AWSClient4::setAWSPath(const char * awsPath) {
    int len = strlen(awsPath) + 1;
    this->awsPath = new char[len]();
    strcpy(this->awsPath, awsPath);
}
void AWSClient4::setAWSSecretKey(const char * awsSecKey) {
    int len = strlen(awsSecKey) + 1;
    this->awsSecKey = new char[len]();
    strcpy(this->awsSecKey, awsSecKey);
}
void AWSClient4::setAWSKeyID(const char * awsKeyID) {
    int len = strlen(awsKeyID) + 1;
    this->awsKeyID = new char[len]();
    strcpy(this->awsKeyID, awsKeyID);
}
void AWSClient4::setHttpClient(IHttpClient* httpClient) {
    this->httpClient = httpClient;
}
void AWSClient4::setDateTimeProvider(IDateTimeProvider* dateTimeProvider) {
    this->dateTimeProvider = dateTimeProvider;
}

AWSClient4::~AWSClient4() {
    if (awsRegion != 0)
        delete[] awsRegion;
    if (awsEndpoint != 0)
        delete[] awsEndpoint;
    if (awsSecKey != 0)
        delete[] awsSecKey;
    if (awsKeyID != 0)
        delete[] awsKeyID;
}

char* AWSClient4::createHost() {
    // return "example.com";
    return awsDomain;
}

char* AWSClient4::createCanonicalHeaders() {
  // headers, alphabetically sorted, lowercase, eg: key:value
  // content-type:x
  // host:host
  // x-amz-content-sha256:hash
  // x-amz-date:date
  char canonical_headers[500] = "";
  sprintf(canonical_headers, "%scontent-type:%s\n", canonical_headers, contentType);
  sprintf(canonical_headers, "%shost:%s\n", canonical_headers, createHost());
  sprintf(canonical_headers, "%sx-amz-content-sha256:%s\n", canonical_headers, payloadHash);
  sprintf(canonical_headers, "%sx-amz-date:%sT%sZ\n", canonical_headers, awsDate, awsTime);
  return canonical_headers;
}

char* AWSClient4::createRequestHeaders(char* signature) {
  char headers[1000] = "";
  sprintf(headers, "%sContent-Type: %s\n", headers, contentType);
  sprintf(headers, "%sHost: %s\n", headers, createHost());
  sprintf(headers, "%sx-amz-content-sha256: %s\n", headers, payloadHash);
  sprintf(headers, "%sx-amz-date: %sT%sZ\n", headers, awsDate, awsTime);
  sprintf(headers, "%sAuthorization: AWS4-HMAC-SHA256 Credential=%s/%s/%s/%s/aws4_request,SignedHeaders=%s,Signature=%s\n", headers, awsKeyID, awsDate, awsRegion, awsService, signedHeaders, signature);
  return headers;
}

char* AWSClient4::createStringToSign(char* canonical_request) {
  char string_to_sign[700] = "AWS4-HMAC-SHA256\n";
  sprintf(string_to_sign, "%s%sT%sZ\n", canonical_request, awsDate, awsTime);
  sprintf(string_to_sign, "%s%s/%s/%s/aws4_request\n", canonical_request, awsDate, awsRegion, awsService);

  SHA256* sha256 = new SHA256();
  char* hashed = (*sha256)(canonical_request, strlen(canonical_request));
  delete sha256;

  sprintf(string_to_sign, "%s%s", canonical_request, hashed);

  return string_to_sign;
}

char* AWSClient4::createCanonicalRequest() {
  char canonical_request[700] = "";
  sprintf(canonical_request, "%s%s\n", canonical_request, method); // VERB
  sprintf(canonical_request, "%s%s\n", canonical_request, uri); // URI
  sprintf(canonical_request, "%s%s\n", canonical_request, queryString); // queryString

  char* headers = createCanonicalHeaders();

  sprintf(canonical_request, "%s%s", canonical_request, headers); // headers
  sprintf(canonical_request, "%s%s\n", canonical_request, signedHeaders); // signed_headers
  sprintf(canonical_request, "%s%s\n", canonical_request, payload.getCStr()); // payload

  return canonical_request;
}


char* AWSClient4::createSignature(const char* toSign) {

    /* Allocate memory for the signature */
    char* signature = new char[HASH_HEX_LEN4 + 1]();

    /* Create the signature key */
    /* + 4 for "AWS4" */
    int keyLen = strlen(awsSecKey) + 4;
    char* key = new char[keyLen + 1]();
    sprintf(key, "AWS4%s", awsSecKey);

    /* repeatedly apply hmac with the appropriate values. See
     * http://docs.aws.amazon.com/general/latest/gr/sigv4-calculate-signature.html
     * for algorithm. */
    char* k1 = hmacSha256(key, keyLen, awsDate, strlen(awsDate));
    delete[] key;
    char* k2 = hmacSha256(k1, SHA256_DEC_HASH_LEN, awsRegion,
            strlen(awsRegion));
    delete[] k1;
    char* k3 = hmacSha256(k2, SHA256_DEC_HASH_LEN, awsService,
            strlen(awsService));
    delete[] k2;
    char* k4 = hmacSha256(k3, SHA256_DEC_HASH_LEN, "aws4_request", 12);
    delete[] k3;
    char* k5 = hmacSha256(k4, SHA256_DEC_HASH_LEN, toSign, strlen(toSign));
    delete[] k4;

    /* Convert the chars in hash to hex for signature. */
    for (int i = 0; i < SHA256_DEC_HASH_LEN; ++i) {
        sprintf(signature + 2 * i, "%02lx", 0xff & (unsigned long) k5[i]);
    }
    delete[] k5;
    return signature;
}


char* AWSClient4::createRequest(MinimalString &reqPayload) {
    /* Check that all values have been initialized. */
    if (awsRegion == 0 || awsEndpoint == 0 || awsSecKey == 0 || awsKeyID == 0
            || httpClient == 0 || dateTimeProvider == 0)
        return 0;

    // set date and time
    // @TODO: find out why sprintf doesn't work
    const char* dateTime = dateTimeProvider->getDateTime();
    strncpy(awsDate, dateTime, 8);
    awsDate[9] = '\0';
    strncpy(awsTime, dateTime + 8, 6);
    awsTime[7] = '\0';

    SHA256* sha256 = new SHA256();
    payloadHash = (*sha256)(reqPayload.getCStr(), reqPayload.length());
    delete sha256;

    payload = reqPayload;

    char *canonical_request = createCanonicalRequest();
    char *string_to_sign = createStringToSign(canonical_request);
    char *signature = createSignature(string_to_sign);

    char *headers = createRequestHeaders(signature);

    char *host = createHost();
    char* request = new char[strlen(method) + strlen(host) + strlen(awsPath) + strlen(headers) + strlen(reqPayload.getCStr()) + 4]();
    sprintf(request, "%s %s%s\n%s\n%s", method, createHost(), awsPath, headers, reqPayload.getCStr());

    return request;

    // createRequestInit(reqPayload);
    // char* request = headersToRequest();
    // createRequestCleanup();

    // return request;
}

char* AWSClient4::sendData(const char* data) {
    char* server = createHostString();
    int port = httpS ? 443 : 80;
    char* response = httpClient->send(data, "A2MBBEONHC9LUG.iot.eu-west-1.amazonaws.com", port);
    delete[] server;
    return response;
}
