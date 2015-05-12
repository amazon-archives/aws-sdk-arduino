/*
 * AWSClient.cpp
 *
 *  See AWSClient.h for description.
 *
 *  Created on: Jul 8, 2014
 *      Author: hoffmaj
 */

#include "AWSClient.h"
#include "Utils.h"
#include "DeviceIndependentInterfaces.h"
#include "AWSFoundationalTypes.h"
#include "sha256.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Constants string, formats, and lengths. */
static const char* CANONICAL_FORM_POST_LINE = "POST\n/\n\n";
static const int CANONICAL_FORM_POST_LINE_LEN = 8;
static const char* HTTPS_REQUEST_POST_LINE =
        "POST https://%s.%s.%s/ HTTP/1.1\n";
static const int HTTPS_REQUEST_POST_LINE_LEN = 28;
static const char* HTTP_REQUEST_POST_LINE = "POST http://%s.%s.%s/ HTTP/1.1\n";
static const int HTTP_REQUEST_POST_LINE_LEN = 27;
static const char* CURL_START = "curl --silent -X POST ";
static const int CURL_START_LEN = 22;
static const char* HTTPS_CURL_END = "https://%s.%s.%s";
static const int HTTPS_CURL_END_LEN = 10;
static const char* HTTP_CURL_END = "http://%s.%s.%s";
static const int HTTP_CURL_END_LEN = 9;
static const char* TO_SIGN_TEMPLATE =
        "AWS4-HMAC-SHA256\n%sT%sZ\n%s/%s/%s/aws4_request\n%s";
static const int TO_SIGN_TEMPLATE_LEN = 36;
static const char* CONTENT_LENGTH_HEADER = "content-length:%d";
static const int CONTENT_LENGTH_HEADER_LEN = 15;
static const char* HOST_HEADER = "host:%s.%s.%s";
static const int HOST_HEADER_LEN = 7;
static const char* CONNECTION_HEADER = "Connection:close";
static const int CONNECTION_HEADER_LEN = 16;
static const char* CONTENT_TYPE_HEADER = "content-type:%s";
static const int CONTENT_TYPE_HEADER_LEN = 13;
static const char* X_AMZ_DATE_HEADER = "x-amz-date:%sT%sZ";
static const int X_AMZ_DATE_HEADER_LEN = 13;
static const char* X_AMZ_TARGET_HEADER = "x-amz-target:%s";
static const int X_AMZ_TARGET_HEADER_LEN = 13;
static const char* AUTHORIZATION_HEADER =
        "Authorization: AWS4-HMAC-SHA256 Credential=%s/%s/%s/%s/aws4_request, SignedHeaders=%s, Signature=%s";
static const int AUTHORIZATION_HEADER_LEN = 87;
static const char* SIGNED_HEADERS =
        "content-length;content-type;host;x-amz-date;x-amz-target";
static const int SIGNED_HEADERS_LEN = 56;

AWSClient::AWSClient() {
    /* Null until set in init method. */
    awsRegion = 0;
    awsEndpoint = 0;
    awsSecKey = 0;
    awsKeyID = 0;
    httpClient = 0;
    dateTimeProvider = 0;
}

void AWSClient::setAWSRegion(const char * awsRegion) {
    int len = strlen(awsRegion) + 1;
    this->awsRegion = new char[len]();
    strcpy(this->awsRegion, awsRegion);
}
void AWSClient::setAWSEndpoint(const char * awsEndpoint) {
    int len = strlen(awsEndpoint) + 1;
    this->awsEndpoint = new char[len]();
    strcpy(this->awsEndpoint, awsEndpoint);
}
void AWSClient::setAWSSecretKey(const char * awsSecKey) {
    int len = strlen(awsSecKey) + 1;
    this->awsSecKey = new char[len]();
    strcpy(this->awsSecKey, awsSecKey);
}
void AWSClient::setAWSKeyID(const char * awsKeyID) {
    int len = strlen(awsKeyID) + 1;
    this->awsKeyID = new char[len]();
    strcpy(this->awsKeyID, awsKeyID);
}
void AWSClient::setHttpClient(IHttpClient* httpClient) {
    this->httpClient = httpClient;
}
void AWSClient::setDateTimeProvider(IDateTimeProvider* dateTimeProvider) {
    this->dateTimeProvider = dateTimeProvider;
}

AWSClient::~AWSClient() {
    if (awsRegion != 0)
        delete[] awsRegion;
    if (awsEndpoint != 0)
        delete[] awsEndpoint;
    if (awsSecKey != 0)
        delete[] awsSecKey;
    if (awsKeyID != 0)
        delete[] awsKeyID;
}

void AWSClient::initSignedHeaders() {
    /* For each of the formats for unsigned headers, determine the size of the
     * formatted string, allocate that much space in the next available element
     * in the headers array, create the string, and add it's length to the
     * headerLens array. */

    int contentLen = payload.length();
    int len = CONTENT_LENGTH_HEADER_LEN + digitCount(contentLen);
    headers[headersCreated] = new char[len + 1]();
    sprintf(headers[headersCreated], CONTENT_LENGTH_HEADER, contentLen);
    headerLens[headersCreated++] = len;

    len = CONTENT_TYPE_HEADER_LEN + strlen(contentType);
    headers[headersCreated] = new char[len + 1]();
    sprintf(headers[headersCreated], CONTENT_TYPE_HEADER, contentType);
    headerLens[headersCreated++] = len;

    len = HOST_HEADER_LEN + strlen(awsService) + strlen(awsRegion)
            + strlen(awsEndpoint);
    headers[headersCreated] = new char[len + 1]();
    sprintf(headers[headersCreated], HOST_HEADER, awsService, awsRegion,
            awsEndpoint);
    headerLens[headersCreated++] = len;

    len = X_AMZ_DATE_HEADER_LEN + AWS_DATE_LEN + AWS_TIME_LEN;
    headers[headersCreated] = new char[len + 1]();
    sprintf(headers[headersCreated], X_AMZ_DATE_HEADER, awsDate, awsTime);
    headerLens[headersCreated++] = len;

    len = X_AMZ_TARGET_HEADER_LEN + strlen(target);
    headers[headersCreated] = new char[len + 1]();
    sprintf(headers[headersCreated], X_AMZ_TARGET_HEADER, target);
    headerLens[headersCreated++] = len;
}

char* AWSClient::createStringToSign(void) {
    SHA256* sha256 = new SHA256();
    char* hashed;
    /* Calculate length of canonicalForm string. */
    int canonicalFormLen = CANONICAL_FORM_POST_LINE_LEN;
    for (int i = 0; i < headersCreated; i++) {
        /* +1 for newlines */
        canonicalFormLen += *(headerLens + i) + 1;
    }
    /* +2 for newlines. */
    canonicalFormLen += SIGNED_HEADERS_LEN + HASH_HEX_LEN + 2;

    char* canonicalForm = new char[canonicalFormLen + 1]();

    /* Write the cannonicalForm string. */
    int canonicalFormWritten = 0;
    canonicalFormWritten += strlen(
            strcpy(canonicalForm + canonicalFormWritten,
                    CANONICAL_FORM_POST_LINE));
    for (int i = 0; i < headersCreated; i++) {
        canonicalFormWritten += sprintf(canonicalForm + canonicalFormWritten,
                "%s\n", *(headers + i));
    }
    canonicalFormWritten += sprintf(canonicalForm + canonicalFormWritten,
            "\n%s\n", SIGNED_HEADERS);
    hashed = (*sha256)(payload.getCStr(), payload.length());
    strcpy(canonicalForm + canonicalFormWritten, hashed);
    delete[] hashed;
    canonicalFormWritten += HASH_HEX_LEN;

    /* Hash the canonicalForm string. */
    hashed = (*sha256)(canonicalForm, canonicalFormWritten);
    delete sha256;

    delete[] canonicalForm;

    /* Determine the size to the string to sign. */
    int toSignLen = TO_SIGN_TEMPLATE_LEN + 2 * AWS_DATE_LEN + AWS_TIME_LEN
            + strlen(awsRegion) + strlen(awsService) + HASH_HEX_LEN;

    /* Create and return the string to sign. */
    char* toSign = new char[toSignLen + 1]();
    sprintf(toSign, TO_SIGN_TEMPLATE, awsDate, awsTime, awsDate, awsRegion,
            awsService, hashed);
    delete[] hashed;
    return toSign;

}
char* AWSClient::createSignature(const char* toSign) {

    /* Allocate memory for the signature */
    char* signature = new char[HASH_HEX_LEN + 1]();

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

void AWSClient::initUnsignedHeaders(const char* signature) {
    int len = AUTHORIZATION_HEADER_LEN + strlen(awsKeyID) + AWS_DATE_LEN
            + strlen(awsRegion) + strlen(awsService) + SIGNED_HEADERS_LEN
            + HASH_HEX_LEN;
    headers[headersCreated] = new char[len + 1]();
    sprintf(headers[headersCreated], AUTHORIZATION_HEADER, awsKeyID, awsDate,
            awsRegion, awsService, SIGNED_HEADERS, signature);
    headerLens[headersCreated++] = len;

    len = CONNECTION_HEADER_LEN;
    headers[headersCreated] = new char[len + 1]();
    strcpy(headers[headersCreated], CONNECTION_HEADER);
    headerLens[headersCreated++] = len;
}

void AWSClient::createRequestInit(MinimalString &reqPayload) {
    //initialize object-scoped variables
    const char* dateTime = dateTimeProvider->getDateTime();
    sprintf(awsDate, "%.8s", dateTime);
    sprintf(awsTime, "%.6s", dateTime + 8);
    payload = reqPayload;
    headersCreated = 0;

    //Create signature and headers
    initSignedHeaders();
    char* toSign = createStringToSign();
    char* signature = createSignature(toSign);
    delete[] toSign;
    initUnsignedHeaders(signature);
    delete[] signature;
}

void AWSClient::createRequestCleanup() {
    /* Free each header */
    for (int i = 0; i < headersCreated; i++) {
        delete[] headers[i];
    }
}

char* AWSClient::headersToRequest() {
    /* Determine whether to use https or http postLine values. */
    int postLineLen =
            httpS ? HTTPS_REQUEST_POST_LINE_LEN : HTTP_REQUEST_POST_LINE_LEN;
    const char* postLine =
            httpS ? HTTPS_REQUEST_POST_LINE : HTTP_REQUEST_POST_LINE;

    /* Calculate length of httpRequest string. */
    int httpRequestLen = postLineLen + strlen(awsService) + strlen(awsRegion)
            + strlen(awsEndpoint);
    for (int i = 0; i < headersCreated; i++) {
        /* +1 for newline. */
        httpRequestLen += *(headerLens + i) + 1;
    }
    /* +1 for newline. */
    httpRequestLen += payload.length() + 1;

    /* Create and write to the httpRequest string. */
    char* httpRequest = new char[httpRequestLen + 1]();
    int httpRequestWritten = 0;
    httpRequestWritten += sprintf(httpRequest + httpRequestWritten, postLine,
            awsService, awsRegion, awsEndpoint);
    for (int i = 0; i < headersCreated; i++) {
        httpRequestWritten += sprintf(httpRequest + httpRequestWritten, "%s\n",
                *(headers + i));
    }
    httpRequestWritten += sprintf(httpRequest + httpRequestWritten, "\n%s",
            payload.getCStr());

    return httpRequest;
}

char* AWSClient::headersToCurlRequest() {
    /* Add backslashes before quotes in json string */
    char* escapedPayload = escapeQuotes(payload.getCStr());

    /* Determine whether to use https or http curlEnd values. */
    int curlEndLen = httpS ? HTTPS_CURL_END_LEN : HTTP_CURL_END_LEN;
    const char* curlEnd = httpS ? HTTPS_CURL_END : HTTP_CURL_END;

    /* Calculate length of curl command. +6 for "-d", 2 spaces, 2 quotes. */
    int httpRequestLen = CURL_START_LEN + curlEndLen + strlen(awsService)
            + strlen(awsRegion) + strlen(awsEndpoint) + strlen(escapedPayload)
            + 6;
    for (int i = 0; i < headersCreated; i++) {
        /* +6 for "-H", 2 spaces, 2 quotes */
        httpRequestLen += *(headerLens + i) + 6;
    }

    /* Create and write to the httpRequest string. */
    char* httpRequest = new char[httpRequestLen + 1]();
    int httpRequestWritten = 0;
    httpRequestWritten += strlen(strcpy(httpRequest, CURL_START));
    for (int i = 0; i < headersCreated; i++) {
        httpRequestWritten += sprintf(httpRequest + httpRequestWritten,
                "-H \"%s\" ", *(headers + i));
    }
    httpRequestWritten += sprintf(httpRequest + httpRequestWritten,
            "-d \"%s\" ", escapedPayload);
    delete[] escapedPayload;
    httpRequestWritten += sprintf(httpRequest + httpRequestWritten, curlEnd,
            awsService, awsRegion, awsEndpoint);

    return httpRequest;
}

char* AWSClient::createRequest(MinimalString &reqPayload) {
    /* Check that all values have been initialized. */
    if (awsRegion == 0 || awsEndpoint == 0 || awsSecKey == 0 || awsKeyID == 0
            || httpClient == 0 || dateTimeProvider == 0)
        return 0;
    createRequestInit(reqPayload);
    char* request = headersToRequest();
    createRequestCleanup();
    return request;
}

char* AWSClient::createCurlRequest(MinimalString &reqPayload) {
    /* Check that all values have been initialized. */
    if (awsRegion == 0 || awsEndpoint == 0 || awsSecKey == 0 || awsKeyID == 0
            || httpClient == 0 || dateTimeProvider == 0)
        return 0;
    createRequestInit(reqPayload);
    char* request = headersToCurlRequest();
    createRequestCleanup();
    return request;
}

char* AWSClient::sendData(const char* data) {
    char* server = new char[strlen(awsService) + strlen(awsRegion)
            + strlen(awsEndpoint) + 4]();
    sprintf(server, "%s.%s.%s", awsService, awsRegion, awsEndpoint);
    int port = httpS ? 443 : 80;
    char* response = httpClient->send(data, server, port);
    delete[] server;
    return response;
}
