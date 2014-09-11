#include "AWSFoundationalTypes.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char* FALSE_AS_JSON = "\"false\"";
static const char* TRUE_AS_JSON = "\"true\"";
static const int DOUBLE_STR_LEN = 10;
static const int LONG_STR_LEN = 10;
static const int INT_STR_LEN = 10;

void MinimalString::innerCopy(const MinimalString &toCopy) {
    alreadySerialized = toCopy.getAlreadySerialized();
    const char* toCopyCStr = toCopy.getCStr();
    if (toCopyCStr == NULL) {
        cStr = NULL;
    } else {
        cStr = new char[strlen(toCopyCStr) + 1]();
        strcpy(cStr, toCopyCStr);
    }
}

void MinimalString::innerDelete() {
    if (cStr != NULL) {
        delete[] cStr;
    }
}

MinimalString::MinimalString(const char* cStr) {
    alreadySerialized = false;
    this->cStr = new char[strlen(cStr) + 1]();
    strcpy(this->cStr, cStr);
}

MinimalString::MinimalString(const char* cStr, int len) {
    alreadySerialized = false;
    this->cStr = new char[len + 1]();
    strncpy(this->cStr, cStr, len);
}

MinimalString::MinimalString() {
    alreadySerialized = false;
    this->cStr = NULL;
}

MinimalString::MinimalString(const MinimalString &toCopy) {
    innerCopy(toCopy);
}

const char* MinimalString::getCStr() const {
    return cStr;
}

MinimalString& MinimalString::operator=(const MinimalString &toCopy) {
    /* Make sure we are not leaking memory by overwriting a value. */
    innerDelete();
    innerCopy(toCopy);
    return *this;
}

void MinimalString::setAlreadySerialized(bool alreadySerialized) {
    this->alreadySerialized = alreadySerialized;
}

bool MinimalString::getAlreadySerialized() const {
    return alreadySerialized;
}

MinimalString MinimalString::jsonSerialize() const {
    if (alreadySerialized) {
        return *this;
    }
    if (cStr == NULL) {
        /* Empty quoted string. */
        return MinimalString((char*) "\"\"");
    } else {
        /* +3 for the 2 quotes and the null termination character. */
        char* json = new char[strlen(cStr) + 3]();
        sprintf(json, "\"%s\"", cStr);
        MinimalString jsonMinStr(json);
        delete json;
        return jsonMinStr;
    }
}

bool MinimalString::jsonDeserialize(MinimalString json) {
    innerDelete();
    const char* jsonCStr = json.getCStr();
    if (jsonCStr == NULL) {
        return false;
    }
    int jsonLen = json.length();
    /* Only deserialize if the first and last characters are quotes. */
    if ((jsonCStr[0] == '"') && (jsonCStr[jsonLen - 1] == '"')) {
        /* -2 for the removed quote, but +1 for the null termination
         * character. */
        cStr = new char[jsonLen - 1]();
        /* Copy all but the first and last characters */
        strncpy(cStr, jsonCStr + 1, jsonLen - 2);
    } else {
        return false;
    }
    return true;
}

int MinimalString::length() const {
    if (cStr == NULL) {
        return 0;
    } else {
        return strlen(cStr);
    }
}

MinimalString::~MinimalString() {
    innerDelete();
}

SerializableDouble::SerializableDouble(double d) {
    this->d = d;
}
SerializableDouble::SerializableDouble() {
}

double SerializableDouble::getValue() const {
    return d;
}

MinimalString SerializableDouble::jsonSerialize() const {
    return jsonSerializeDouble(d);
}

bool SerializableDouble::jsonDeserialize(MinimalString json) {
    d = atof(json.getCStr());
    /* No error checking for this deserialize method, always return success. */
    return true;
}

MinimalString jsonCommaConcatenate(MinimalList<MinimalString> list,
        char openChar, char closeChar) {
    int arrayLen = list.getLength();
    const MinimalString* array = list.getArray();
    MinimalString* jsonSerializationsArray = new MinimalString[arrayLen]();
    /* Count the length of the string to allocate and create array of
     * serialized char*s:
     * There are arrayLen - 1 separating commas and 1 openChar and 1 closeChar.
     * fullSerializationLen = arrayLen - 1 + 2 + (lengths of all
     * json-serialized elements) = arraylen + 1 + (lengths of all
     * json-serialized elements). */
    int fullSerializationLen = arrayLen + 1;
    for (int i = 0; i < arrayLen; i++) {
        /* If quote, use teh jsonSerialized string, which simply puts quotes
         * around it. Otherwise use the string as is. */
        jsonSerializationsArray[i] = array[i];
        fullSerializationLen += jsonSerializationsArray[i].length();
    }
    /* Create the array-syntax serialization of all inner serializations. */
    char* fullSerialization = new char[fullSerializationLen + 1]();
    int fullSerializationWritten = 0;
    fullSerialization[fullSerializationWritten++] = openChar;
    for (int i = 0; i < arrayLen; i++) {
        /* Write the inner serialization to the full serialization and delete
         * the no longer needed inner serialization. */
        fullSerializationWritten += sprintf(
                fullSerialization + fullSerializationWritten, "%s",
                jsonSerializationsArray[i].getCStr());
        /* Comma after all but last element. */
        if (i != arrayLen - 1) {
            fullSerialization[fullSerializationWritten++] = ',';
        }
    }
    delete[] jsonSerializationsArray;
    fullSerialization[fullSerializationWritten++] = closeChar;
    MinimalString jsonString(fullSerialization);
    delete[] fullSerialization;
    return jsonString;
}

MinimalList<MinimalString> jsonCommaSeparate(MinimalString jsonList,
        char openChar, char closeChar) {
    /* Incemented for every opening bracket and decremented for every closing
     * bracket. */
    int bracketLevel = 0;
    /* Incemented for every opening brace and decremented for every closing
     * brace. */
    int braceLevel = 0;
    /* True when looking at characters within quotes. */
    bool inQuotes = false;
    int jsonLen = jsonList.length();

    if ((jsonLen <= 2) || (jsonList.getCStr()[0] != openChar)
            || (jsonList.getCStr()[jsonLen - 1] != closeChar)) {
        /* Missing open/close character or empty list, return empty list. */
        MinimalList<MinimalString> emptyList(0, 0);
        return emptyList;
    }

    /* Number of elements in the list, determined as the number of
     * unquoted/unbraced/unbracketed commas found plus one. */
    int elementCount = 1;
    /* This for loop determines elementCount. */
    for (int i = 1; i < jsonLen - 1; i++) {
        switch (jsonList.getCStr()[i]) {
        case '"':
            /* Toggle inQuotes. */
            inQuotes = !inQuotes;
            break;
            /* increment and decrement for unquoted brackets and braces. */
        case '[':
            if (!inQuotes) {
                bracketLevel++;
            }
            break;
        case ']':
            if (!inQuotes) {
                bracketLevel--;
            }
            break;
        case '{':
            if (!inQuotes) {
                braceLevel++;
            }
            break;
        case '}':
            if (!inQuotes) {
                braceLevel--;
            }
            break;
        case ',':
            /* If we are at the opening level, and unquoted, and have
             * encountered a comma, it is a element separator. */
            if ((braceLevel == 0) && (bracketLevel == 0) && !inQuotes) {
                elementCount++;
            }
        }
    }
    /* Array of elements. */
    MinimalString* elements = new MinimalString[elementCount]();
    /* Number of elements created / next index of elements array to write
     * to. */
    int elementsCreated = 0;
    /* The start index for the current element being coppied. */
    int elementStartIdx = 1;
    /* This for loop fills the elements array. */
    for (int i = 1; i < jsonLen - 1; i++) {
        switch (jsonList.getCStr()[i]) {
        case '"':
            /* Toggle inQuotes. */
            inQuotes = !inQuotes;
            break;
            /* increment and decrement for unquoted brackets and braces. */
        case '[':
            if (!inQuotes) {
                bracketLevel++;
            }
            break;
        case ']':
            if (!inQuotes) {
                bracketLevel--;
            }
            break;
        case '{':
            if (!inQuotes) {
                braceLevel++;
            }
            break;
        case '}':
            if (!inQuotes) {
                braceLevel--;
            }
            break;
        case ',':
            /* If we are at the opening level, and unquoted, and have
             * encountered a comma, it is a element separator. */
            if ((braceLevel == 0) && (bracketLevel == 0) && !inQuotes) {
                /* Copy over the string from the elementStartIdx (inclusive) to
                 * i (exclusive). */
                elements[elementsCreated] = MinimalString(
                        jsonList.getCStr() + elementStartIdx,
                        i - elementStartIdx);

                elementsCreated++;
                /* The start of the next element is the index after the index
                 * of this comma. */
                elementStartIdx = i + 1;
            }
        }
    }
    /* Copy over the last element, which does not end with a comma. */
    elements[elementsCreated] = MinimalString(
            jsonList.getCStr() + elementStartIdx,
            (jsonList.length() - 1) - elementStartIdx);

    MinimalList<MinimalString> elementsList(elements, elementCount);
    delete[] elements;
    return elementsList;
}

MinimalString jsonSerializeBool(bool b) {
    return b ? MinimalString(TRUE_AS_JSON) : MinimalString(FALSE_AS_JSON);
}

bool jsonDeserializeBool(MinimalString json) {
    return !strcmp("true", json.getCStr());
}

int jsonDeserializeEnum(const char** enumLookup, int enumSize,
        MinimalString json) {
    MinimalString serialized = json.jsonSerialize();
    const char* expected = serialized.getCStr();
    for (int i = 0; i < enumSize; i++) {
        if (!strcmp(enumLookup[i], expected)) {
            return i;
        }
    }
    return -1;
}

MinimalString jsonSerializeDouble(double d) {
    char buffer[DOUBLE_STR_LEN + 1];
    snprintf(buffer, DOUBLE_STR_LEN, "%f", d);
    return MinimalString(buffer);
}

MinimalString jsonSerializeLong(long l) {
    char buffer[LONG_STR_LEN + 1];
    snprintf(buffer, LONG_STR_LEN, "%ld", l);
    return MinimalString(buffer);
}

MinimalString jsonSerializeInt(int i) {
    char buffer[INT_STR_LEN + 1];
    snprintf(buffer, INT_STR_LEN, "%d", i);
    return MinimalString(buffer);
}

