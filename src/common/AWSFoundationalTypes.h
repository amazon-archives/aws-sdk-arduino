/* Arduino compatible c++ does not have any built-in map or list types. It
 * does have a string type that is ONLY available on Arduino. The types in this
 * file are designed to be substitutes for map/list/string that function on
 * any device. They are simple and do not cover some edge conditions and should
 * be used carefully. Any types provided to the generics here should have deep
 * copying for their assignment operator and copy constructor, e.g. no
 * "char*". Many also require jsonSerialize and jsonDeserialize methods. */

#ifndef AWSFOUNDATIONALTYPES_H_
#define AWSFOUNDATIONALTYPES_H_

#include <string.h>
#include <stdio.h>

/* Common enum for AWS actions. Are set/returned by actions depending on the
 * action's success. */
enum ActionError {
    NONE_ACTIONERROR,
    RESPONSE_PARSING_ACTIONERROR,
    INVALID_REQUEST_ACTIONERROR,
    MISSING_REQUIRED_ARGS_ACTIONERROR,
    CONNECTION_ACTIONERROR
};

/* A wrapper for c_strings that has a deep-copying equality operator and
 * constructor, as well as json serialization and deserialization
 * capabilities. */
class MinimalString {
    /* The wrapped c_string. */
    char* cStr;
    /* True if this string represents an object that is already serialized and
     * therefore does not need to be enclosed in quotes when jsonSerialize()
     * is called. */
    bool alreadySerialized;
    /* Copy the other MinimalString's c_string. */
    void innerCopy(const MinimalString &toCopy);
    /* Delete the c_string if it exists. */
    void innerDelete();
public:
    /* Copy the passed c_string. */
    MinimalString(const char* cStr);
    /* Copy the passed c_string. */
    MinimalString(const char* cStr, int len);
    /* Empty constructor, c_string is NULL. */
    MinimalString();
    /* Coppying constructor. */
    MinimalString(const MinimalString &toCopy);
    /* c_string getter. */
    const char* getCStr() const;
    /* Overload = for deep copying. */
    MinimalString& operator=(const MinimalString &toCopy);
    void setAlreadySerialized(bool alreadySerialized);
    bool getAlreadySerialized() const;
    /* Returns a quoted copy of the string. */
    MinimalString jsonSerialize() const;
    /* Unquotes the string if it has surrounding quotes. */
    bool jsonDeserialize(MinimalString json);
    /* Calculate the length of the string. */
    int length() const;
    ~MinimalString();
};

/* A wrapper for double objects that can has serialize/deserialize methods */
class SerializableDouble {
    /* The wrapped double. */
    double d;
public:
    SerializableDouble(double d);
    SerializableDouble();
    /* c_string getter. */
    double getValue() const;
    /* Returns the value's serialization. */
    MinimalString jsonSerialize() const;
    /* Sets the value to the json's deserialization. */
    bool jsonDeserialize(MinimalString json);
};

/* Contains an array of elements and has a deep-copying equality operator and
 * constructor. Element_Type must also have a deep-copying equality operator
 * and constructor for this to work properly. Also has json serialization and
 * deserialization capabilities*/
template<class Element_Type>
class MinimalList {
    /* The underylying array. */
    Element_Type* array;
    /* Number of elements in the array. */
    int length;
    /* Deep-copy an array and length. */
    void copyArray(const Element_Type* array, int length);
    /* Delete the array is it exists. */
    void innerDelete();
public:
    /* Getter for array. */
    const Element_Type* getArray() const;
    /* Getter for length. */
    int getLength() const;
    /* Constructor taking the array and length. */
    MinimalList(const Element_Type* array, int length);
    /* Deep-copying constructor. */
    MinimalList(const MinimalList<Element_Type>& toCopy);
    /* Empty constructor with null array. */
    MinimalList();
    /* Deep-copying equality operator. */
    MinimalList<Element_Type>& operator=(
            const MinimalList<Element_Type>& toCopy);
    /* Given a json syntax list of Element_Type, populate the MinimalList.
     * Element_Type must have a jsonDeserialize() method. */
    bool jsonDeserialize(MinimalString json);
    /* Create a json syntax list of the elements. Element_Type must have a
     * jsonSerialize() method. */
    MinimalString jsonSerialize() const;
    ~MinimalList();
};

/* Conatiner for a key-value pair, with json serialization/deserialization
 * capabilities. */
template<class Key_Type, class Value_Type>
class MinimalKeyValuePair {
    Key_Type key;
    Value_Type value;
public:
    /* Given a string of the json form for a key-value pair (i.e. key:value),
     * set key and value. Key_Type and Value_Type must each have a
     * jsonDeserialize() method. */
    bool jsonDeserialize(MinimalString json);
    /* Create a json key-value pair. Key_Type and Value_Type must each have a
     * jsonSerialize() method.  */
    MinimalString jsonSerialize() const;
    /* Get the key. */
    Key_Type getKey() const;
    /* Get the value. */
    Value_Type getValue() const;
    MinimalKeyValuePair(Key_Type key, Value_Type value);
    MinimalKeyValuePair();
};

/* A MinimalString to Value_Type map. This is designed to take a small amount
 * of code and memory, not to be fast. It is json
 * serializable/deserializable. */
template<class Value_Type>
class MinimalMap {
    MinimalList<MinimalKeyValuePair<MinimalString, Value_Type> > pairList;
public:
    /* Json serialize a map. Value_Type MUST have a jsonSerialize() method. */
    MinimalString jsonSerialize() const;
    /* Json deserialize a map. Value_Type MUST have a jsonDeserialize() method. */
    bool jsonDeserialize(MinimalString json);
    /* Constructor taking a list of MinimalString, Value_Type pairs. */
    MinimalMap(
            MinimalList<MinimalKeyValuePair<MinimalString, Value_Type> > pairList);
    /* Constructor taking a array of MinimalString, Value_Type pairs and it's
     * length. */
    MinimalMap(MinimalKeyValuePair<MinimalString, Value_Type>* pairArray,
            int length);
    /* Return the underlying list of KeyValuePairs. */
    MinimalList<MinimalKeyValuePair<MinimalString, Value_Type> > getPairList() const;
    MinimalMap();
    /* Get the value (passed by reference) for the given key (as a c_string
     * for usability purposes). */
    bool get(const char* key, Value_Type &value);
};

/* Takes an List of MinimalStrings, places a comma between each element and
 * surrounds all in brackets. */
MinimalString jsonCommaConcatenate(MinimalList<MinimalString> list,
        char openChar, char closeChar);

/* Create a list of strings for a string that starts openChar, ends with
 * closedChar, and has comma separated elements. */
MinimalList<MinimalString> jsonCommaSeparate(MinimalString jsonList,
        char openChar, char closeChar);

/* Get the json representation of a boolean */
MinimalString jsonSerializeBool(bool b);

/* Get the json representation of a long */
MinimalString jsonSerializeLong(long l);

/* Get the json representation of a int */
MinimalString jsonSerializeInt(int l);

/* Get the json representation of a boolean */
bool jsonDeserializeBool(MinimalString json);

/* Return the integer value of the enum deserialization of the passed json
 * string. enumLookup is an array of char* where each element is the json
 * serialization of a an enum, where the index of the lookup string is the same
 * as the integer value of the corresponding enum. */
int jsonDeserializeEnum(const char** enumLookup, int enumSize,
        MinimalString json);

/* Get the json representation of a double */
MinimalString jsonSerializeDouble(double d);

/*
 *
 * The following are the implementations for the template classes. Apparently
 * these are not allowed in the cpp file for templates.
 *
 */

template<class Element_Type>
void MinimalList<Element_Type>::copyArray(const Element_Type* array,
        int length) {
    this->length = length;
    this->array = new Element_Type[length]();
    for (int i = 0; i < length; i++) {
        this->array[i] = array[i];
    }
}

template<class Element_Type>
void MinimalList<Element_Type>::innerDelete() {
    if (array != NULL) {
        delete[] array;
    }
}

template<class Element_Type>
const Element_Type* MinimalList<Element_Type>::getArray() const {
    return array;
}

template<class Element_Type>
int MinimalList<Element_Type>::getLength() const {
    return length;
}

template<class Element_Type>
MinimalList<Element_Type>::MinimalList(const Element_Type* array, int length) {
    copyArray(array, length);
}

template<class Element_Type>
MinimalList<Element_Type>::MinimalList(
        const MinimalList<Element_Type>& toCopy) {
    copyArray(toCopy.getArray(), toCopy.getLength());
}

template<class Element_Type>
MinimalList<Element_Type>::MinimalList() {
    this->array = NULL;
    this->length = 0;
}

template<class Element_Type>
MinimalList<Element_Type>& MinimalList<Element_Type>::operator=(
        const MinimalList<Element_Type>& toCopy) {
    innerDelete();
    copyArray(toCopy.getArray(), toCopy.getLength());
    return *this;
}

template<class Element_Type>
bool MinimalList<Element_Type>::jsonDeserialize(MinimalString json) {
    innerDelete();
    /* Create a list of strings of the elements and get the underlying
     * array. */
    MinimalList<MinimalString> elementStrs = jsonCommaSeparate(json, '[', ']');
    const MinimalString* elementStrsArray = elementStrs.getArray();
    length = elementStrs.getLength();
    /* Set this list's underlying array to be the same size as the length of
     * array of elementStings, and serialize each element with the
     * elementString of the same index. */
    array = new Element_Type[length]();
    for (int i = 0; i < length; i++) {
        if (!array[i].jsonDeserialize(elementStrsArray[i])) {
            /* Propogate error. */
            return false;
        }
    }
    return true;
}

template<class Element_Type>
MinimalString MinimalList<Element_Type>::jsonSerialize() const {
    /* Create a list where each element is the serialization of the
     * Element_Type. */
    MinimalString* elementStrs = new MinimalString[length]();
    for (int i = 0; i < length; i++) {
        elementStrs[i] = array[i].jsonSerialize();
    }
    MinimalList<MinimalString> strList(elementStrs, length);
    delete[] elementStrs;
    /* Concatenate each string into a json-format list. */
    return jsonCommaConcatenate(strList, '[', ']');
}

template<class Element_Type>
MinimalList<Element_Type>::~MinimalList() {
    innerDelete();
}

template<class Key_Type, class Value_Type>
bool MinimalKeyValuePair<Key_Type, Value_Type>::jsonDeserialize(
        MinimalString json) {
    int jsonLen = json.length();
    const char* jsonCStr = json.getCStr();
    const char* colonLocation = strchr(jsonCStr, ':');
    /* If not containing a colon, use empty constructor. */
    if (colonLocation == NULL) {
        key = Key_Type();
        value = Value_Type();
    }
    int colonIdx = colonLocation - jsonCStr;
    /* Deserialize each substring on either side of the ':' in the json
     * string. */
    if (!key.jsonDeserialize(MinimalString(jsonCStr, colonIdx))
            || !value.jsonDeserialize(
                    MinimalString(colonLocation + 1, jsonLen - colonIdx))) {
        /* Propogate error. */
        return false;
    }
    return true;
}

template<class Key_Type, class Value_Type>
MinimalString MinimalKeyValuePair<Key_Type, Value_Type>::jsonSerialize() const {
    MinimalString keyStr = key.jsonSerialize();
    MinimalString valueStr = value.jsonSerialize();
    /* +1 for the ':' between them. */
    int serializedLen = keyStr.length() + valueStr.length() + 1;
    char* serialized = new char[serializedLen + 1]();
    sprintf(serialized, "%s:%s", keyStr.getCStr(), valueStr.getCStr());
    MinimalString jsonString(serialized);
    delete[] serialized;
    return jsonString;
}

template<class Key_Type, class Value_Type>
Key_Type MinimalKeyValuePair<Key_Type, Value_Type>::getKey() const {
    return key;
}

template<class Key_Type, class Value_Type>
Value_Type MinimalKeyValuePair<Key_Type, Value_Type>::getValue() const {
    return value;
}

template<class Key_Type, class Value_Type>
MinimalKeyValuePair<Key_Type, Value_Type>::MinimalKeyValuePair(Key_Type key,
        Value_Type value) {
    this->key = key;
    this->value = value;
}

template<class Key_Type, class Value_Type>
MinimalKeyValuePair<Key_Type, Value_Type>::MinimalKeyValuePair() {
}

template<class Value_Type>
MinimalString MinimalMap<Value_Type>::jsonSerialize() const {
    const MinimalKeyValuePair<MinimalString, Value_Type>* pairArray =
            pairList.getArray();
    int arrayLen = pairList.getLength();
    /* Create a string array and assign each element to the serialized form of
     * the element at the same index in pairArray. */
    MinimalString* serializedPairsArray = new MinimalString[arrayLen]();
    for (int i = 0; i < arrayLen; i++) {
        serializedPairsArray[i] = pairArray[i].jsonSerialize();
    }
    /* Create a list out of the serializedPairsArray, and create a braced,
     * comma-separated string out of the list. */
    MinimalList<MinimalString> serializedPairsList(serializedPairsArray,
            arrayLen);
    delete[] serializedPairsArray;
    return jsonCommaConcatenate(serializedPairsList, '{', '}');
}

template<class Value_Type>
bool MinimalMap<Value_Type>::jsonDeserialize(MinimalString json) {
    /* Create the list of serialized pairs. */
    MinimalList<MinimalString> strList = jsonCommaSeparate(json, '{', '}');
    const MinimalString* strArray = strList.getArray();
    int arrayLen = strList.getLength();
    /* Create a list of pairs the same size as the list of serialized pair
     * strings, then deserialize each element. */
    MinimalKeyValuePair<MinimalString, Value_Type>* deserializedPairsArray =
            new MinimalKeyValuePair<MinimalString, Value_Type> [arrayLen]();
    for (int i = 0; i < arrayLen; i++) {
        if (!deserializedPairsArray[i].jsonDeserialize(strArray[i])) {
            delete[] deserializedPairsArray;
            /* Propogate error. */
            return false;
        }
    }
    pairList = MinimalList<MinimalKeyValuePair<MinimalString, Value_Type> >(
            deserializedPairsArray, arrayLen);
    delete[] deserializedPairsArray;
    return true;
}

template<class Value_Type>
MinimalMap<Value_Type>::MinimalMap(
        MinimalList<MinimalKeyValuePair<MinimalString, Value_Type> > pairList) {
    this->pairList = pairList;
}

template<class Value_Type>
MinimalMap<Value_Type>::MinimalMap(
        MinimalKeyValuePair<MinimalString, Value_Type>* pairArray, int length) {
    /* Create a MinimalList out of the array. */
    this->pairList =
            MinimalList<MinimalKeyValuePair<MinimalString, Value_Type> >(
                    pairArray, length);
}

template<class Value_Type>
MinimalList<MinimalKeyValuePair<MinimalString, Value_Type> > MinimalMap<
        Value_Type>::getPairList() const {
    return pairList;
}

template<class Value_Type>
MinimalMap<Value_Type>::MinimalMap() {
}

template<class Value_Type>
bool MinimalMap<Value_Type>::get(const char* key, Value_Type &value) {
    /* Using a linear search, not hashing for this map. Program size and memory
     * usage are more important than speed on these microcontrollers. */
    for (int i = 0; i < pairList.getLength(); i++) {
        if (!strcmp(key, pairList.getArray()[i].getKey().getCStr())) {
            value = pairList.getArray()[i].getValue();
            return true;
        }
    }
    return false;
}

#endif /* AWSFOUNDATIONALTYPES_H_ */
