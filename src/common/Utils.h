/*
 * Utils.h
 *
 *  Utility functions for the Arduino AWS SDK.
 *
 *  Created on: Jun 30, 2014
 *      Author: hoffmaj
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include "jsmn.h"

extern const int SHA256_DEC_HASH_LEN;

/* Base encode 64 an array of characters. Returned array must be deleted by
 * caller. */
char *base64Encode(const char *inputBuffer);

/* Determine the number of digits in the base 10 representation a given
 * number. */
int digitCount(int i);

/* Create a string identical to the given string except with backslashes before
 * quotes. Caller must delete the returned string. */
char* escapeQuotes(const char* unescaped);

/* Determines the index of the opening and closing of a json string that is
 * surrounded by non-json, eg. http headers and newlines. Returns true if json
 * was found, false otherwise. */
bool findJsonStartEnd(const char* str, int* start, int* end);

/* Find and return the status code of an http response. */
int findHttpStatusCode(const char* str);

/* Given a char* of json array syntax ( e.g. ["a", "b", "c"]), count the number
 * of elements. */
int jsonArraySize(const char* jsonArrayStr, int jsonArrayStrLen);

/* Given a char* of json array syntax ( e.g. ["a", "b", "c"]) and the number of
 * elements, create a matching array of char*'s. Caller must delete the created
 * array AND each element. */
char** jsonArrayToStringArray(int numOfElements, const char* jsonArrayStr,
        int jsonArrayStrLen);

/* Determines whether a token is a json key, given the json string, the token's
 * end index, and the next token's start index. This includes keys of inner
 * json objects, i.e. both 'a' and 'b' would be a key in '{"a":{"b":1}}'. */
bool isKey(const char * json, int thisEnd, int nextStart);

/* Determines whether a token is a json key, given the json string, the token's
 * end index, and the next token's start index. This does not include keys of
 * inner json objects, i.e. 'a' would be a key in '{"a":{"b":1}}', but 'b'
 * would not. */
bool isOuterKey(const char * json, int thisEnd, int nextStart);

/* Given a json key, get a corresponding key. Takes the key, the json object
 * string, and the jsmn tokens. Returns 0 if key does not exist. If key does
 * exist, user must free the memory allocated to store the value. */
char* jsmnGetVal(const char* key, const char* json, jsmntok_t* tokens,
        int tokenCount);

/* Very minimal implementation for finding the current time from the message
 * returned by a signature failure. This works with at least Kinesis and
 * DynamoDB, in which the time is located within the 15 characters following
 * the first opening parenthesis. (e.g. "...(20140721T184435Z ..." )*/
char* getTimeFromInvalidSignatureMessage(const char* message);

/* Apply hmac to the key and message. */
char* hmacSha256(const char* key, int keyLen, const char* message,
        int messageLen);

#endif /* UTILS_H_ */
