/*
 * Utils.cpp
 *
 *  See Utils.h for description.
 *
 *  Created on: Jun 30, 2014
 *      Author: hoffmaj
 */

#include "Utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "jsmn.h"
#include "sha256.h"

/* Constants for base64Encode. */
static const char ENCODE_CHARS[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char PAD_CHAR = '=';

/* Constants for findHttpStatusCode. */
static const char HTTP_STATUS_CODE_PREFIX[] = "HTTP/1.1 ";
static const int HTTP_STATUS_CODE_LEN = 3;

/* Constants for hmacSha256. */
static const int BLOCK_SIZE = 64;
static const char OPAD = 0x5c;
static const char IPAD = 0x36;
const int SHA256_DEC_HASH_LEN = 32;

char *base64Encode(const char *toEncode) {
    int inLen = strlen(toEncode);
    /* For every three input chars there are 4 output chars, plus an extra 4
     * output chars for the possible 1 or 2 remaining input chars, plus an
     * extra byte for null termination. */
    size_t encodedLen = (((inLen / 3) + (inLen % 3 > 0)) * 4) + 1;

    char *encoded = new char[encodedLen]();
    int chunkIdx;
    char inChar1;
    char inChar2;
    char inChar3;
    int outIdx1;
    int outIdx2;
    int outIdx3;
    int outIdx4;

    for (chunkIdx = 0; chunkIdx < inLen / 3; chunkIdx++) {
        /* This approach of treating each character individually instead of
         * containing all bits in a long type allows the encoding to work on 8,
         * 16, 32, and 64 bit systems. */
        inChar1 = *toEncode++;
        inChar2 = *toEncode++;
        inChar3 = *toEncode++;

        outIdx1 = (inChar1 & 0xFC) >> 2;
        outIdx2 = ((inChar1 & 0x03) << 4) + ((inChar2 & 0xF0) >> 4);
        outIdx3 = ((inChar2 & 0x0F) << 2) + ((inChar3 & 0xC0) >> 6);
        outIdx4 = inChar3 & 0x3F;

        encoded[chunkIdx * 4] = ENCODE_CHARS[outIdx1];
        encoded[chunkIdx * 4 + 1] = ENCODE_CHARS[outIdx2];
        encoded[chunkIdx * 4 + 2] = ENCODE_CHARS[outIdx3];
        encoded[chunkIdx * 4 + 3] = ENCODE_CHARS[outIdx4];
    }

    switch (inLen % 3) {
    case 1:
        /* 1 extra input char -> 2 output chars and 2 padding chars. */

        inChar1 = *toEncode++;

        outIdx1 = (inChar1 & 0xFC) >> 2;
        outIdx2 = (inChar1 & 0x03) << 4;

        encoded[chunkIdx * 4] = ENCODE_CHARS[outIdx1];
        encoded[chunkIdx * 4 + 1] = ENCODE_CHARS[outIdx2];
        encoded[chunkIdx * 4 + 2] = PAD_CHAR;
        encoded[chunkIdx * 4 + 3] = PAD_CHAR;
        chunkIdx++;
        break;
    case 2:
        /* 2 extra input chars -> 3 output chars and 1 padding char. */

        inChar1 = *toEncode++;
        inChar2 = *toEncode++;
        outIdx1 = (inChar1 & 0xFC) >> 2;
        outIdx2 = ((inChar1 & 0x03) << 4) + ((inChar2 & 0xF0) >> 4);
        outIdx3 = ((inChar2 & 0x0F) << 2);
        encoded[chunkIdx * 4] = ENCODE_CHARS[outIdx1];
        encoded[chunkIdx * 4 + 1] = ENCODE_CHARS[outIdx2];
        encoded[chunkIdx * 4 + 2] = ENCODE_CHARS[outIdx3];
        encoded[chunkIdx * 4 + 3] = PAD_CHAR;
        chunkIdx++;
        break;
    }
    /* Ensure null termination. */
    encoded[chunkIdx * 4] = 0;

    return encoded;
}

int digitCount(int i) {
    int digits;
    for (digits = 0; i != 0; digits++)
        i /= 10;
    return digits;
}

char* escapeQuotes(const char* unescaped) {
    int unescapedLen = strlen(unescaped);

    /* Count quotes so that the amount of memory to be allocated can be
     * determined */
    int quoteCount = 0;
    for (int i = 0; i < unescapedLen; i++) {
        if (unescaped[i] == '\"') {
            quoteCount++;
        }
    }

    /* Copy ever character over, including a backslash before every quote. */
    char* escaped = new char[unescapedLen + quoteCount + 1]();
    int escapedWritten = 0;
    for (int i = 0; i < unescapedLen; i++) {
        if (unescaped[i] == '\"') {
            escaped[escapedWritten] = '\\';
            escapedWritten++;
        }
        escaped[escapedWritten] = unescaped[i];
        escapedWritten++;
    }
    return escaped;
}

bool findJsonStartEnd(const char* str, int* start, int* end) {
    /* Ignore everything before the first unquoted bracket and after the
     * unquoted bracket matching the first unquoted bracket, eg. headers and
     * newlines. */
    int strLen = strlen(str);
    /* Incrememented for every unquoted '{' and decremented for every
     * unquoted '}' */
    int braceBalance = 0;
    /* -1 is invalid start and end indices. */
    int s = -1;
    int e = -1;
    bool inQuotes = false;
    for (int i = 0; i < strLen; i++) {
        /* Toggle inQuotes. */
        if (str[i] == '\"') {
            inQuotes = !inQuotes;
        }
        /* Only consider brackets outside of quotes. */
        if (!inQuotes) {
            if (str[i] == '{') {
                if (s == -1) {
                    s = i;
                }
                braceBalance++;
            } else if (str[i] == '}') {
                braceBalance--;
                if (braceBalance == 0) {
                    e = i;
                    break;
                }
            }
        }
    }
    *start = s;
    *end = e;
    /* If not a full json string no success. */
    if ((s == -1) || (e == -1)) {
        return false;
    }
    return true;
}

int findHttpStatusCode(const char* str) {
    /* If the input is null OR the input is not long enough to contain the
     * error code OR the first characters of the input are not
     * HTTP_STATUS_CODE_PREFIX, return 0; */
    if (str == NULL
            || strlen(str)
                    < strlen(HTTP_STATUS_CODE_PREFIX) + HTTP_STATUS_CODE_LEN
            || strncmp(HTTP_STATUS_CODE_PREFIX, str,
                    strlen(HTTP_STATUS_CODE_PREFIX))) {
        return 0;
    }
    /* copy the error code string and convert it to an int. */
    char errorCodeStr[HTTP_STATUS_CODE_LEN + 1];
    strncpy(errorCodeStr, str + strlen(HTTP_STATUS_CODE_PREFIX),
            HTTP_STATUS_CODE_LEN);
    return atoi(errorCodeStr);
}

int jsonArraySize(const char* jsonArrayStr, int jsonArrayStrLen) {
    int elementCount = 0;
    bool inQuotes = false;
    if (jsonArrayStr[0] != '[' || jsonArrayStr[jsonArrayStrLen - 1] != ']') {
        /* Invalid syntax. */
        return -1;
    }
    for (int i = 1; i < jsonArrayStrLen - 1; i++) {
        if (jsonArrayStr[i] == '"' && jsonArrayStr[i - 1] != '\\') {
            if (inQuotes) {
                elementCount++;
            }
            inQuotes = !inQuotes;
        }
    }
    return elementCount;
}

char** jsonArrayToStringArray(int numOfElements, const char* jsonArrayStr,
        int jsonArrayStrLen) {
    char** strArray = new char*[numOfElements]();
    int start = -1;
    int elementCount = 0;
    bool inQuotes = false;
    if (jsonArrayStr[0] != '[' || jsonArrayStr[jsonArrayStrLen - 1] != ']') {
        /* Invalid syntax. */
        return 0;
    }
    for (int i = 1; i < jsonArrayStrLen - 1; i++) {
        if (jsonArrayStr[i] == '"' && jsonArrayStr[i - 1] != '\\') {
            if (inQuotes) {
                /* Delete values and return null if we find more elements than
                 * expected. */
                if (elementCount == numOfElements) {
                    for (int j = 0; j < numOfElements; j++) {
                        delete[] strArray[j];
                    }
                    delete[] strArray;
                    return 0;
                }

                char* str = new char[(i - start) + 1]();
                strncpy(str, jsonArrayStr + start, i - start);
                strArray[elementCount] = str;
                elementCount++;
            } else {
                start = i + 1;
            }
            inQuotes = !inQuotes;
        }
    }
    return strArray;
}

bool isKey(const char * json, int thisEnd, int nextStart) {
    /* Check the characters in between the two tokens. */
    for (int i = thisEnd; i < nextStart; i++) {
        if (json[i] == ':') {
            /* A ':' means the token on the left is a key. */
            return true;
        } else if (json[i] == ',') {
            /* A ',' means the token on the left is a value. */
            return false;
        }
    }
    return false;
}

bool isOuterKey(const char * json, int thisEnd, int nextStart) {
    /* Check if token is a key at all before checking if it is an inner key. */
    if (!isKey(json, thisEnd, nextStart)) {
        return false;
    }
    /* True when we are in a quoted section of a string. Ignore braces in a
     * quoted section. */
    bool inQuote = false;
    /* The number of opening braces encountered minus the number of closing
     * braces encountered. braceLevel greater than 1 means we are looking at
     * inner keys. */
    int braceLevel = 0;
    for (int i = 0; i < thisEnd; i++) {
        switch (json[i]) {
        case '"':
            /* Toggle inQuote. */
            inQuote = !inQuote;
            break;
        case '{':
            /* If not in quotes, increase braceLevel. */
            if (!inQuote) {
                braceLevel++;
            }
            break;
        case '}':
            /* If not in quotes, decrease braceLevel. */
            if (!inQuote) {
                braceLevel--;
            }
            break;
        }
    }
    /* if we are at brace level 1 upon reaching, the key is in the outermost
     * json object */
    return (braceLevel == 1);

}

char* jsmnGetVal(const char* key, const char* json, jsmntok_t* tokens,
        int tokenCount) {
    /* Look at all json tokens. */
    for (int i = 0; i < tokenCount - 1; i++) {
        /* Check if token is an outer key. */
        if (isOuterKey(json, tokens[i].end, tokens[i + 1].start)) {
            int currentKeyLen = tokens[i].end - tokens[i].start;
            int valueLen = tokens[i + 1].end - tokens[i + 1].start;
            /* Check if the key we are looking at is the key we are looking
             * for. */
            if (((int) strlen(key) == currentKeyLen)
                    && (strncmp(json + tokens[i].start, key, currentKeyLen) == 0)) {
                /* Copy and return the value */
                char* value = new char[valueLen + 1]();
                strncpy(value, json + tokens[i + 1].start, valueLen);
                return value;
            }
        }
    }
    /* Key was not found. */
    return NULL;
}

char* getTimeFromInvalidSignatureMessage(const char* message) {
    int messageLen = strlen(message);
    /* Iterate through each character in the string. */
    for (int i = 0; i < messageLen; i++) {
        /* If an opening parenthesis is found, copy the following 15
         * characters, excluding the 9th character which is a 'T'.*/
        if (message[i] == '(') {
            char* time = new char[15]();
            sprintf(time, "%.8s%.6s", message + i + 1, message + i + 10);
            return time;
        }
    }
    return 0;
}

char* hmacSha256(const char* key, int keyLen, const char* message,
        int messageLen) {
    SHA256* sha256 = new SHA256();
    /* The corrected key should be BLOCK_SIZE long. */
    char* correctedKey = new char[BLOCK_SIZE + 1]();
    /* If the key is greater than BLOCK_SIZE long, copy over its sha256 hash of
     * SHA256_DEC_HASH_LEN, leaving 0-padding to fill the entire BLOCK_SIZE. */
    if ((int) keyLen > BLOCK_SIZE) {
        sha256->reset();
        sha256->add(key, keyLen);
        char* hashedKey = sha256->getHashDec();
        memcpy(correctedKey, hashedKey, SHA256_DEC_HASH_LEN);
        delete[] hashedKey;
    }
    /* if the key is less than BLOCK_SIZE long, simply copy it over, leaving
     * 0-padding to fill the entire BLOCK_SIZE. */
    else {
        memcpy(correctedKey, key, keyLen);
    }

    /* Using an exclusive or with these OPAD and IPAD values to create the
     * iPadded and oPadded values specified by the HMAC algorithm. */
    char* oPadded = new char[BLOCK_SIZE + 1]();
    char* iPadded = new char[BLOCK_SIZE + 1]();
    for (int i = 0; i < BLOCK_SIZE; i++) {
        oPadded[i] = correctedKey[i] ^ OPAD;
        iPadded[i] = correctedKey[i] ^ IPAD;
    }

    delete[] correctedKey;

    /* Create the inner hash with the concatenation of iPadded and message. */
    sha256->reset();
    sha256->add(iPadded, BLOCK_SIZE);
    delete[] iPadded;
    sha256->add(message, messageLen);
    char* innerHash = sha256->getHashDec();

    /* Create the outer hash with the concatenation of oPadded and
     * innerhash. */
    sha256->reset();
    sha256->add(oPadded, BLOCK_SIZE);
    delete[] oPadded;
    sha256->add(innerHash, SHA256_DEC_HASH_LEN);
    delete[] innerHash;
    char* final = sha256->getHashDec();
    delete sha256;
    return final;
}

