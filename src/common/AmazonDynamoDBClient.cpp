#include "AmazonDynamoDBClient.h"
#include "AWSFoundationalTypes.h"
#include "jsmn.h"
#include "Utils.h"
#include <stdlib.h>

static const char* SERVICE = "dynamodb";
static const char* JSON_TYPE = "application/x-amz-json-1.0";
static const char* BATCHGETITEM_TARGET = "DynamoDB_20120810.BatchGetItem";
static const char* BATCHWRITEITEM_TARGET = "DynamoDB_20120810.BatchWriteItem";
static const char* CREATETABLE_TARGET = "DynamoDB_20120810.CreateTable";
static const char* DELETEITEM_TARGET = "DynamoDB_20120810.DeleteItem";
static const char* DELETETABLE_TARGET = "DynamoDB_20120810.DeleteTable";
static const char* DESCRIBETABLE_TARGET = "DynamoDB_20120810.DescribeTable";
static const char* GETITEM_TARGET = "DynamoDB_20120810.GetItem";
static const char* LISTTABLES_TARGET = "DynamoDB_20120810.ListTables";
static const char* PUTITEM_TARGET = "DynamoDB_20120810.PutItem";
static const char* QUERY_TARGET = "DynamoDB_20120810.Query";
static const char* SCAN_TARGET = "DynamoDB_20120810.Scan";
static const char* UPDATEITEM_TARGET = "DynamoDB_20120810.UpdateItem";
static const char* UPDATETABLE_TARGET = "DynamoDB_20120810.UpdateTable";
static const char* DYNAMODB_ERROR_TYPE_KEY = "__type";
static const char* DYNAMODB_ERROR_MESSAGE_KEY = "message";
static const char* DYNAMODB_SIGNATURE_ERROR_TYPE = "com.amazon.coral.service#InvalidSignatureException";

static const char* ATTRIBUTEVALUELIST_KEY = "AttributeValueList";
static const char* COMPARISONOPERATOR_KEY = "ComparisonOperator";
static const char* ATTRIBUTES_KEY = "Attributes";
static const char* ITEMCOLLECTIONMETRICS_KEY = "ItemCollectionMetrics";
static const char* CONSUMEDCAPACITY_KEY = "ConsumedCapacity";
static const char* TABLEDESCRIPTION_KEY = "TableDescription";
static const char* DELETEREQUEST_KEY = "DeleteRequest";
static const char* PUTREQUEST_KEY = "PutRequest";
static const char* PROVISIONEDTHROUGHPUT_KEY = "ProvisionedThroughput";
static const char* INDEXNAME_KEY = "IndexName";
static const char* ATTRIBUTETYPE_KEY = "AttributeType";
static const char* ATTRIBUTENAME_KEY = "AttributeName";
static const char* REQUESTITEMS_KEY = "RequestItems";
static const char* RETURNCONSUMEDCAPACITY_KEY = "ReturnConsumedCapacity";
static const char* KEY_KEY = "Key";
static const char* TABLENAME_KEY = "TableName";
static const char* VALUE_KEY = "Value";
static const char* ACTION_KEY = "Action";
static const char* GLOBALSECONDARYINDEXES_KEY = "GlobalSecondaryIndexes";
static const char* LOCALSECONDARYINDEXES_KEY = "LocalSecondaryIndexes";
static const char* KEYSCHEMA_KEY = "KeySchema";
static const char* ATTRIBUTEDEFINITIONS_KEY = "AttributeDefinitions";
static const char* WRITECAPACITYUNITS_KEY = "WriteCapacityUnits";
static const char* READCAPACITYUNITS_KEY = "ReadCapacityUnits";
static const char* RESPONSES_KEY = "Responses";
static const char* UNPROCESSEDKEYS_KEY = "UnprocessedKeys";
static const char* RETURNITEMCOLLECTIONMETRICS_KEY = "ReturnItemCollectionMetrics";
static const char* SCANINDEXFORWARD_KEY = "ScanIndexForward";
static const char* SELECT_KEY = "Select";
static const char* CONSISTENTREAD_KEY = "ConsistentRead";
static const char* CONDITIONALOPERATOR_KEY = "ConditionalOperator";
static const char* QUERYFILTER_KEY = "QueryFilter";
static const char* ATTRIBUTESTOGET_KEY = "AttributesToGet";
static const char* KEYCONDITIONS_KEY = "KeyConditions";
static const char* LIMIT_KEY = "Limit";
static const char* EXCLUSIVESTARTKEY_KEY = "ExclusiveStartKey";
static const char* CAPACITYUNITS_KEY = "CapacityUnits";
static const char* TABLE_KEY = "Table";
static const char* EXCLUSIVESTARTTABLENAME_KEY = "ExclusiveStartTableName";
static const char* UPDATE_KEY = "Update";
static const char* EXISTS_KEY = "Exists";
static const char* PROJECTIONTYPE_KEY = "ProjectionType";
static const char* NONKEYATTRIBUTES_KEY = "NonKeyAttributes";
static const char* PROJECTION_KEY = "Projection";
static const char* LASTEVALUATEDTABLENAME_KEY = "LastEvaluatedTableName";
static const char* TABLENAMES_KEY = "TableNames";
static const char* SIZEESTIMATERANGEGB_KEY = "SizeEstimateRangeGB";
static const char* ITEMCOLLECTIONKEY_KEY = "ItemCollectionKey";
static const char* ITEM_KEY = "Item";
static const char* GLOBALSECONDARYINDEXUPDATES_KEY = "GlobalSecondaryIndexUpdates";
static const char* RETURNVALUES_KEY = "ReturnValues";
static const char* EXPECTED_KEY = "Expected";
static const char* ATTRIBUTEUPDATES_KEY = "AttributeUpdates";
static const char* LASTEVALUATEDKEY_KEY = "LastEvaluatedKey";
static const char* ITEMS_KEY = "Items";
static const char* COUNT_KEY = "Count";
static const char* SCANNEDCOUNT_KEY = "ScannedCount";
static const char* INDEXSTATUS_KEY = "IndexStatus";
static const char* ITEMCOUNT_KEY = "ItemCount";
static const char* INDEXSIZEBYTES_KEY = "IndexSizeBytes";
static const char* NUMBEROFDECREASESTODAY_KEY = "NumberOfDecreasesToday";
static const char* LASTINCREASEDATETIME_KEY = "LastIncreaseDateTime";
static const char* LASTDECREASEDATETIME_KEY = "LastDecreaseDateTime";
static const char* SS_KEY = "SS";
static const char* BS_KEY = "BS";
static const char* B_KEY = "B";
static const char* S_KEY = "S";
static const char* NS_KEY = "NS";
static const char* N_KEY = "N";
static const char* UNPROCESSEDITEMS_KEY = "UnprocessedItems";
static const char* SCANFILTER_KEY = "ScanFilter";
static const char* TOTALSEGMENTS_KEY = "TotalSegments";
static const char* SEGMENT_KEY = "Segment";
static const char* KEYTYPE_KEY = "KeyType";
static const char* CREATIONDATETIME_KEY = "CreationDateTime";
static const char* TABLESTATUS_KEY = "TableStatus";
static const char* TABLESIZEBYTES_KEY = "TableSizeBytes";
static const char* KEYS_KEY = "Keys";

const char* RETURNVALUE_LOOKUP[] = {
    "\"NONE\"",
    "\"ALL_OLD\"",
    "\"UPDATED_OLD\"",
    "\"ALL_NEW\"",
    "\"UPDATED_NEW\""
};

const char* CONDITIONALOPERATOR_LOOKUP[] = {
    "\"AND\"",
    "\"OR\""
};

const char* TABLESTATUS_LOOKUP[] = {
    "\"CREATING\"",
    "\"UPDATING\"",
    "\"DELETING\"",
    "\"ACTIVE\""
};

const char* RETURNCONSUMEDCAPACITY_LOOKUP[] = {
    "\"INDEXES\"",
    "\"TOTAL\"",
    "\"NONE\""
};

const char* PROJECTIONTYPE_LOOKUP[] = {
    "\"ALL\"",
    "\"KEYS_ONLY\"",
    "\"INCLUDE\""
};

const char* COMPARISONOPERATOR_LOOKUP[] = {
    "\"EQ\"",
    "\"NE\"",
    "\"IN\"",
    "\"LE\"",
    "\"LT\"",
    "\"GE\"",
    "\"GT\"",
    "\"BETWEEN\"",
    "\"NOT_NULL\"",
    "\"NULL\"",
    "\"CONTAINS\"",
    "\"NOT_CONTAINS\"",
    "\"BEGINS_WITH\""
};

const char* RETURNITEMCOLLECTIONMETRICS_LOOKUP[] = {
    "\"SIZE\"",
    "\"NONE\""
};

const char* SCALARATTRIBUTETYPE_LOOKUP[] = {
    "\"S\"",
    "\"N\"",
    "\"B\""
};

const char* INDEXSTATUS_LOOKUP[] = {
    "\"CREATING\"",
    "\"UPDATING\"",
    "\"DELETING\"",
    "\"ACTIVE\""
};

const char* SELECT_LOOKUP[] = {
    "\"ALL_ATTRIBUTES\"",
    "\"ALL_PROJECTED_ATTRIBUTES\"",
    "\"SPECIFIC_ATTRIBUTES\"",
    "\"COUNT\""
};

const char* KEYTYPE_LOOKUP[] = {
    "\"HASH\"",
    "\"RANGE\""
};

const char* ATTRIBUTEACTION_LOOKUP[] = {
    "\"ADD\"",
    "\"PUT\"",
    "\"DELETE\""
};

KeySchemaElement::KeySchemaElement() {
    reset();
}

void KeySchemaElement::reset() {
    keyTypeBeenSet = false;
    attributeNameBeenSet = false;
}

bool KeySchemaElement::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedKeyType = jsmnGetVal(KEYTYPE_KEY, jsonCStr, tokens, tCount);
    char* serializedAttributeName = jsmnGetVal(ATTRIBUTENAME_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedKeyType != NULL) {
        keyTypeBeenSet = true;
        int enumInt = jsonDeserializeEnum(KEYTYPE_LOOKUP, 2, MinimalString(serializedKeyType));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedAttributeName != NULL) {
        attributeNameBeenSet = true;
        attributeName = MinimalString(serializedAttributeName);
    }
    if (serializedKeyType != NULL) {
        delete[] serializedKeyType;
    }
    if (serializedAttributeName != NULL) {
        delete[] serializedAttributeName;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString KeySchemaElement::jsonSerialize() const {
    int itemCount = 0;
    if (keyTypeBeenSet) {
        itemCount++;
    }
    if (attributeNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (keyTypeBeenSet) {
        MinimalString pairValue = MinimalString(KEYTYPE_LOOKUP[keyType]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYTYPE_KEY, pairValue);
    }
    if (attributeNameBeenSet) {
        MinimalString pairValue = attributeName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTENAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void KeySchemaElement::setKeyType(KeyType keyType) {
    keyTypeBeenSet = true;
    this->keyType = keyType;
}

void KeySchemaElement::setAttributeName(MinimalString attributeName) {
    attributeNameBeenSet = true;
    this->attributeName = attributeName;
}

KeyType KeySchemaElement::getKeyType() const {
    return keyType;
}

MinimalString KeySchemaElement::getAttributeName() const {
    return attributeName;
}

Projection::Projection() {
    reset();
}

void Projection::reset() {
    projectionTypeBeenSet = false;
    nonKeyAttributesBeenSet = false;
}

bool Projection::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedProjectionType = jsmnGetVal(PROJECTIONTYPE_KEY, jsonCStr, tokens, tCount);
    char* serializedNonKeyAttributes = jsmnGetVal(NONKEYATTRIBUTES_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedProjectionType != NULL) {
        projectionTypeBeenSet = true;
        int enumInt = jsonDeserializeEnum(PROJECTIONTYPE_LOOKUP, 3, MinimalString(serializedProjectionType));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedNonKeyAttributes != NULL) {
        nonKeyAttributesBeenSet = true;
        if (!nonKeyAttributes.jsonDeserialize(serializedNonKeyAttributes)) {
            toReturn = false;
        }
    }
    if (serializedProjectionType != NULL) {
        delete[] serializedProjectionType;
    }
    if (serializedNonKeyAttributes != NULL) {
        delete[] serializedNonKeyAttributes;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString Projection::jsonSerialize() const {
    int itemCount = 0;
    if (projectionTypeBeenSet) {
        itemCount++;
    }
    if (nonKeyAttributesBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (projectionTypeBeenSet) {
        MinimalString pairValue = MinimalString(PROJECTIONTYPE_LOOKUP[projectionType]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROJECTIONTYPE_KEY, pairValue);
    }
    if (nonKeyAttributesBeenSet) {
        MinimalString pairValue = nonKeyAttributes.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(NONKEYATTRIBUTES_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void Projection::setProjectionType(ProjectionType projectionType) {
    projectionTypeBeenSet = true;
    this->projectionType = projectionType;
}

void Projection::setNonKeyAttributes(MinimalList<MinimalString > nonKeyAttributes) {
    nonKeyAttributesBeenSet = true;
    this->nonKeyAttributes = nonKeyAttributes;
}

ProjectionType Projection::getProjectionType() const {
    return projectionType;
}

MinimalList<MinimalString > Projection::getNonKeyAttributes() const {
    return nonKeyAttributes;
}

ProvisionedThroughputDescription::ProvisionedThroughputDescription() {
    reset();
}

void ProvisionedThroughputDescription::reset() {
    writeCapacityUnitsBeenSet = false;
    numberOfDecreasesTodayBeenSet = false;
    lastIncreaseDateTimeBeenSet = false;
    lastDecreaseDateTimeBeenSet = false;
    readCapacityUnitsBeenSet = false;
}

bool ProvisionedThroughputDescription::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedWriteCapacityUnits = jsmnGetVal(WRITECAPACITYUNITS_KEY, jsonCStr, tokens, tCount);
    char* serializedNumberOfDecreasesToday = jsmnGetVal(NUMBEROFDECREASESTODAY_KEY, jsonCStr, tokens, tCount);
    char* serializedLastIncreaseDateTime = jsmnGetVal(LASTINCREASEDATETIME_KEY, jsonCStr, tokens, tCount);
    char* serializedLastDecreaseDateTime = jsmnGetVal(LASTDECREASEDATETIME_KEY, jsonCStr, tokens, tCount);
    char* serializedReadCapacityUnits = jsmnGetVal(READCAPACITYUNITS_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedWriteCapacityUnits != NULL) {
        writeCapacityUnitsBeenSet = true;
        writeCapacityUnits = atol(serializedWriteCapacityUnits);
    }
    if (serializedNumberOfDecreasesToday != NULL) {
        numberOfDecreasesTodayBeenSet = true;
        numberOfDecreasesToday = atol(serializedNumberOfDecreasesToday);
    }
    if (serializedLastIncreaseDateTime != NULL) {
        lastIncreaseDateTimeBeenSet = true;
        lastIncreaseDateTime = MinimalString(serializedLastIncreaseDateTime);
    }
    if (serializedLastDecreaseDateTime != NULL) {
        lastDecreaseDateTimeBeenSet = true;
        lastDecreaseDateTime = MinimalString(serializedLastDecreaseDateTime);
    }
    if (serializedReadCapacityUnits != NULL) {
        readCapacityUnitsBeenSet = true;
        readCapacityUnits = atol(serializedReadCapacityUnits);
    }
    if (serializedWriteCapacityUnits != NULL) {
        delete[] serializedWriteCapacityUnits;
    }
    if (serializedNumberOfDecreasesToday != NULL) {
        delete[] serializedNumberOfDecreasesToday;
    }
    if (serializedLastIncreaseDateTime != NULL) {
        delete[] serializedLastIncreaseDateTime;
    }
    if (serializedLastDecreaseDateTime != NULL) {
        delete[] serializedLastDecreaseDateTime;
    }
    if (serializedReadCapacityUnits != NULL) {
        delete[] serializedReadCapacityUnits;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString ProvisionedThroughputDescription::jsonSerialize() const {
    int itemCount = 0;
    if (writeCapacityUnitsBeenSet) {
        itemCount++;
    }
    if (numberOfDecreasesTodayBeenSet) {
        itemCount++;
    }
    if (lastIncreaseDateTimeBeenSet) {
        itemCount++;
    }
    if (lastDecreaseDateTimeBeenSet) {
        itemCount++;
    }
    if (readCapacityUnitsBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (writeCapacityUnitsBeenSet) {
        MinimalString pairValue = jsonSerializeLong(writeCapacityUnits);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(WRITECAPACITYUNITS_KEY, pairValue);
    }
    if (numberOfDecreasesTodayBeenSet) {
        MinimalString pairValue = jsonSerializeLong(numberOfDecreasesToday);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(NUMBEROFDECREASESTODAY_KEY, pairValue);
    }
    if (lastIncreaseDateTimeBeenSet) {
        MinimalString pairValue = lastIncreaseDateTime.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LASTINCREASEDATETIME_KEY, pairValue);
    }
    if (lastDecreaseDateTimeBeenSet) {
        MinimalString pairValue = lastDecreaseDateTime.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LASTDECREASEDATETIME_KEY, pairValue);
    }
    if (readCapacityUnitsBeenSet) {
        MinimalString pairValue = jsonSerializeLong(readCapacityUnits);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(READCAPACITYUNITS_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void ProvisionedThroughputDescription::setWriteCapacityUnits(long writeCapacityUnits) {
    writeCapacityUnitsBeenSet = true;
    this->writeCapacityUnits = writeCapacityUnits;
}

void ProvisionedThroughputDescription::setNumberOfDecreasesToday(long numberOfDecreasesToday) {
    numberOfDecreasesTodayBeenSet = true;
    this->numberOfDecreasesToday = numberOfDecreasesToday;
}

void ProvisionedThroughputDescription::setLastIncreaseDateTime(MinimalString lastIncreaseDateTime) {
    lastIncreaseDateTimeBeenSet = true;
    this->lastIncreaseDateTime = lastIncreaseDateTime;
}

void ProvisionedThroughputDescription::setLastDecreaseDateTime(MinimalString lastDecreaseDateTime) {
    lastDecreaseDateTimeBeenSet = true;
    this->lastDecreaseDateTime = lastDecreaseDateTime;
}

void ProvisionedThroughputDescription::setReadCapacityUnits(long readCapacityUnits) {
    readCapacityUnitsBeenSet = true;
    this->readCapacityUnits = readCapacityUnits;
}

long ProvisionedThroughputDescription::getWriteCapacityUnits() const {
    return writeCapacityUnits;
}

long ProvisionedThroughputDescription::getNumberOfDecreasesToday() const {
    return numberOfDecreasesToday;
}

MinimalString ProvisionedThroughputDescription::getLastIncreaseDateTime() const {
    return lastIncreaseDateTime;
}

MinimalString ProvisionedThroughputDescription::getLastDecreaseDateTime() const {
    return lastDecreaseDateTime;
}

long ProvisionedThroughputDescription::getReadCapacityUnits() const {
    return readCapacityUnits;
}

AttributeValue::AttributeValue() {
    reset();
}

void AttributeValue::reset() {
    SSBeenSet = false;
    BSBeenSet = false;
    BBeenSet = false;
    SBeenSet = false;
    NSBeenSet = false;
    NBeenSet = false;
}

bool AttributeValue::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedSS = jsmnGetVal(SS_KEY, jsonCStr, tokens, tCount);
    char* serializedBS = jsmnGetVal(BS_KEY, jsonCStr, tokens, tCount);
    char* serializedB = jsmnGetVal(B_KEY, jsonCStr, tokens, tCount);
    char* serializedS = jsmnGetVal(S_KEY, jsonCStr, tokens, tCount);
    char* serializedNS = jsmnGetVal(NS_KEY, jsonCStr, tokens, tCount);
    char* serializedN = jsmnGetVal(N_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedSS != NULL) {
        SSBeenSet = true;
        if (!SS.jsonDeserialize(serializedSS)) {
            toReturn = false;
        }
    }
    if (serializedBS != NULL) {
        BSBeenSet = true;
        if (!BS.jsonDeserialize(serializedBS)) {
            toReturn = false;
        }
    }
    if (serializedB != NULL) {
        BBeenSet = true;
        B = MinimalString(serializedB);
    }
    if (serializedS != NULL) {
        SBeenSet = true;
        S = MinimalString(serializedS);
    }
    if (serializedNS != NULL) {
        NSBeenSet = true;
        if (!NS.jsonDeserialize(serializedNS)) {
            toReturn = false;
        }
    }
    if (serializedN != NULL) {
        NBeenSet = true;
        N = MinimalString(serializedN);
    }
    if (serializedSS != NULL) {
        delete[] serializedSS;
    }
    if (serializedBS != NULL) {
        delete[] serializedBS;
    }
    if (serializedB != NULL) {
        delete[] serializedB;
    }
    if (serializedS != NULL) {
        delete[] serializedS;
    }
    if (serializedNS != NULL) {
        delete[] serializedNS;
    }
    if (serializedN != NULL) {
        delete[] serializedN;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString AttributeValue::jsonSerialize() const {
    int itemCount = 0;
    if (SSBeenSet) {
        itemCount++;
    }
    if (BSBeenSet) {
        itemCount++;
    }
    if (BBeenSet) {
        itemCount++;
    }
    if (SBeenSet) {
        itemCount++;
    }
    if (NSBeenSet) {
        itemCount++;
    }
    if (NBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (SSBeenSet) {
        MinimalString pairValue = SS.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SS_KEY, pairValue);
    }
    if (BSBeenSet) {
        MinimalString pairValue = BS.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(BS_KEY, pairValue);
    }
    if (BBeenSet) {
        MinimalString pairValue = B.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(B_KEY, pairValue);
    }
    if (SBeenSet) {
        MinimalString pairValue = S.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(S_KEY, pairValue);
    }
    if (NSBeenSet) {
        MinimalString pairValue = NS.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(NS_KEY, pairValue);
    }
    if (NBeenSet) {
        MinimalString pairValue = N.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(N_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void AttributeValue::setSS(MinimalList<MinimalString > SS) {
    SSBeenSet = true;
    this->SS = SS;
}

void AttributeValue::setBS(MinimalList<MinimalString > BS) {
    BSBeenSet = true;
    this->BS = BS;
}

void AttributeValue::setB(MinimalString B) {
    BBeenSet = true;
    this->B = B;
}

void AttributeValue::setS(MinimalString S) {
    SBeenSet = true;
    this->S = S;
}

void AttributeValue::setNS(MinimalList<MinimalString > NS) {
    NSBeenSet = true;
    this->NS = NS;
}

void AttributeValue::setN(MinimalString N) {
    NBeenSet = true;
    this->N = N;
}

MinimalList<MinimalString > AttributeValue::getSS() const {
    return SS;
}

MinimalList<MinimalString > AttributeValue::getBS() const {
    return BS;
}

MinimalString AttributeValue::getB() const {
    return B;
}

MinimalString AttributeValue::getS() const {
    return S;
}

MinimalList<MinimalString > AttributeValue::getNS() const {
    return NS;
}

MinimalString AttributeValue::getN() const {
    return N;
}

ProvisionedThroughput::ProvisionedThroughput() {
    reset();
}

void ProvisionedThroughput::reset() {
    writeCapacityUnitsBeenSet = false;
    readCapacityUnitsBeenSet = false;
}

bool ProvisionedThroughput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedWriteCapacityUnits = jsmnGetVal(WRITECAPACITYUNITS_KEY, jsonCStr, tokens, tCount);
    char* serializedReadCapacityUnits = jsmnGetVal(READCAPACITYUNITS_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedWriteCapacityUnits != NULL) {
        writeCapacityUnitsBeenSet = true;
        writeCapacityUnits = atol(serializedWriteCapacityUnits);
    }
    if (serializedReadCapacityUnits != NULL) {
        readCapacityUnitsBeenSet = true;
        readCapacityUnits = atol(serializedReadCapacityUnits);
    }
    if (serializedWriteCapacityUnits != NULL) {
        delete[] serializedWriteCapacityUnits;
    }
    if (serializedReadCapacityUnits != NULL) {
        delete[] serializedReadCapacityUnits;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString ProvisionedThroughput::jsonSerialize() const {
    int itemCount = 0;
    if (writeCapacityUnitsBeenSet) {
        itemCount++;
    }
    if (readCapacityUnitsBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (writeCapacityUnitsBeenSet) {
        MinimalString pairValue = jsonSerializeLong(writeCapacityUnits);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(WRITECAPACITYUNITS_KEY, pairValue);
    }
    if (readCapacityUnitsBeenSet) {
        MinimalString pairValue = jsonSerializeLong(readCapacityUnits);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(READCAPACITYUNITS_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void ProvisionedThroughput::setWriteCapacityUnits(long writeCapacityUnits) {
    writeCapacityUnitsBeenSet = true;
    this->writeCapacityUnits = writeCapacityUnits;
}

void ProvisionedThroughput::setReadCapacityUnits(long readCapacityUnits) {
    readCapacityUnitsBeenSet = true;
    this->readCapacityUnits = readCapacityUnits;
}

long ProvisionedThroughput::getWriteCapacityUnits() const {
    return writeCapacityUnits;
}

long ProvisionedThroughput::getReadCapacityUnits() const {
    return readCapacityUnits;
}

AttributeDefinition::AttributeDefinition() {
    reset();
}

void AttributeDefinition::reset() {
    attributeTypeBeenSet = false;
    attributeNameBeenSet = false;
}

bool AttributeDefinition::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedAttributeType = jsmnGetVal(ATTRIBUTETYPE_KEY, jsonCStr, tokens, tCount);
    char* serializedAttributeName = jsmnGetVal(ATTRIBUTENAME_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedAttributeType != NULL) {
        attributeTypeBeenSet = true;
        int enumInt = jsonDeserializeEnum(SCALARATTRIBUTETYPE_LOOKUP, 3, MinimalString(serializedAttributeType));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedAttributeName != NULL) {
        attributeNameBeenSet = true;
        attributeName = MinimalString(serializedAttributeName);
    }
    if (serializedAttributeType != NULL) {
        delete[] serializedAttributeType;
    }
    if (serializedAttributeName != NULL) {
        delete[] serializedAttributeName;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString AttributeDefinition::jsonSerialize() const {
    int itemCount = 0;
    if (attributeTypeBeenSet) {
        itemCount++;
    }
    if (attributeNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (attributeTypeBeenSet) {
        MinimalString pairValue = MinimalString(SCALARATTRIBUTETYPE_LOOKUP[attributeType]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTETYPE_KEY, pairValue);
    }
    if (attributeNameBeenSet) {
        MinimalString pairValue = attributeName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTENAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void AttributeDefinition::setAttributeType(ScalarAttributeType attributeType) {
    attributeTypeBeenSet = true;
    this->attributeType = attributeType;
}

void AttributeDefinition::setAttributeName(MinimalString attributeName) {
    attributeNameBeenSet = true;
    this->attributeName = attributeName;
}

ScalarAttributeType AttributeDefinition::getAttributeType() const {
    return attributeType;
}

MinimalString AttributeDefinition::getAttributeName() const {
    return attributeName;
}

LocalSecondaryIndexDescription::LocalSecondaryIndexDescription() {
    reset();
}

void LocalSecondaryIndexDescription::reset() {
    projectionBeenSet = false;
    itemCountBeenSet = false;
    indexSizeBytesBeenSet = false;
    keySchemaBeenSet = false;
    indexNameBeenSet = false;
}

bool LocalSecondaryIndexDescription::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedProjection = jsmnGetVal(PROJECTION_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCount = jsmnGetVal(ITEMCOUNT_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexSizeBytes = jsmnGetVal(INDEXSIZEBYTES_KEY, jsonCStr, tokens, tCount);
    char* serializedKeySchema = jsmnGetVal(KEYSCHEMA_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexName = jsmnGetVal(INDEXNAME_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedProjection != NULL) {
        projectionBeenSet = true;
        if (!projection.jsonDeserialize(serializedProjection)) {
            toReturn = false;
        }
    }
    if (serializedItemCount != NULL) {
        itemCountBeenSet = true;
        itemCount = atol(serializedItemCount);
    }
    if (serializedIndexSizeBytes != NULL) {
        indexSizeBytesBeenSet = true;
        indexSizeBytes = atol(serializedIndexSizeBytes);
    }
    if (serializedKeySchema != NULL) {
        keySchemaBeenSet = true;
        if (!keySchema.jsonDeserialize(serializedKeySchema)) {
            toReturn = false;
        }
    }
    if (serializedIndexName != NULL) {
        indexNameBeenSet = true;
        indexName = MinimalString(serializedIndexName);
    }
    if (serializedProjection != NULL) {
        delete[] serializedProjection;
    }
    if (serializedItemCount != NULL) {
        delete[] serializedItemCount;
    }
    if (serializedIndexSizeBytes != NULL) {
        delete[] serializedIndexSizeBytes;
    }
    if (serializedKeySchema != NULL) {
        delete[] serializedKeySchema;
    }
    if (serializedIndexName != NULL) {
        delete[] serializedIndexName;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString LocalSecondaryIndexDescription::jsonSerialize() const {
    int itemCount = 0;
    if (projectionBeenSet) {
        itemCount++;
    }
    if (itemCountBeenSet) {
        itemCount++;
    }
    if (indexSizeBytesBeenSet) {
        itemCount++;
    }
    if (keySchemaBeenSet) {
        itemCount++;
    }
    if (indexNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (projectionBeenSet) {
        MinimalString pairValue = projection.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROJECTION_KEY, pairValue);
    }
    if (itemCountBeenSet) {
        MinimalString pairValue = jsonSerializeLong(itemCount);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ITEMCOUNT_KEY, pairValue);
    }
    if (indexSizeBytesBeenSet) {
        MinimalString pairValue = jsonSerializeLong(indexSizeBytes);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXSIZEBYTES_KEY, pairValue);
    }
    if (keySchemaBeenSet) {
        MinimalString pairValue = keySchema.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYSCHEMA_KEY, pairValue);
    }
    if (indexNameBeenSet) {
        MinimalString pairValue = indexName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXNAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void LocalSecondaryIndexDescription::setProjection(Projection projection) {
    projectionBeenSet = true;
    this->projection = projection;
}

void LocalSecondaryIndexDescription::setItemCount(long itemCount) {
    itemCountBeenSet = true;
    this->itemCount = itemCount;
}

void LocalSecondaryIndexDescription::setIndexSizeBytes(long indexSizeBytes) {
    indexSizeBytesBeenSet = true;
    this->indexSizeBytes = indexSizeBytes;
}

void LocalSecondaryIndexDescription::setKeySchema(MinimalList<KeySchemaElement > keySchema) {
    keySchemaBeenSet = true;
    this->keySchema = keySchema;
}

void LocalSecondaryIndexDescription::setIndexName(MinimalString indexName) {
    indexNameBeenSet = true;
    this->indexName = indexName;
}

Projection LocalSecondaryIndexDescription::getProjection() const {
    return projection;
}

long LocalSecondaryIndexDescription::getItemCount() const {
    return itemCount;
}

long LocalSecondaryIndexDescription::getIndexSizeBytes() const {
    return indexSizeBytes;
}

MinimalList<KeySchemaElement > LocalSecondaryIndexDescription::getKeySchema() const {
    return keySchema;
}

MinimalString LocalSecondaryIndexDescription::getIndexName() const {
    return indexName;
}

GlobalSecondaryIndexDescription::GlobalSecondaryIndexDescription() {
    reset();
}

void GlobalSecondaryIndexDescription::reset() {
    projectionBeenSet = false;
    provisionedThroughputBeenSet = false;
    indexStatusBeenSet = false;
    itemCountBeenSet = false;
    indexSizeBytesBeenSet = false;
    keySchemaBeenSet = false;
    indexNameBeenSet = false;
}

bool GlobalSecondaryIndexDescription::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedProjection = jsmnGetVal(PROJECTION_KEY, jsonCStr, tokens, tCount);
    char* serializedProvisionedThroughput = jsmnGetVal(PROVISIONEDTHROUGHPUT_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexStatus = jsmnGetVal(INDEXSTATUS_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCount = jsmnGetVal(ITEMCOUNT_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexSizeBytes = jsmnGetVal(INDEXSIZEBYTES_KEY, jsonCStr, tokens, tCount);
    char* serializedKeySchema = jsmnGetVal(KEYSCHEMA_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexName = jsmnGetVal(INDEXNAME_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedProjection != NULL) {
        projectionBeenSet = true;
        if (!projection.jsonDeserialize(serializedProjection)) {
            toReturn = false;
        }
    }
    if (serializedProvisionedThroughput != NULL) {
        provisionedThroughputBeenSet = true;
        if (!provisionedThroughput.jsonDeserialize(serializedProvisionedThroughput)) {
            toReturn = false;
        }
    }
    if (serializedIndexStatus != NULL) {
        indexStatusBeenSet = true;
        int enumInt = jsonDeserializeEnum(INDEXSTATUS_LOOKUP, 4, MinimalString(serializedIndexStatus));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedItemCount != NULL) {
        itemCountBeenSet = true;
        itemCount = atol(serializedItemCount);
    }
    if (serializedIndexSizeBytes != NULL) {
        indexSizeBytesBeenSet = true;
        indexSizeBytes = atol(serializedIndexSizeBytes);
    }
    if (serializedKeySchema != NULL) {
        keySchemaBeenSet = true;
        if (!keySchema.jsonDeserialize(serializedKeySchema)) {
            toReturn = false;
        }
    }
    if (serializedIndexName != NULL) {
        indexNameBeenSet = true;
        indexName = MinimalString(serializedIndexName);
    }
    if (serializedProjection != NULL) {
        delete[] serializedProjection;
    }
    if (serializedProvisionedThroughput != NULL) {
        delete[] serializedProvisionedThroughput;
    }
    if (serializedIndexStatus != NULL) {
        delete[] serializedIndexStatus;
    }
    if (serializedItemCount != NULL) {
        delete[] serializedItemCount;
    }
    if (serializedIndexSizeBytes != NULL) {
        delete[] serializedIndexSizeBytes;
    }
    if (serializedKeySchema != NULL) {
        delete[] serializedKeySchema;
    }
    if (serializedIndexName != NULL) {
        delete[] serializedIndexName;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString GlobalSecondaryIndexDescription::jsonSerialize() const {
    int itemCount = 0;
    if (projectionBeenSet) {
        itemCount++;
    }
    if (provisionedThroughputBeenSet) {
        itemCount++;
    }
    if (indexStatusBeenSet) {
        itemCount++;
    }
    if (itemCountBeenSet) {
        itemCount++;
    }
    if (indexSizeBytesBeenSet) {
        itemCount++;
    }
    if (keySchemaBeenSet) {
        itemCount++;
    }
    if (indexNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (projectionBeenSet) {
        MinimalString pairValue = projection.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROJECTION_KEY, pairValue);
    }
    if (provisionedThroughputBeenSet) {
        MinimalString pairValue = provisionedThroughput.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROVISIONEDTHROUGHPUT_KEY, pairValue);
    }
    if (indexStatusBeenSet) {
        MinimalString pairValue = MinimalString(INDEXSTATUS_LOOKUP[indexStatus]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXSTATUS_KEY, pairValue);
    }
    if (itemCountBeenSet) {
        MinimalString pairValue = jsonSerializeLong(itemCount);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ITEMCOUNT_KEY, pairValue);
    }
    if (indexSizeBytesBeenSet) {
        MinimalString pairValue = jsonSerializeLong(indexSizeBytes);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXSIZEBYTES_KEY, pairValue);
    }
    if (keySchemaBeenSet) {
        MinimalString pairValue = keySchema.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYSCHEMA_KEY, pairValue);
    }
    if (indexNameBeenSet) {
        MinimalString pairValue = indexName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXNAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void GlobalSecondaryIndexDescription::setProjection(Projection projection) {
    projectionBeenSet = true;
    this->projection = projection;
}

void GlobalSecondaryIndexDescription::setProvisionedThroughput(ProvisionedThroughputDescription provisionedThroughput) {
    provisionedThroughputBeenSet = true;
    this->provisionedThroughput = provisionedThroughput;
}

void GlobalSecondaryIndexDescription::setIndexStatus(IndexStatus indexStatus) {
    indexStatusBeenSet = true;
    this->indexStatus = indexStatus;
}

void GlobalSecondaryIndexDescription::setItemCount(long itemCount) {
    itemCountBeenSet = true;
    this->itemCount = itemCount;
}

void GlobalSecondaryIndexDescription::setIndexSizeBytes(long indexSizeBytes) {
    indexSizeBytesBeenSet = true;
    this->indexSizeBytes = indexSizeBytes;
}

void GlobalSecondaryIndexDescription::setKeySchema(MinimalList<KeySchemaElement > keySchema) {
    keySchemaBeenSet = true;
    this->keySchema = keySchema;
}

void GlobalSecondaryIndexDescription::setIndexName(MinimalString indexName) {
    indexNameBeenSet = true;
    this->indexName = indexName;
}

Projection GlobalSecondaryIndexDescription::getProjection() const {
    return projection;
}

ProvisionedThroughputDescription GlobalSecondaryIndexDescription::getProvisionedThroughput() const {
    return provisionedThroughput;
}

IndexStatus GlobalSecondaryIndexDescription::getIndexStatus() const {
    return indexStatus;
}

long GlobalSecondaryIndexDescription::getItemCount() const {
    return itemCount;
}

long GlobalSecondaryIndexDescription::getIndexSizeBytes() const {
    return indexSizeBytes;
}

MinimalList<KeySchemaElement > GlobalSecondaryIndexDescription::getKeySchema() const {
    return keySchema;
}

MinimalString GlobalSecondaryIndexDescription::getIndexName() const {
    return indexName;
}

Capacity::Capacity() {
    reset();
}

void Capacity::reset() {
    capacityUnitsBeenSet = false;
}

bool Capacity::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedCapacityUnits = jsmnGetVal(CAPACITYUNITS_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedCapacityUnits != NULL) {
        capacityUnitsBeenSet = true;
        capacityUnits = atof(serializedCapacityUnits);
    }
    if (serializedCapacityUnits != NULL) {
        delete[] serializedCapacityUnits;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString Capacity::jsonSerialize() const {
    int itemCount = 0;
    if (capacityUnitsBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (capacityUnitsBeenSet) {
        MinimalString pairValue = jsonSerializeDouble(capacityUnits);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CAPACITYUNITS_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void Capacity::setCapacityUnits(double capacityUnits) {
    capacityUnitsBeenSet = true;
    this->capacityUnits = capacityUnits;
}

double Capacity::getCapacityUnits() const {
    return capacityUnits;
}

PutRequest::PutRequest() {
    reset();
}

void PutRequest::reset() {
    itemBeenSet = false;
}

bool PutRequest::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedItem = jsmnGetVal(ITEM_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedItem != NULL) {
        itemBeenSet = true;
        if (!item.jsonDeserialize(serializedItem)) {
            toReturn = false;
        }
    }
    if (serializedItem != NULL) {
        delete[] serializedItem;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString PutRequest::jsonSerialize() const {
    int itemCount = 0;
    if (itemBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (itemBeenSet) {
        MinimalString pairValue = item.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ITEM_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void PutRequest::setItem(MinimalMap<AttributeValue > item) {
    itemBeenSet = true;
    this->item = item;
}

MinimalMap<AttributeValue > PutRequest::getItem() const {
    return item;
}

DeleteRequest::DeleteRequest() {
    reset();
}

void DeleteRequest::reset() {
    keyBeenSet = false;
}

bool DeleteRequest::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedKey = jsmnGetVal(KEY_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedKey != NULL) {
        keyBeenSet = true;
        if (!key.jsonDeserialize(serializedKey)) {
            toReturn = false;
        }
    }
    if (serializedKey != NULL) {
        delete[] serializedKey;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString DeleteRequest::jsonSerialize() const {
    int itemCount = 0;
    if (keyBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (keyBeenSet) {
        MinimalString pairValue = key.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void DeleteRequest::setKey(MinimalMap<AttributeValue > key) {
    keyBeenSet = true;
    this->key = key;
}

MinimalMap<AttributeValue > DeleteRequest::getKey() const {
    return key;
}

UpdateGlobalSecondaryIndexAction::UpdateGlobalSecondaryIndexAction() {
    reset();
}

void UpdateGlobalSecondaryIndexAction::reset() {
    provisionedThroughputBeenSet = false;
    indexNameBeenSet = false;
}

bool UpdateGlobalSecondaryIndexAction::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedProvisionedThroughput = jsmnGetVal(PROVISIONEDTHROUGHPUT_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexName = jsmnGetVal(INDEXNAME_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedProvisionedThroughput != NULL) {
        provisionedThroughputBeenSet = true;
        if (!provisionedThroughput.jsonDeserialize(serializedProvisionedThroughput)) {
            toReturn = false;
        }
    }
    if (serializedIndexName != NULL) {
        indexNameBeenSet = true;
        indexName = MinimalString(serializedIndexName);
    }
    if (serializedProvisionedThroughput != NULL) {
        delete[] serializedProvisionedThroughput;
    }
    if (serializedIndexName != NULL) {
        delete[] serializedIndexName;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString UpdateGlobalSecondaryIndexAction::jsonSerialize() const {
    int itemCount = 0;
    if (provisionedThroughputBeenSet) {
        itemCount++;
    }
    if (indexNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (provisionedThroughputBeenSet) {
        MinimalString pairValue = provisionedThroughput.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROVISIONEDTHROUGHPUT_KEY, pairValue);
    }
    if (indexNameBeenSet) {
        MinimalString pairValue = indexName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXNAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void UpdateGlobalSecondaryIndexAction::setProvisionedThroughput(ProvisionedThroughput provisionedThroughput) {
    provisionedThroughputBeenSet = true;
    this->provisionedThroughput = provisionedThroughput;
}

void UpdateGlobalSecondaryIndexAction::setIndexName(MinimalString indexName) {
    indexNameBeenSet = true;
    this->indexName = indexName;
}

ProvisionedThroughput UpdateGlobalSecondaryIndexAction::getProvisionedThroughput() const {
    return provisionedThroughput;
}

MinimalString UpdateGlobalSecondaryIndexAction::getIndexName() const {
    return indexName;
}

TableDescription::TableDescription() {
    reset();
}

void TableDescription::reset() {
    provisionedThroughputBeenSet = false;
    creationDateTimeBeenSet = false;
    itemCountBeenSet = false;
    globalSecondaryIndexesBeenSet = false;
    tableStatusBeenSet = false;
    tableNameBeenSet = false;
    localSecondaryIndexesBeenSet = false;
    keySchemaBeenSet = false;
    attributeDefinitionsBeenSet = false;
    tableSizeBytesBeenSet = false;
}

bool TableDescription::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedProvisionedThroughput = jsmnGetVal(PROVISIONEDTHROUGHPUT_KEY, jsonCStr, tokens, tCount);
    char* serializedCreationDateTime = jsmnGetVal(CREATIONDATETIME_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCount = jsmnGetVal(ITEMCOUNT_KEY, jsonCStr, tokens, tCount);
    char* serializedGlobalSecondaryIndexes = jsmnGetVal(GLOBALSECONDARYINDEXES_KEY, jsonCStr, tokens, tCount);
    char* serializedTableStatus = jsmnGetVal(TABLESTATUS_KEY, jsonCStr, tokens, tCount);
    char* serializedTableName = jsmnGetVal(TABLENAME_KEY, jsonCStr, tokens, tCount);
    char* serializedLocalSecondaryIndexes = jsmnGetVal(LOCALSECONDARYINDEXES_KEY, jsonCStr, tokens, tCount);
    char* serializedKeySchema = jsmnGetVal(KEYSCHEMA_KEY, jsonCStr, tokens, tCount);
    char* serializedAttributeDefinitions = jsmnGetVal(ATTRIBUTEDEFINITIONS_KEY, jsonCStr, tokens, tCount);
    char* serializedTableSizeBytes = jsmnGetVal(TABLESIZEBYTES_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedProvisionedThroughput != NULL) {
        provisionedThroughputBeenSet = true;
        if (!provisionedThroughput.jsonDeserialize(serializedProvisionedThroughput)) {
            toReturn = false;
        }
    }
    if (serializedCreationDateTime != NULL) {
        creationDateTimeBeenSet = true;
        creationDateTime = MinimalString(serializedCreationDateTime);
    }
    if (serializedItemCount != NULL) {
        itemCountBeenSet = true;
        itemCount = atol(serializedItemCount);
    }
    if (serializedGlobalSecondaryIndexes != NULL) {
        globalSecondaryIndexesBeenSet = true;
        if (!globalSecondaryIndexes.jsonDeserialize(serializedGlobalSecondaryIndexes)) {
            toReturn = false;
        }
    }
    if (serializedTableStatus != NULL) {
        tableStatusBeenSet = true;
        int enumInt = jsonDeserializeEnum(TABLESTATUS_LOOKUP, 4, MinimalString(serializedTableStatus));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedTableName != NULL) {
        tableNameBeenSet = true;
        tableName = MinimalString(serializedTableName);
    }
    if (serializedLocalSecondaryIndexes != NULL) {
        localSecondaryIndexesBeenSet = true;
        if (!localSecondaryIndexes.jsonDeserialize(serializedLocalSecondaryIndexes)) {
            toReturn = false;
        }
    }
    if (serializedKeySchema != NULL) {
        keySchemaBeenSet = true;
        if (!keySchema.jsonDeserialize(serializedKeySchema)) {
            toReturn = false;
        }
    }
    if (serializedAttributeDefinitions != NULL) {
        attributeDefinitionsBeenSet = true;
        if (!attributeDefinitions.jsonDeserialize(serializedAttributeDefinitions)) {
            toReturn = false;
        }
    }
    if (serializedTableSizeBytes != NULL) {
        tableSizeBytesBeenSet = true;
        tableSizeBytes = atol(serializedTableSizeBytes);
    }
    if (serializedProvisionedThroughput != NULL) {
        delete[] serializedProvisionedThroughput;
    }
    if (serializedCreationDateTime != NULL) {
        delete[] serializedCreationDateTime;
    }
    if (serializedItemCount != NULL) {
        delete[] serializedItemCount;
    }
    if (serializedGlobalSecondaryIndexes != NULL) {
        delete[] serializedGlobalSecondaryIndexes;
    }
    if (serializedTableStatus != NULL) {
        delete[] serializedTableStatus;
    }
    if (serializedTableName != NULL) {
        delete[] serializedTableName;
    }
    if (serializedLocalSecondaryIndexes != NULL) {
        delete[] serializedLocalSecondaryIndexes;
    }
    if (serializedKeySchema != NULL) {
        delete[] serializedKeySchema;
    }
    if (serializedAttributeDefinitions != NULL) {
        delete[] serializedAttributeDefinitions;
    }
    if (serializedTableSizeBytes != NULL) {
        delete[] serializedTableSizeBytes;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString TableDescription::jsonSerialize() const {
    int itemCount = 0;
    if (provisionedThroughputBeenSet) {
        itemCount++;
    }
    if (creationDateTimeBeenSet) {
        itemCount++;
    }
    if (itemCountBeenSet) {
        itemCount++;
    }
    if (globalSecondaryIndexesBeenSet) {
        itemCount++;
    }
    if (tableStatusBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (localSecondaryIndexesBeenSet) {
        itemCount++;
    }
    if (keySchemaBeenSet) {
        itemCount++;
    }
    if (attributeDefinitionsBeenSet) {
        itemCount++;
    }
    if (tableSizeBytesBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (provisionedThroughputBeenSet) {
        MinimalString pairValue = provisionedThroughput.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROVISIONEDTHROUGHPUT_KEY, pairValue);
    }
    if (creationDateTimeBeenSet) {
        MinimalString pairValue = creationDateTime.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CREATIONDATETIME_KEY, pairValue);
    }
    if (itemCountBeenSet) {
        MinimalString pairValue = jsonSerializeLong(itemCount);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ITEMCOUNT_KEY, pairValue);
    }
    if (globalSecondaryIndexesBeenSet) {
        MinimalString pairValue = globalSecondaryIndexes.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(GLOBALSECONDARYINDEXES_KEY, pairValue);
    }
    if (tableStatusBeenSet) {
        MinimalString pairValue = MinimalString(TABLESTATUS_LOOKUP[tableStatus]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLESTATUS_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (localSecondaryIndexesBeenSet) {
        MinimalString pairValue = localSecondaryIndexes.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LOCALSECONDARYINDEXES_KEY, pairValue);
    }
    if (keySchemaBeenSet) {
        MinimalString pairValue = keySchema.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYSCHEMA_KEY, pairValue);
    }
    if (attributeDefinitionsBeenSet) {
        MinimalString pairValue = attributeDefinitions.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTEDEFINITIONS_KEY, pairValue);
    }
    if (tableSizeBytesBeenSet) {
        MinimalString pairValue = jsonSerializeLong(tableSizeBytes);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLESIZEBYTES_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void TableDescription::setProvisionedThroughput(ProvisionedThroughputDescription provisionedThroughput) {
    provisionedThroughputBeenSet = true;
    this->provisionedThroughput = provisionedThroughput;
}

void TableDescription::setCreationDateTime(MinimalString creationDateTime) {
    creationDateTimeBeenSet = true;
    this->creationDateTime = creationDateTime;
}

void TableDescription::setItemCount(long itemCount) {
    itemCountBeenSet = true;
    this->itemCount = itemCount;
}

void TableDescription::setGlobalSecondaryIndexes(MinimalList<GlobalSecondaryIndexDescription > globalSecondaryIndexes) {
    globalSecondaryIndexesBeenSet = true;
    this->globalSecondaryIndexes = globalSecondaryIndexes;
}

void TableDescription::setTableStatus(TableStatus tableStatus) {
    tableStatusBeenSet = true;
    this->tableStatus = tableStatus;
}

void TableDescription::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void TableDescription::setLocalSecondaryIndexes(MinimalList<LocalSecondaryIndexDescription > localSecondaryIndexes) {
    localSecondaryIndexesBeenSet = true;
    this->localSecondaryIndexes = localSecondaryIndexes;
}

void TableDescription::setKeySchema(MinimalList<KeySchemaElement > keySchema) {
    keySchemaBeenSet = true;
    this->keySchema = keySchema;
}

void TableDescription::setAttributeDefinitions(MinimalList<AttributeDefinition > attributeDefinitions) {
    attributeDefinitionsBeenSet = true;
    this->attributeDefinitions = attributeDefinitions;
}

void TableDescription::setTableSizeBytes(long tableSizeBytes) {
    tableSizeBytesBeenSet = true;
    this->tableSizeBytes = tableSizeBytes;
}

ProvisionedThroughputDescription TableDescription::getProvisionedThroughput() const {
    return provisionedThroughput;
}

MinimalString TableDescription::getCreationDateTime() const {
    return creationDateTime;
}

long TableDescription::getItemCount() const {
    return itemCount;
}

MinimalList<GlobalSecondaryIndexDescription > TableDescription::getGlobalSecondaryIndexes() const {
    return globalSecondaryIndexes;
}

TableStatus TableDescription::getTableStatus() const {
    return tableStatus;
}

MinimalString TableDescription::getTableName() const {
    return tableName;
}

MinimalList<LocalSecondaryIndexDescription > TableDescription::getLocalSecondaryIndexes() const {
    return localSecondaryIndexes;
}

MinimalList<KeySchemaElement > TableDescription::getKeySchema() const {
    return keySchema;
}

MinimalList<AttributeDefinition > TableDescription::getAttributeDefinitions() const {
    return attributeDefinitions;
}

long TableDescription::getTableSizeBytes() const {
    return tableSizeBytes;
}

ConsumedCapacity::ConsumedCapacity() {
    reset();
}

void ConsumedCapacity::reset() {
    capacityUnitsBeenSet = false;
    globalSecondaryIndexesBeenSet = false;
    tableNameBeenSet = false;
    localSecondaryIndexesBeenSet = false;
    tableBeenSet = false;
}

bool ConsumedCapacity::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedCapacityUnits = jsmnGetVal(CAPACITYUNITS_KEY, jsonCStr, tokens, tCount);
    char* serializedGlobalSecondaryIndexes = jsmnGetVal(GLOBALSECONDARYINDEXES_KEY, jsonCStr, tokens, tCount);
    char* serializedTableName = jsmnGetVal(TABLENAME_KEY, jsonCStr, tokens, tCount);
    char* serializedLocalSecondaryIndexes = jsmnGetVal(LOCALSECONDARYINDEXES_KEY, jsonCStr, tokens, tCount);
    char* serializedTable = jsmnGetVal(TABLE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedCapacityUnits != NULL) {
        capacityUnitsBeenSet = true;
        capacityUnits = atof(serializedCapacityUnits);
    }
    if (serializedGlobalSecondaryIndexes != NULL) {
        globalSecondaryIndexesBeenSet = true;
        if (!globalSecondaryIndexes.jsonDeserialize(serializedGlobalSecondaryIndexes)) {
            toReturn = false;
        }
    }
    if (serializedTableName != NULL) {
        tableNameBeenSet = true;
        tableName = MinimalString(serializedTableName);
    }
    if (serializedLocalSecondaryIndexes != NULL) {
        localSecondaryIndexesBeenSet = true;
        if (!localSecondaryIndexes.jsonDeserialize(serializedLocalSecondaryIndexes)) {
            toReturn = false;
        }
    }
    if (serializedTable != NULL) {
        tableBeenSet = true;
        if (!table.jsonDeserialize(serializedTable)) {
            toReturn = false;
        }
    }
    if (serializedCapacityUnits != NULL) {
        delete[] serializedCapacityUnits;
    }
    if (serializedGlobalSecondaryIndexes != NULL) {
        delete[] serializedGlobalSecondaryIndexes;
    }
    if (serializedTableName != NULL) {
        delete[] serializedTableName;
    }
    if (serializedLocalSecondaryIndexes != NULL) {
        delete[] serializedLocalSecondaryIndexes;
    }
    if (serializedTable != NULL) {
        delete[] serializedTable;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString ConsumedCapacity::jsonSerialize() const {
    int itemCount = 0;
    if (capacityUnitsBeenSet) {
        itemCount++;
    }
    if (globalSecondaryIndexesBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (localSecondaryIndexesBeenSet) {
        itemCount++;
    }
    if (tableBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (capacityUnitsBeenSet) {
        MinimalString pairValue = jsonSerializeDouble(capacityUnits);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CAPACITYUNITS_KEY, pairValue);
    }
    if (globalSecondaryIndexesBeenSet) {
        MinimalString pairValue = globalSecondaryIndexes.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(GLOBALSECONDARYINDEXES_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (localSecondaryIndexesBeenSet) {
        MinimalString pairValue = localSecondaryIndexes.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LOCALSECONDARYINDEXES_KEY, pairValue);
    }
    if (tableBeenSet) {
        MinimalString pairValue = table.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLE_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void ConsumedCapacity::setCapacityUnits(double capacityUnits) {
    capacityUnitsBeenSet = true;
    this->capacityUnits = capacityUnits;
}

void ConsumedCapacity::setGlobalSecondaryIndexes(MinimalMap<Capacity > globalSecondaryIndexes) {
    globalSecondaryIndexesBeenSet = true;
    this->globalSecondaryIndexes = globalSecondaryIndexes;
}

void ConsumedCapacity::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void ConsumedCapacity::setLocalSecondaryIndexes(MinimalMap<Capacity > localSecondaryIndexes) {
    localSecondaryIndexesBeenSet = true;
    this->localSecondaryIndexes = localSecondaryIndexes;
}

void ConsumedCapacity::setTable(Capacity table) {
    tableBeenSet = true;
    this->table = table;
}

double ConsumedCapacity::getCapacityUnits() const {
    return capacityUnits;
}

MinimalMap<Capacity > ConsumedCapacity::getGlobalSecondaryIndexes() const {
    return globalSecondaryIndexes;
}

MinimalString ConsumedCapacity::getTableName() const {
    return tableName;
}

MinimalMap<Capacity > ConsumedCapacity::getLocalSecondaryIndexes() const {
    return localSecondaryIndexes;
}

Capacity ConsumedCapacity::getTable() const {
    return table;
}

ItemCollectionMetrics::ItemCollectionMetrics() {
    reset();
}

void ItemCollectionMetrics::reset() {
    sizeEstimateRangeGBBeenSet = false;
    itemCollectionKeyBeenSet = false;
}

bool ItemCollectionMetrics::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedSizeEstimateRangeGB = jsmnGetVal(SIZEESTIMATERANGEGB_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCollectionKey = jsmnGetVal(ITEMCOLLECTIONKEY_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedSizeEstimateRangeGB != NULL) {
        sizeEstimateRangeGBBeenSet = true;
        if (!sizeEstimateRangeGB.jsonDeserialize(serializedSizeEstimateRangeGB)) {
            toReturn = false;
        }
    }
    if (serializedItemCollectionKey != NULL) {
        itemCollectionKeyBeenSet = true;
        if (!itemCollectionKey.jsonDeserialize(serializedItemCollectionKey)) {
            toReturn = false;
        }
    }
    if (serializedSizeEstimateRangeGB != NULL) {
        delete[] serializedSizeEstimateRangeGB;
    }
    if (serializedItemCollectionKey != NULL) {
        delete[] serializedItemCollectionKey;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString ItemCollectionMetrics::jsonSerialize() const {
    int itemCount = 0;
    if (sizeEstimateRangeGBBeenSet) {
        itemCount++;
    }
    if (itemCollectionKeyBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (sizeEstimateRangeGBBeenSet) {
        MinimalString pairValue = sizeEstimateRangeGB.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SIZEESTIMATERANGEGB_KEY, pairValue);
    }
    if (itemCollectionKeyBeenSet) {
        MinimalString pairValue = itemCollectionKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ITEMCOLLECTIONKEY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void ItemCollectionMetrics::setSizeEstimateRangeGB(MinimalList<SerializableDouble > sizeEstimateRangeGB) {
    sizeEstimateRangeGBBeenSet = true;
    this->sizeEstimateRangeGB = sizeEstimateRangeGB;
}

void ItemCollectionMetrics::setItemCollectionKey(MinimalMap<AttributeValue > itemCollectionKey) {
    itemCollectionKeyBeenSet = true;
    this->itemCollectionKey = itemCollectionKey;
}

MinimalList<SerializableDouble > ItemCollectionMetrics::getSizeEstimateRangeGB() const {
    return sizeEstimateRangeGB;
}

MinimalMap<AttributeValue > ItemCollectionMetrics::getItemCollectionKey() const {
    return itemCollectionKey;
}

ExpectedAttributeValue::ExpectedAttributeValue() {
    reset();
}

void ExpectedAttributeValue::reset() {
    valueBeenSet = false;
    existsBeenSet = false;
    attributeValueListBeenSet = false;
    comparisonOperatorBeenSet = false;
}

bool ExpectedAttributeValue::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedValue = jsmnGetVal(VALUE_KEY, jsonCStr, tokens, tCount);
    char* serializedExists = jsmnGetVal(EXISTS_KEY, jsonCStr, tokens, tCount);
    char* serializedAttributeValueList = jsmnGetVal(ATTRIBUTEVALUELIST_KEY, jsonCStr, tokens, tCount);
    char* serializedComparisonOperator = jsmnGetVal(COMPARISONOPERATOR_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedValue != NULL) {
        valueBeenSet = true;
        if (!value.jsonDeserialize(serializedValue)) {
            toReturn = false;
        }
    }
    if (serializedExists != NULL) {
        existsBeenSet = true;
        jsonDeserializeBool(serializedExists);
    }
    if (serializedAttributeValueList != NULL) {
        attributeValueListBeenSet = true;
        if (!attributeValueList.jsonDeserialize(serializedAttributeValueList)) {
            toReturn = false;
        }
    }
    if (serializedComparisonOperator != NULL) {
        comparisonOperatorBeenSet = true;
        int enumInt = jsonDeserializeEnum(COMPARISONOPERATOR_LOOKUP, 13, MinimalString(serializedComparisonOperator));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedValue != NULL) {
        delete[] serializedValue;
    }
    if (serializedExists != NULL) {
        delete[] serializedExists;
    }
    if (serializedAttributeValueList != NULL) {
        delete[] serializedAttributeValueList;
    }
    if (serializedComparisonOperator != NULL) {
        delete[] serializedComparisonOperator;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString ExpectedAttributeValue::jsonSerialize() const {
    int itemCount = 0;
    if (valueBeenSet) {
        itemCount++;
    }
    if (existsBeenSet) {
        itemCount++;
    }
    if (attributeValueListBeenSet) {
        itemCount++;
    }
    if (comparisonOperatorBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (valueBeenSet) {
        MinimalString pairValue = value.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(VALUE_KEY, pairValue);
    }
    if (existsBeenSet) {
        MinimalString pairValue = jsonSerializeBool(exists);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXISTS_KEY, pairValue);
    }
    if (attributeValueListBeenSet) {
        MinimalString pairValue = attributeValueList.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTEVALUELIST_KEY, pairValue);
    }
    if (comparisonOperatorBeenSet) {
        MinimalString pairValue = MinimalString(COMPARISONOPERATOR_LOOKUP[comparisonOperator]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(COMPARISONOPERATOR_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void ExpectedAttributeValue::setValue(AttributeValue value) {
    valueBeenSet = true;
    this->value = value;
}

void ExpectedAttributeValue::setExists(bool exists) {
    existsBeenSet = true;
    this->exists = exists;
}

void ExpectedAttributeValue::setAttributeValueList(MinimalList<AttributeValue > attributeValueList) {
    attributeValueListBeenSet = true;
    this->attributeValueList = attributeValueList;
}

void ExpectedAttributeValue::setComparisonOperator(ComparisonOperator comparisonOperator) {
    comparisonOperatorBeenSet = true;
    this->comparisonOperator = comparisonOperator;
}

AttributeValue ExpectedAttributeValue::getValue() const {
    return value;
}

bool ExpectedAttributeValue::getExists() const {
    return exists;
}

MinimalList<AttributeValue > ExpectedAttributeValue::getAttributeValueList() const {
    return attributeValueList;
}

ComparisonOperator ExpectedAttributeValue::getComparisonOperator() const {
    return comparisonOperator;
}

Condition::Condition() {
    reset();
}

void Condition::reset() {
    attributeValueListBeenSet = false;
    comparisonOperatorBeenSet = false;
}

bool Condition::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedAttributeValueList = jsmnGetVal(ATTRIBUTEVALUELIST_KEY, jsonCStr, tokens, tCount);
    char* serializedComparisonOperator = jsmnGetVal(COMPARISONOPERATOR_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedAttributeValueList != NULL) {
        attributeValueListBeenSet = true;
        if (!attributeValueList.jsonDeserialize(serializedAttributeValueList)) {
            toReturn = false;
        }
    }
    if (serializedComparisonOperator != NULL) {
        comparisonOperatorBeenSet = true;
        int enumInt = jsonDeserializeEnum(COMPARISONOPERATOR_LOOKUP, 13, MinimalString(serializedComparisonOperator));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedAttributeValueList != NULL) {
        delete[] serializedAttributeValueList;
    }
    if (serializedComparisonOperator != NULL) {
        delete[] serializedComparisonOperator;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString Condition::jsonSerialize() const {
    int itemCount = 0;
    if (attributeValueListBeenSet) {
        itemCount++;
    }
    if (comparisonOperatorBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (attributeValueListBeenSet) {
        MinimalString pairValue = attributeValueList.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTEVALUELIST_KEY, pairValue);
    }
    if (comparisonOperatorBeenSet) {
        MinimalString pairValue = MinimalString(COMPARISONOPERATOR_LOOKUP[comparisonOperator]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(COMPARISONOPERATOR_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void Condition::setAttributeValueList(MinimalList<AttributeValue > attributeValueList) {
    attributeValueListBeenSet = true;
    this->attributeValueList = attributeValueList;
}

void Condition::setComparisonOperator(ComparisonOperator comparisonOperator) {
    comparisonOperatorBeenSet = true;
    this->comparisonOperator = comparisonOperator;
}

MinimalList<AttributeValue > Condition::getAttributeValueList() const {
    return attributeValueList;
}

ComparisonOperator Condition::getComparisonOperator() const {
    return comparisonOperator;
}

WriteRequest::WriteRequest() {
    reset();
}

void WriteRequest::reset() {
    deleteRequestBeenSet = false;
    putRequestBeenSet = false;
}

bool WriteRequest::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedDeleteRequest = jsmnGetVal(DELETEREQUEST_KEY, jsonCStr, tokens, tCount);
    char* serializedPutRequest = jsmnGetVal(PUTREQUEST_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedDeleteRequest != NULL) {
        deleteRequestBeenSet = true;
        if (!deleteRequest.jsonDeserialize(serializedDeleteRequest)) {
            toReturn = false;
        }
    }
    if (serializedPutRequest != NULL) {
        putRequestBeenSet = true;
        if (!putRequest.jsonDeserialize(serializedPutRequest)) {
            toReturn = false;
        }
    }
    if (serializedDeleteRequest != NULL) {
        delete[] serializedDeleteRequest;
    }
    if (serializedPutRequest != NULL) {
        delete[] serializedPutRequest;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString WriteRequest::jsonSerialize() const {
    int itemCount = 0;
    if (deleteRequestBeenSet) {
        itemCount++;
    }
    if (putRequestBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (deleteRequestBeenSet) {
        MinimalString pairValue = deleteRequest.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(DELETEREQUEST_KEY, pairValue);
    }
    if (putRequestBeenSet) {
        MinimalString pairValue = putRequest.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PUTREQUEST_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void WriteRequest::setDeleteRequest(DeleteRequest deleteRequest) {
    deleteRequestBeenSet = true;
    this->deleteRequest = deleteRequest;
}

void WriteRequest::setPutRequest(PutRequest putRequest) {
    putRequestBeenSet = true;
    this->putRequest = putRequest;
}

DeleteRequest WriteRequest::getDeleteRequest() const {
    return deleteRequest;
}

PutRequest WriteRequest::getPutRequest() const {
    return putRequest;
}

KeysAndAttributes::KeysAndAttributes() {
    reset();
}

void KeysAndAttributes::reset() {
    consistentReadBeenSet = false;
    attributesToGetBeenSet = false;
    keysBeenSet = false;
}

bool KeysAndAttributes::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedConsistentRead = jsmnGetVal(CONSISTENTREAD_KEY, jsonCStr, tokens, tCount);
    char* serializedAttributesToGet = jsmnGetVal(ATTRIBUTESTOGET_KEY, jsonCStr, tokens, tCount);
    char* serializedKeys = jsmnGetVal(KEYS_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedConsistentRead != NULL) {
        consistentReadBeenSet = true;
        jsonDeserializeBool(serializedConsistentRead);
    }
    if (serializedAttributesToGet != NULL) {
        attributesToGetBeenSet = true;
        if (!attributesToGet.jsonDeserialize(serializedAttributesToGet)) {
            toReturn = false;
        }
    }
    if (serializedKeys != NULL) {
        keysBeenSet = true;
        if (!keys.jsonDeserialize(serializedKeys)) {
            toReturn = false;
        }
    }
    if (serializedConsistentRead != NULL) {
        delete[] serializedConsistentRead;
    }
    if (serializedAttributesToGet != NULL) {
        delete[] serializedAttributesToGet;
    }
    if (serializedKeys != NULL) {
        delete[] serializedKeys;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString KeysAndAttributes::jsonSerialize() const {
    int itemCount = 0;
    if (consistentReadBeenSet) {
        itemCount++;
    }
    if (attributesToGetBeenSet) {
        itemCount++;
    }
    if (keysBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (consistentReadBeenSet) {
        MinimalString pairValue = jsonSerializeBool(consistentRead);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONSISTENTREAD_KEY, pairValue);
    }
    if (attributesToGetBeenSet) {
        MinimalString pairValue = attributesToGet.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTESTOGET_KEY, pairValue);
    }
    if (keysBeenSet) {
        MinimalString pairValue = keys.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYS_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void KeysAndAttributes::setConsistentRead(bool consistentRead) {
    consistentReadBeenSet = true;
    this->consistentRead = consistentRead;
}

void KeysAndAttributes::setAttributesToGet(MinimalList<MinimalString > attributesToGet) {
    attributesToGetBeenSet = true;
    this->attributesToGet = attributesToGet;
}

void KeysAndAttributes::setKeys(MinimalList<MinimalMap<AttributeValue > > keys) {
    keysBeenSet = true;
    this->keys = keys;
}

bool KeysAndAttributes::getConsistentRead() const {
    return consistentRead;
}

MinimalList<MinimalString > KeysAndAttributes::getAttributesToGet() const {
    return attributesToGet;
}

MinimalList<MinimalMap<AttributeValue > > KeysAndAttributes::getKeys() const {
    return keys;
}

LocalSecondaryIndex::LocalSecondaryIndex() {
    reset();
}

void LocalSecondaryIndex::reset() {
    projectionBeenSet = false;
    keySchemaBeenSet = false;
    indexNameBeenSet = false;
}

bool LocalSecondaryIndex::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedProjection = jsmnGetVal(PROJECTION_KEY, jsonCStr, tokens, tCount);
    char* serializedKeySchema = jsmnGetVal(KEYSCHEMA_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexName = jsmnGetVal(INDEXNAME_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedProjection != NULL) {
        projectionBeenSet = true;
        if (!projection.jsonDeserialize(serializedProjection)) {
            toReturn = false;
        }
    }
    if (serializedKeySchema != NULL) {
        keySchemaBeenSet = true;
        if (!keySchema.jsonDeserialize(serializedKeySchema)) {
            toReturn = false;
        }
    }
    if (serializedIndexName != NULL) {
        indexNameBeenSet = true;
        indexName = MinimalString(serializedIndexName);
    }
    if (serializedProjection != NULL) {
        delete[] serializedProjection;
    }
    if (serializedKeySchema != NULL) {
        delete[] serializedKeySchema;
    }
    if (serializedIndexName != NULL) {
        delete[] serializedIndexName;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString LocalSecondaryIndex::jsonSerialize() const {
    int itemCount = 0;
    if (projectionBeenSet) {
        itemCount++;
    }
    if (keySchemaBeenSet) {
        itemCount++;
    }
    if (indexNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (projectionBeenSet) {
        MinimalString pairValue = projection.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROJECTION_KEY, pairValue);
    }
    if (keySchemaBeenSet) {
        MinimalString pairValue = keySchema.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYSCHEMA_KEY, pairValue);
    }
    if (indexNameBeenSet) {
        MinimalString pairValue = indexName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXNAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void LocalSecondaryIndex::setProjection(Projection projection) {
    projectionBeenSet = true;
    this->projection = projection;
}

void LocalSecondaryIndex::setKeySchema(MinimalList<KeySchemaElement > keySchema) {
    keySchemaBeenSet = true;
    this->keySchema = keySchema;
}

void LocalSecondaryIndex::setIndexName(MinimalString indexName) {
    indexNameBeenSet = true;
    this->indexName = indexName;
}

Projection LocalSecondaryIndex::getProjection() const {
    return projection;
}

MinimalList<KeySchemaElement > LocalSecondaryIndex::getKeySchema() const {
    return keySchema;
}

MinimalString LocalSecondaryIndex::getIndexName() const {
    return indexName;
}

GlobalSecondaryIndex::GlobalSecondaryIndex() {
    reset();
}

void GlobalSecondaryIndex::reset() {
    projectionBeenSet = false;
    provisionedThroughputBeenSet = false;
    keySchemaBeenSet = false;
    indexNameBeenSet = false;
}

bool GlobalSecondaryIndex::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedProjection = jsmnGetVal(PROJECTION_KEY, jsonCStr, tokens, tCount);
    char* serializedProvisionedThroughput = jsmnGetVal(PROVISIONEDTHROUGHPUT_KEY, jsonCStr, tokens, tCount);
    char* serializedKeySchema = jsmnGetVal(KEYSCHEMA_KEY, jsonCStr, tokens, tCount);
    char* serializedIndexName = jsmnGetVal(INDEXNAME_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedProjection != NULL) {
        projectionBeenSet = true;
        if (!projection.jsonDeserialize(serializedProjection)) {
            toReturn = false;
        }
    }
    if (serializedProvisionedThroughput != NULL) {
        provisionedThroughputBeenSet = true;
        if (!provisionedThroughput.jsonDeserialize(serializedProvisionedThroughput)) {
            toReturn = false;
        }
    }
    if (serializedKeySchema != NULL) {
        keySchemaBeenSet = true;
        if (!keySchema.jsonDeserialize(serializedKeySchema)) {
            toReturn = false;
        }
    }
    if (serializedIndexName != NULL) {
        indexNameBeenSet = true;
        indexName = MinimalString(serializedIndexName);
    }
    if (serializedProjection != NULL) {
        delete[] serializedProjection;
    }
    if (serializedProvisionedThroughput != NULL) {
        delete[] serializedProvisionedThroughput;
    }
    if (serializedKeySchema != NULL) {
        delete[] serializedKeySchema;
    }
    if (serializedIndexName != NULL) {
        delete[] serializedIndexName;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString GlobalSecondaryIndex::jsonSerialize() const {
    int itemCount = 0;
    if (projectionBeenSet) {
        itemCount++;
    }
    if (provisionedThroughputBeenSet) {
        itemCount++;
    }
    if (keySchemaBeenSet) {
        itemCount++;
    }
    if (indexNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (projectionBeenSet) {
        MinimalString pairValue = projection.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROJECTION_KEY, pairValue);
    }
    if (provisionedThroughputBeenSet) {
        MinimalString pairValue = provisionedThroughput.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROVISIONEDTHROUGHPUT_KEY, pairValue);
    }
    if (keySchemaBeenSet) {
        MinimalString pairValue = keySchema.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYSCHEMA_KEY, pairValue);
    }
    if (indexNameBeenSet) {
        MinimalString pairValue = indexName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXNAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void GlobalSecondaryIndex::setProjection(Projection projection) {
    projectionBeenSet = true;
    this->projection = projection;
}

void GlobalSecondaryIndex::setProvisionedThroughput(ProvisionedThroughput provisionedThroughput) {
    provisionedThroughputBeenSet = true;
    this->provisionedThroughput = provisionedThroughput;
}

void GlobalSecondaryIndex::setKeySchema(MinimalList<KeySchemaElement > keySchema) {
    keySchemaBeenSet = true;
    this->keySchema = keySchema;
}

void GlobalSecondaryIndex::setIndexName(MinimalString indexName) {
    indexNameBeenSet = true;
    this->indexName = indexName;
}

Projection GlobalSecondaryIndex::getProjection() const {
    return projection;
}

ProvisionedThroughput GlobalSecondaryIndex::getProvisionedThroughput() const {
    return provisionedThroughput;
}

MinimalList<KeySchemaElement > GlobalSecondaryIndex::getKeySchema() const {
    return keySchema;
}

MinimalString GlobalSecondaryIndex::getIndexName() const {
    return indexName;
}

AttributeValueUpdate::AttributeValueUpdate() {
    reset();
}

void AttributeValueUpdate::reset() {
    valueBeenSet = false;
    actionBeenSet = false;
}

bool AttributeValueUpdate::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedValue = jsmnGetVal(VALUE_KEY, jsonCStr, tokens, tCount);
    char* serializedAction = jsmnGetVal(ACTION_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedValue != NULL) {
        valueBeenSet = true;
        if (!value.jsonDeserialize(serializedValue)) {
            toReturn = false;
        }
    }
    if (serializedAction != NULL) {
        actionBeenSet = true;
        int enumInt = jsonDeserializeEnum(ATTRIBUTEACTION_LOOKUP, 3, MinimalString(serializedAction));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedValue != NULL) {
        delete[] serializedValue;
    }
    if (serializedAction != NULL) {
        delete[] serializedAction;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString AttributeValueUpdate::jsonSerialize() const {
    int itemCount = 0;
    if (valueBeenSet) {
        itemCount++;
    }
    if (actionBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (valueBeenSet) {
        MinimalString pairValue = value.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(VALUE_KEY, pairValue);
    }
    if (actionBeenSet) {
        MinimalString pairValue = MinimalString(ATTRIBUTEACTION_LOOKUP[action]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ACTION_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void AttributeValueUpdate::setValue(AttributeValue value) {
    valueBeenSet = true;
    this->value = value;
}

void AttributeValueUpdate::setAction(AttributeAction action) {
    actionBeenSet = true;
    this->action = action;
}

AttributeValue AttributeValueUpdate::getValue() const {
    return value;
}

AttributeAction AttributeValueUpdate::getAction() const {
    return action;
}

GlobalSecondaryIndexUpdate::GlobalSecondaryIndexUpdate() {
    reset();
}

void GlobalSecondaryIndexUpdate::reset() {
    updateBeenSet = false;
}

bool GlobalSecondaryIndexUpdate::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedUpdate = jsmnGetVal(UPDATE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedUpdate != NULL) {
        updateBeenSet = true;
        if (!update.jsonDeserialize(serializedUpdate)) {
            toReturn = false;
        }
    }
    if (serializedUpdate != NULL) {
        delete[] serializedUpdate;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString GlobalSecondaryIndexUpdate::jsonSerialize() const {
    int itemCount = 0;
    if (updateBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (updateBeenSet) {
        MinimalString pairValue = update.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(UPDATE_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void GlobalSecondaryIndexUpdate::setUpdate(UpdateGlobalSecondaryIndexAction update) {
    updateBeenSet = true;
    this->update = update;
}

UpdateGlobalSecondaryIndexAction GlobalSecondaryIndexUpdate::getUpdate() const {
    return update;
}

ListTablesOutput::ListTablesOutput() {
    reset();
}

void ListTablesOutput::reset() {
    lastEvaluatedTableNameBeenSet = false;
    tableNamesBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool ListTablesOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedLastEvaluatedTableName = jsmnGetVal(LASTEVALUATEDTABLENAME_KEY, jsonCStr, tokens, tCount);
    char* serializedTableNames = jsmnGetVal(TABLENAMES_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedLastEvaluatedTableName != NULL) {
        lastEvaluatedTableNameBeenSet = true;
        lastEvaluatedTableName = MinimalString(serializedLastEvaluatedTableName);
    }
    if (serializedTableNames != NULL) {
        tableNamesBeenSet = true;
        if (!tableNames.jsonDeserialize(serializedTableNames)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedLastEvaluatedTableName != NULL) {
        delete[] serializedLastEvaluatedTableName;
    }
    if (serializedTableNames != NULL) {
        delete[] serializedTableNames;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString ListTablesOutput::getErrorType() const {
    return errorType;
}

MinimalString ListTablesOutput::getErrorMessage() const {
    return errorMessage;
}

void ListTablesOutput::setLastEvaluatedTableName(MinimalString lastEvaluatedTableName) {
    lastEvaluatedTableNameBeenSet = true;
    this->lastEvaluatedTableName = lastEvaluatedTableName;
}

void ListTablesOutput::setTableNames(MinimalList<MinimalString > tableNames) {
    tableNamesBeenSet = true;
    this->tableNames = tableNames;
}

MinimalString ListTablesOutput::getLastEvaluatedTableName() const {
    return lastEvaluatedTableName;
}

MinimalList<MinimalString > ListTablesOutput::getTableNames() const {
    return tableNames;
}

UpdateTableOutput::UpdateTableOutput() {
    reset();
}

void UpdateTableOutput::reset() {
    tableDescriptionBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool UpdateTableOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedTableDescription = jsmnGetVal(TABLEDESCRIPTION_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedTableDescription != NULL) {
        tableDescriptionBeenSet = true;
        if (!tableDescription.jsonDeserialize(serializedTableDescription)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedTableDescription != NULL) {
        delete[] serializedTableDescription;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString UpdateTableOutput::getErrorType() const {
    return errorType;
}

MinimalString UpdateTableOutput::getErrorMessage() const {
    return errorMessage;
}

void UpdateTableOutput::setTableDescription(TableDescription tableDescription) {
    tableDescriptionBeenSet = true;
    this->tableDescription = tableDescription;
}

TableDescription UpdateTableOutput::getTableDescription() const {
    return tableDescription;
}

UpdateItemOutput::UpdateItemOutput() {
    reset();
}

void UpdateItemOutput::reset() {
    attributesBeenSet = false;
    itemCollectionMetricsBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool UpdateItemOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedAttributes = jsmnGetVal(ATTRIBUTES_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCollectionMetrics = jsmnGetVal(ITEMCOLLECTIONMETRICS_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedAttributes != NULL) {
        attributesBeenSet = true;
        if (!attributes.jsonDeserialize(serializedAttributes)) {
            toReturn = false;
        }
    }
    if (serializedItemCollectionMetrics != NULL) {
        itemCollectionMetricsBeenSet = true;
        if (!itemCollectionMetrics.jsonDeserialize(serializedItemCollectionMetrics)) {
            toReturn = false;
        }
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedAttributes != NULL) {
        delete[] serializedAttributes;
    }
    if (serializedItemCollectionMetrics != NULL) {
        delete[] serializedItemCollectionMetrics;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString UpdateItemOutput::getErrorType() const {
    return errorType;
}

MinimalString UpdateItemOutput::getErrorMessage() const {
    return errorMessage;
}

void UpdateItemOutput::setAttributes(MinimalMap<AttributeValue > attributes) {
    attributesBeenSet = true;
    this->attributes = attributes;
}

void UpdateItemOutput::setItemCollectionMetrics(ItemCollectionMetrics itemCollectionMetrics) {
    itemCollectionMetricsBeenSet = true;
    this->itemCollectionMetrics = itemCollectionMetrics;
}

void UpdateItemOutput::setConsumedCapacity(ConsumedCapacity consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<AttributeValue > UpdateItemOutput::getAttributes() const {
    return attributes;
}

ItemCollectionMetrics UpdateItemOutput::getItemCollectionMetrics() const {
    return itemCollectionMetrics;
}

ConsumedCapacity UpdateItemOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

DeleteItemInput::DeleteItemInput() {
    reset();
}

void DeleteItemInput::reset() {
    returnItemCollectionMetricsBeenSet = false;
    returnValuesBeenSet = false;
    keyBeenSet = false;
    conditionalOperatorBeenSet = false;
    expectedBeenSet = false;
    tableNameBeenSet = false;
    returnConsumedCapacityBeenSet = false;
}

bool DeleteItemInput::requiredAreSet() const {
    return tableNameBeenSet && keyBeenSet;
}

MinimalString DeleteItemInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (returnItemCollectionMetricsBeenSet) {
        itemCount++;
    }
    if (returnValuesBeenSet) {
        itemCount++;
    }
    if (keyBeenSet) {
        itemCount++;
    }
    if (conditionalOperatorBeenSet) {
        itemCount++;
    }
    if (expectedBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (returnItemCollectionMetricsBeenSet) {
        MinimalString pairValue = MinimalString(RETURNITEMCOLLECTIONMETRICS_LOOKUP[returnItemCollectionMetrics]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNITEMCOLLECTIONMETRICS_KEY, pairValue);
    }
    if (returnValuesBeenSet) {
        MinimalString pairValue = MinimalString(RETURNVALUE_LOOKUP[returnValues]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNVALUES_KEY, pairValue);
    }
    if (keyBeenSet) {
        MinimalString pairValue = key.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEY_KEY, pairValue);
    }
    if (conditionalOperatorBeenSet) {
        MinimalString pairValue = MinimalString(CONDITIONALOPERATOR_LOOKUP[conditionalOperator]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONDITIONALOPERATOR_KEY, pairValue);
    }
    if (expectedBeenSet) {
        MinimalString pairValue = expected.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXPECTED_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void DeleteItemInput::setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics) {
    returnItemCollectionMetricsBeenSet = true;
    this->returnItemCollectionMetrics = returnItemCollectionMetrics;
}

void DeleteItemInput::setReturnValues(ReturnValue returnValues) {
    returnValuesBeenSet = true;
    this->returnValues = returnValues;
}

void DeleteItemInput::setKey(MinimalMap<AttributeValue > key) {
    keyBeenSet = true;
    this->key = key;
}

void DeleteItemInput::setConditionalOperator(ConditionalOperator conditionalOperator) {
    conditionalOperatorBeenSet = true;
    this->conditionalOperator = conditionalOperator;
}

void DeleteItemInput::setExpected(MinimalMap<ExpectedAttributeValue > expected) {
    expectedBeenSet = true;
    this->expected = expected;
}

void DeleteItemInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void DeleteItemInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

ReturnItemCollectionMetrics DeleteItemInput::getReturnItemCollectionMetrics() const {
    return returnItemCollectionMetrics;
}

ReturnValue DeleteItemInput::getReturnValues() const {
    return returnValues;
}

MinimalMap<AttributeValue > DeleteItemInput::getKey() const {
    return key;
}

ConditionalOperator DeleteItemInput::getConditionalOperator() const {
    return conditionalOperator;
}

MinimalMap<ExpectedAttributeValue > DeleteItemInput::getExpected() const {
    return expected;
}

MinimalString DeleteItemInput::getTableName() const {
    return tableName;
}

ReturnConsumedCapacity DeleteItemInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

ScanInput::ScanInput() {
    reset();
}

void ScanInput::reset() {
    scanFilterBeenSet = false;
    selectBeenSet = false;
    totalSegmentsBeenSet = false;
    conditionalOperatorBeenSet = false;
    segmentBeenSet = false;
    attributesToGetBeenSet = false;
    tableNameBeenSet = false;
    returnConsumedCapacityBeenSet = false;
    limitBeenSet = false;
    exclusiveStartKeyBeenSet = false;
}

bool ScanInput::requiredAreSet() const {
    return tableNameBeenSet;
}

MinimalString ScanInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (scanFilterBeenSet) {
        itemCount++;
    }
    if (selectBeenSet) {
        itemCount++;
    }
    if (totalSegmentsBeenSet) {
        itemCount++;
    }
    if (conditionalOperatorBeenSet) {
        itemCount++;
    }
    if (segmentBeenSet) {
        itemCount++;
    }
    if (attributesToGetBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }
    if (limitBeenSet) {
        itemCount++;
    }
    if (exclusiveStartKeyBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (scanFilterBeenSet) {
        MinimalString pairValue = scanFilter.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SCANFILTER_KEY, pairValue);
    }
    if (selectBeenSet) {
        MinimalString pairValue = MinimalString(SELECT_LOOKUP[select]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SELECT_KEY, pairValue);
    }
    if (totalSegmentsBeenSet) {
        MinimalString pairValue = jsonSerializeInt(totalSegments);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TOTALSEGMENTS_KEY, pairValue);
    }
    if (conditionalOperatorBeenSet) {
        MinimalString pairValue = MinimalString(CONDITIONALOPERATOR_LOOKUP[conditionalOperator]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONDITIONALOPERATOR_KEY, pairValue);
    }
    if (segmentBeenSet) {
        MinimalString pairValue = jsonSerializeInt(segment);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SEGMENT_KEY, pairValue);
    }
    if (attributesToGetBeenSet) {
        MinimalString pairValue = attributesToGet.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTESTOGET_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }
    if (limitBeenSet) {
        MinimalString pairValue = jsonSerializeInt(limit);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LIMIT_KEY, pairValue);
    }
    if (exclusiveStartKeyBeenSet) {
        MinimalString pairValue = exclusiveStartKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXCLUSIVESTARTKEY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void ScanInput::setScanFilter(MinimalMap<Condition > scanFilter) {
    scanFilterBeenSet = true;
    this->scanFilter = scanFilter;
}

void ScanInput::setSelect(Select select) {
    selectBeenSet = true;
    this->select = select;
}

void ScanInput::setTotalSegments(int totalSegments) {
    totalSegmentsBeenSet = true;
    this->totalSegments = totalSegments;
}

void ScanInput::setConditionalOperator(ConditionalOperator conditionalOperator) {
    conditionalOperatorBeenSet = true;
    this->conditionalOperator = conditionalOperator;
}

void ScanInput::setSegment(int segment) {
    segmentBeenSet = true;
    this->segment = segment;
}

void ScanInput::setAttributesToGet(MinimalList<MinimalString > attributesToGet) {
    attributesToGetBeenSet = true;
    this->attributesToGet = attributesToGet;
}

void ScanInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void ScanInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

void ScanInput::setLimit(int limit) {
    limitBeenSet = true;
    this->limit = limit;
}

void ScanInput::setExclusiveStartKey(MinimalMap<AttributeValue > exclusiveStartKey) {
    exclusiveStartKeyBeenSet = true;
    this->exclusiveStartKey = exclusiveStartKey;
}

MinimalMap<Condition > ScanInput::getScanFilter() const {
    return scanFilter;
}

Select ScanInput::getSelect() const {
    return select;
}

int ScanInput::getTotalSegments() const {
    return totalSegments;
}

ConditionalOperator ScanInput::getConditionalOperator() const {
    return conditionalOperator;
}

int ScanInput::getSegment() const {
    return segment;
}

MinimalList<MinimalString > ScanInput::getAttributesToGet() const {
    return attributesToGet;
}

MinimalString ScanInput::getTableName() const {
    return tableName;
}

ReturnConsumedCapacity ScanInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

int ScanInput::getLimit() const {
    return limit;
}

MinimalMap<AttributeValue > ScanInput::getExclusiveStartKey() const {
    return exclusiveStartKey;
}

ListTablesInput::ListTablesInput() {
    reset();
}

void ListTablesInput::reset() {
    exclusiveStartTableNameBeenSet = false;
    limitBeenSet = false;
}

bool ListTablesInput::requiredAreSet() const {
    return true;
}

MinimalString ListTablesInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (exclusiveStartTableNameBeenSet) {
        itemCount++;
    }
    if (limitBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (exclusiveStartTableNameBeenSet) {
        MinimalString pairValue = exclusiveStartTableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXCLUSIVESTARTTABLENAME_KEY, pairValue);
    }
    if (limitBeenSet) {
        MinimalString pairValue = jsonSerializeInt(limit);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LIMIT_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void ListTablesInput::setExclusiveStartTableName(MinimalString exclusiveStartTableName) {
    exclusiveStartTableNameBeenSet = true;
    this->exclusiveStartTableName = exclusiveStartTableName;
}

void ListTablesInput::setLimit(int limit) {
    limitBeenSet = true;
    this->limit = limit;
}

MinimalString ListTablesInput::getExclusiveStartTableName() const {
    return exclusiveStartTableName;
}

int ListTablesInput::getLimit() const {
    return limit;
}

BatchWriteItemOutput::BatchWriteItemOutput() {
    reset();
}

void BatchWriteItemOutput::reset() {
    unprocessedItemsBeenSet = false;
    itemCollectionMetricsBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool BatchWriteItemOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedUnprocessedItems = jsmnGetVal(UNPROCESSEDITEMS_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCollectionMetrics = jsmnGetVal(ITEMCOLLECTIONMETRICS_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedUnprocessedItems != NULL) {
        unprocessedItemsBeenSet = true;
        if (!unprocessedItems.jsonDeserialize(serializedUnprocessedItems)) {
            toReturn = false;
        }
    }
    if (serializedItemCollectionMetrics != NULL) {
        itemCollectionMetricsBeenSet = true;
        if (!itemCollectionMetrics.jsonDeserialize(serializedItemCollectionMetrics)) {
            toReturn = false;
        }
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedUnprocessedItems != NULL) {
        delete[] serializedUnprocessedItems;
    }
    if (serializedItemCollectionMetrics != NULL) {
        delete[] serializedItemCollectionMetrics;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString BatchWriteItemOutput::getErrorType() const {
    return errorType;
}

MinimalString BatchWriteItemOutput::getErrorMessage() const {
    return errorMessage;
}

void BatchWriteItemOutput::setUnprocessedItems(MinimalMap<MinimalList<WriteRequest > > unprocessedItems) {
    unprocessedItemsBeenSet = true;
    this->unprocessedItems = unprocessedItems;
}

void BatchWriteItemOutput::setItemCollectionMetrics(MinimalMap<MinimalList<ItemCollectionMetrics > > itemCollectionMetrics) {
    itemCollectionMetricsBeenSet = true;
    this->itemCollectionMetrics = itemCollectionMetrics;
}

void BatchWriteItemOutput::setConsumedCapacity(MinimalList<ConsumedCapacity > consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<MinimalList<WriteRequest > > BatchWriteItemOutput::getUnprocessedItems() const {
    return unprocessedItems;
}

MinimalMap<MinimalList<ItemCollectionMetrics > > BatchWriteItemOutput::getItemCollectionMetrics() const {
    return itemCollectionMetrics;
}

MinimalList<ConsumedCapacity > BatchWriteItemOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

QueryInput::QueryInput() {
    reset();
}

void QueryInput::reset() {
    scanIndexForwardBeenSet = false;
    selectBeenSet = false;
    consistentReadBeenSet = false;
    conditionalOperatorBeenSet = false;
    queryFilterBeenSet = false;
    attributesToGetBeenSet = false;
    keyConditionsBeenSet = false;
    tableNameBeenSet = false;
    indexNameBeenSet = false;
    returnConsumedCapacityBeenSet = false;
    limitBeenSet = false;
    exclusiveStartKeyBeenSet = false;
}

bool QueryInput::requiredAreSet() const {
    return tableNameBeenSet && keyConditionsBeenSet;
}

MinimalString QueryInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (scanIndexForwardBeenSet) {
        itemCount++;
    }
    if (selectBeenSet) {
        itemCount++;
    }
    if (consistentReadBeenSet) {
        itemCount++;
    }
    if (conditionalOperatorBeenSet) {
        itemCount++;
    }
    if (queryFilterBeenSet) {
        itemCount++;
    }
    if (attributesToGetBeenSet) {
        itemCount++;
    }
    if (keyConditionsBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (indexNameBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }
    if (limitBeenSet) {
        itemCount++;
    }
    if (exclusiveStartKeyBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (scanIndexForwardBeenSet) {
        MinimalString pairValue = jsonSerializeBool(scanIndexForward);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SCANINDEXFORWARD_KEY, pairValue);
    }
    if (selectBeenSet) {
        MinimalString pairValue = MinimalString(SELECT_LOOKUP[select]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SELECT_KEY, pairValue);
    }
    if (consistentReadBeenSet) {
        MinimalString pairValue = jsonSerializeBool(consistentRead);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONSISTENTREAD_KEY, pairValue);
    }
    if (conditionalOperatorBeenSet) {
        MinimalString pairValue = MinimalString(CONDITIONALOPERATOR_LOOKUP[conditionalOperator]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONDITIONALOPERATOR_KEY, pairValue);
    }
    if (queryFilterBeenSet) {
        MinimalString pairValue = queryFilter.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(QUERYFILTER_KEY, pairValue);
    }
    if (attributesToGetBeenSet) {
        MinimalString pairValue = attributesToGet.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTESTOGET_KEY, pairValue);
    }
    if (keyConditionsBeenSet) {
        MinimalString pairValue = keyConditions.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYCONDITIONS_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (indexNameBeenSet) {
        MinimalString pairValue = indexName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(INDEXNAME_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }
    if (limitBeenSet) {
        MinimalString pairValue = jsonSerializeInt(limit);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LIMIT_KEY, pairValue);
    }
    if (exclusiveStartKeyBeenSet) {
        MinimalString pairValue = exclusiveStartKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXCLUSIVESTARTKEY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void QueryInput::setScanIndexForward(bool scanIndexForward) {
    scanIndexForwardBeenSet = true;
    this->scanIndexForward = scanIndexForward;
}

void QueryInput::setSelect(Select select) {
    selectBeenSet = true;
    this->select = select;
}

void QueryInput::setConsistentRead(bool consistentRead) {
    consistentReadBeenSet = true;
    this->consistentRead = consistentRead;
}

void QueryInput::setConditionalOperator(ConditionalOperator conditionalOperator) {
    conditionalOperatorBeenSet = true;
    this->conditionalOperator = conditionalOperator;
}

void QueryInput::setQueryFilter(MinimalMap<Condition > queryFilter) {
    queryFilterBeenSet = true;
    this->queryFilter = queryFilter;
}

void QueryInput::setAttributesToGet(MinimalList<MinimalString > attributesToGet) {
    attributesToGetBeenSet = true;
    this->attributesToGet = attributesToGet;
}

void QueryInput::setKeyConditions(MinimalMap<Condition > keyConditions) {
    keyConditionsBeenSet = true;
    this->keyConditions = keyConditions;
}

void QueryInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void QueryInput::setIndexName(MinimalString indexName) {
    indexNameBeenSet = true;
    this->indexName = indexName;
}

void QueryInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

void QueryInput::setLimit(int limit) {
    limitBeenSet = true;
    this->limit = limit;
}

void QueryInput::setExclusiveStartKey(MinimalMap<AttributeValue > exclusiveStartKey) {
    exclusiveStartKeyBeenSet = true;
    this->exclusiveStartKey = exclusiveStartKey;
}

bool QueryInput::getScanIndexForward() const {
    return scanIndexForward;
}

Select QueryInput::getSelect() const {
    return select;
}

bool QueryInput::getConsistentRead() const {
    return consistentRead;
}

ConditionalOperator QueryInput::getConditionalOperator() const {
    return conditionalOperator;
}

MinimalMap<Condition > QueryInput::getQueryFilter() const {
    return queryFilter;
}

MinimalList<MinimalString > QueryInput::getAttributesToGet() const {
    return attributesToGet;
}

MinimalMap<Condition > QueryInput::getKeyConditions() const {
    return keyConditions;
}

MinimalString QueryInput::getTableName() const {
    return tableName;
}

MinimalString QueryInput::getIndexName() const {
    return indexName;
}

ReturnConsumedCapacity QueryInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

int QueryInput::getLimit() const {
    return limit;
}

MinimalMap<AttributeValue > QueryInput::getExclusiveStartKey() const {
    return exclusiveStartKey;
}

BatchWriteItemInput::BatchWriteItemInput() {
    reset();
}

void BatchWriteItemInput::reset() {
    returnItemCollectionMetricsBeenSet = false;
    requestItemsBeenSet = false;
    returnConsumedCapacityBeenSet = false;
}

bool BatchWriteItemInput::requiredAreSet() const {
    return requestItemsBeenSet;
}

MinimalString BatchWriteItemInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (returnItemCollectionMetricsBeenSet) {
        itemCount++;
    }
    if (requestItemsBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (returnItemCollectionMetricsBeenSet) {
        MinimalString pairValue = MinimalString(RETURNITEMCOLLECTIONMETRICS_LOOKUP[returnItemCollectionMetrics]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNITEMCOLLECTIONMETRICS_KEY, pairValue);
    }
    if (requestItemsBeenSet) {
        MinimalString pairValue = requestItems.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(REQUESTITEMS_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void BatchWriteItemInput::setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics) {
    returnItemCollectionMetricsBeenSet = true;
    this->returnItemCollectionMetrics = returnItemCollectionMetrics;
}

void BatchWriteItemInput::setRequestItems(MinimalMap<MinimalList<WriteRequest > > requestItems) {
    requestItemsBeenSet = true;
    this->requestItems = requestItems;
}

void BatchWriteItemInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

ReturnItemCollectionMetrics BatchWriteItemInput::getReturnItemCollectionMetrics() const {
    return returnItemCollectionMetrics;
}

MinimalMap<MinimalList<WriteRequest > > BatchWriteItemInput::getRequestItems() const {
    return requestItems;
}

ReturnConsumedCapacity BatchWriteItemInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

QueryOutput::QueryOutput() {
    reset();
}

void QueryOutput::reset() {
    lastEvaluatedKeyBeenSet = false;
    itemsBeenSet = false;
    countBeenSet = false;
    scannedCountBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool QueryOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedLastEvaluatedKey = jsmnGetVal(LASTEVALUATEDKEY_KEY, jsonCStr, tokens, tCount);
    char* serializedItems = jsmnGetVal(ITEMS_KEY, jsonCStr, tokens, tCount);
    char* serializedCount = jsmnGetVal(COUNT_KEY, jsonCStr, tokens, tCount);
    char* serializedScannedCount = jsmnGetVal(SCANNEDCOUNT_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedLastEvaluatedKey != NULL) {
        lastEvaluatedKeyBeenSet = true;
        if (!lastEvaluatedKey.jsonDeserialize(serializedLastEvaluatedKey)) {
            toReturn = false;
        }
    }
    if (serializedItems != NULL) {
        itemsBeenSet = true;
        if (!items.jsonDeserialize(serializedItems)) {
            toReturn = false;
        }
    }
    if (serializedCount != NULL) {
        countBeenSet = true;
        count = atoi(serializedCount);
    }
    if (serializedScannedCount != NULL) {
        scannedCountBeenSet = true;
        scannedCount = atoi(serializedScannedCount);
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedLastEvaluatedKey != NULL) {
        delete[] serializedLastEvaluatedKey;
    }
    if (serializedItems != NULL) {
        delete[] serializedItems;
    }
    if (serializedCount != NULL) {
        delete[] serializedCount;
    }
    if (serializedScannedCount != NULL) {
        delete[] serializedScannedCount;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString QueryOutput::getErrorType() const {
    return errorType;
}

MinimalString QueryOutput::getErrorMessage() const {
    return errorMessage;
}

void QueryOutput::setLastEvaluatedKey(MinimalMap<AttributeValue > lastEvaluatedKey) {
    lastEvaluatedKeyBeenSet = true;
    this->lastEvaluatedKey = lastEvaluatedKey;
}

void QueryOutput::setItems(MinimalList<MinimalMap<AttributeValue > > items) {
    itemsBeenSet = true;
    this->items = items;
}

void QueryOutput::setCount(int count) {
    countBeenSet = true;
    this->count = count;
}

void QueryOutput::setScannedCount(int scannedCount) {
    scannedCountBeenSet = true;
    this->scannedCount = scannedCount;
}

void QueryOutput::setConsumedCapacity(ConsumedCapacity consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<AttributeValue > QueryOutput::getLastEvaluatedKey() const {
    return lastEvaluatedKey;
}

MinimalList<MinimalMap<AttributeValue > > QueryOutput::getItems() const {
    return items;
}

int QueryOutput::getCount() const {
    return count;
}

int QueryOutput::getScannedCount() const {
    return scannedCount;
}

ConsumedCapacity QueryOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

DeleteItemOutput::DeleteItemOutput() {
    reset();
}

void DeleteItemOutput::reset() {
    attributesBeenSet = false;
    itemCollectionMetricsBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool DeleteItemOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedAttributes = jsmnGetVal(ATTRIBUTES_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCollectionMetrics = jsmnGetVal(ITEMCOLLECTIONMETRICS_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedAttributes != NULL) {
        attributesBeenSet = true;
        if (!attributes.jsonDeserialize(serializedAttributes)) {
            toReturn = false;
        }
    }
    if (serializedItemCollectionMetrics != NULL) {
        itemCollectionMetricsBeenSet = true;
        if (!itemCollectionMetrics.jsonDeserialize(serializedItemCollectionMetrics)) {
            toReturn = false;
        }
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedAttributes != NULL) {
        delete[] serializedAttributes;
    }
    if (serializedItemCollectionMetrics != NULL) {
        delete[] serializedItemCollectionMetrics;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString DeleteItemOutput::getErrorType() const {
    return errorType;
}

MinimalString DeleteItemOutput::getErrorMessage() const {
    return errorMessage;
}

void DeleteItemOutput::setAttributes(MinimalMap<AttributeValue > attributes) {
    attributesBeenSet = true;
    this->attributes = attributes;
}

void DeleteItemOutput::setItemCollectionMetrics(ItemCollectionMetrics itemCollectionMetrics) {
    itemCollectionMetricsBeenSet = true;
    this->itemCollectionMetrics = itemCollectionMetrics;
}

void DeleteItemOutput::setConsumedCapacity(ConsumedCapacity consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<AttributeValue > DeleteItemOutput::getAttributes() const {
    return attributes;
}

ItemCollectionMetrics DeleteItemOutput::getItemCollectionMetrics() const {
    return itemCollectionMetrics;
}

ConsumedCapacity DeleteItemOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

BatchGetItemOutput::BatchGetItemOutput() {
    reset();
}

void BatchGetItemOutput::reset() {
    responsesBeenSet = false;
    unprocessedKeysBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool BatchGetItemOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedResponses = jsmnGetVal(RESPONSES_KEY, jsonCStr, tokens, tCount);
    char* serializedUnprocessedKeys = jsmnGetVal(UNPROCESSEDKEYS_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedResponses != NULL) {
        responsesBeenSet = true;
        if (!responses.jsonDeserialize(serializedResponses)) {
            toReturn = false;
        }
    }
    if (serializedUnprocessedKeys != NULL) {
        unprocessedKeysBeenSet = true;
        if (!unprocessedKeys.jsonDeserialize(serializedUnprocessedKeys)) {
            toReturn = false;
        }
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedResponses != NULL) {
        delete[] serializedResponses;
    }
    if (serializedUnprocessedKeys != NULL) {
        delete[] serializedUnprocessedKeys;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString BatchGetItemOutput::getErrorType() const {
    return errorType;
}

MinimalString BatchGetItemOutput::getErrorMessage() const {
    return errorMessage;
}

void BatchGetItemOutput::setResponses(MinimalMap<MinimalList<MinimalMap<AttributeValue > > > responses) {
    responsesBeenSet = true;
    this->responses = responses;
}

void BatchGetItemOutput::setUnprocessedKeys(MinimalMap<KeysAndAttributes > unprocessedKeys) {
    unprocessedKeysBeenSet = true;
    this->unprocessedKeys = unprocessedKeys;
}

void BatchGetItemOutput::setConsumedCapacity(MinimalList<ConsumedCapacity > consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<MinimalList<MinimalMap<AttributeValue > > > BatchGetItemOutput::getResponses() const {
    return responses;
}

MinimalMap<KeysAndAttributes > BatchGetItemOutput::getUnprocessedKeys() const {
    return unprocessedKeys;
}

MinimalList<ConsumedCapacity > BatchGetItemOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

CreateTableInput::CreateTableInput() {
    reset();
}

void CreateTableInput::reset() {
    provisionedThroughputBeenSet = false;
    globalSecondaryIndexesBeenSet = false;
    tableNameBeenSet = false;
    localSecondaryIndexesBeenSet = false;
    keySchemaBeenSet = false;
    attributeDefinitionsBeenSet = false;
}

bool CreateTableInput::requiredAreSet() const {
    return attributeDefinitionsBeenSet && tableNameBeenSet && keySchemaBeenSet && provisionedThroughputBeenSet;
}

MinimalString CreateTableInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (provisionedThroughputBeenSet) {
        itemCount++;
    }
    if (globalSecondaryIndexesBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (localSecondaryIndexesBeenSet) {
        itemCount++;
    }
    if (keySchemaBeenSet) {
        itemCount++;
    }
    if (attributeDefinitionsBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (provisionedThroughputBeenSet) {
        MinimalString pairValue = provisionedThroughput.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROVISIONEDTHROUGHPUT_KEY, pairValue);
    }
    if (globalSecondaryIndexesBeenSet) {
        MinimalString pairValue = globalSecondaryIndexes.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(GLOBALSECONDARYINDEXES_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (localSecondaryIndexesBeenSet) {
        MinimalString pairValue = localSecondaryIndexes.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(LOCALSECONDARYINDEXES_KEY, pairValue);
    }
    if (keySchemaBeenSet) {
        MinimalString pairValue = keySchema.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEYSCHEMA_KEY, pairValue);
    }
    if (attributeDefinitionsBeenSet) {
        MinimalString pairValue = attributeDefinitions.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTEDEFINITIONS_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void CreateTableInput::setProvisionedThroughput(ProvisionedThroughput provisionedThroughput) {
    provisionedThroughputBeenSet = true;
    this->provisionedThroughput = provisionedThroughput;
}

void CreateTableInput::setGlobalSecondaryIndexes(MinimalList<GlobalSecondaryIndex > globalSecondaryIndexes) {
    globalSecondaryIndexesBeenSet = true;
    this->globalSecondaryIndexes = globalSecondaryIndexes;
}

void CreateTableInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void CreateTableInput::setLocalSecondaryIndexes(MinimalList<LocalSecondaryIndex > localSecondaryIndexes) {
    localSecondaryIndexesBeenSet = true;
    this->localSecondaryIndexes = localSecondaryIndexes;
}

void CreateTableInput::setKeySchema(MinimalList<KeySchemaElement > keySchema) {
    keySchemaBeenSet = true;
    this->keySchema = keySchema;
}

void CreateTableInput::setAttributeDefinitions(MinimalList<AttributeDefinition > attributeDefinitions) {
    attributeDefinitionsBeenSet = true;
    this->attributeDefinitions = attributeDefinitions;
}

ProvisionedThroughput CreateTableInput::getProvisionedThroughput() const {
    return provisionedThroughput;
}

MinimalList<GlobalSecondaryIndex > CreateTableInput::getGlobalSecondaryIndexes() const {
    return globalSecondaryIndexes;
}

MinimalString CreateTableInput::getTableName() const {
    return tableName;
}

MinimalList<LocalSecondaryIndex > CreateTableInput::getLocalSecondaryIndexes() const {
    return localSecondaryIndexes;
}

MinimalList<KeySchemaElement > CreateTableInput::getKeySchema() const {
    return keySchema;
}

MinimalList<AttributeDefinition > CreateTableInput::getAttributeDefinitions() const {
    return attributeDefinitions;
}

DescribeTableInput::DescribeTableInput() {
    reset();
}

void DescribeTableInput::reset() {
    tableNameBeenSet = false;
}

bool DescribeTableInput::requiredAreSet() const {
    return tableNameBeenSet;
}

MinimalString DescribeTableInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (tableNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void DescribeTableInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

MinimalString DescribeTableInput::getTableName() const {
    return tableName;
}

PutItemInput::PutItemInput() {
    reset();
}

void PutItemInput::reset() {
    returnItemCollectionMetricsBeenSet = false;
    returnValuesBeenSet = false;
    itemBeenSet = false;
    conditionalOperatorBeenSet = false;
    expectedBeenSet = false;
    tableNameBeenSet = false;
    returnConsumedCapacityBeenSet = false;
}

bool PutItemInput::requiredAreSet() const {
    return tableNameBeenSet && itemBeenSet;
}

MinimalString PutItemInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (returnItemCollectionMetricsBeenSet) {
        itemCount++;
    }
    if (returnValuesBeenSet) {
        itemCount++;
    }
    if (itemBeenSet) {
        itemCount++;
    }
    if (conditionalOperatorBeenSet) {
        itemCount++;
    }
    if (expectedBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (returnItemCollectionMetricsBeenSet) {
        MinimalString pairValue = MinimalString(RETURNITEMCOLLECTIONMETRICS_LOOKUP[returnItemCollectionMetrics]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNITEMCOLLECTIONMETRICS_KEY, pairValue);
    }
    if (returnValuesBeenSet) {
        MinimalString pairValue = MinimalString(RETURNVALUE_LOOKUP[returnValues]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNVALUES_KEY, pairValue);
    }
    if (itemBeenSet) {
        MinimalString pairValue = item.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ITEM_KEY, pairValue);
    }
    if (conditionalOperatorBeenSet) {
        MinimalString pairValue = MinimalString(CONDITIONALOPERATOR_LOOKUP[conditionalOperator]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONDITIONALOPERATOR_KEY, pairValue);
    }
    if (expectedBeenSet) {
        MinimalString pairValue = expected.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXPECTED_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void PutItemInput::setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics) {
    returnItemCollectionMetricsBeenSet = true;
    this->returnItemCollectionMetrics = returnItemCollectionMetrics;
}

void PutItemInput::setReturnValues(ReturnValue returnValues) {
    returnValuesBeenSet = true;
    this->returnValues = returnValues;
}

void PutItemInput::setItem(MinimalMap<AttributeValue > item) {
    itemBeenSet = true;
    this->item = item;
}

void PutItemInput::setConditionalOperator(ConditionalOperator conditionalOperator) {
    conditionalOperatorBeenSet = true;
    this->conditionalOperator = conditionalOperator;
}

void PutItemInput::setExpected(MinimalMap<ExpectedAttributeValue > expected) {
    expectedBeenSet = true;
    this->expected = expected;
}

void PutItemInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void PutItemInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

ReturnItemCollectionMetrics PutItemInput::getReturnItemCollectionMetrics() const {
    return returnItemCollectionMetrics;
}

ReturnValue PutItemInput::getReturnValues() const {
    return returnValues;
}

MinimalMap<AttributeValue > PutItemInput::getItem() const {
    return item;
}

ConditionalOperator PutItemInput::getConditionalOperator() const {
    return conditionalOperator;
}

MinimalMap<ExpectedAttributeValue > PutItemInput::getExpected() const {
    return expected;
}

MinimalString PutItemInput::getTableName() const {
    return tableName;
}

ReturnConsumedCapacity PutItemInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

GetItemOutput::GetItemOutput() {
    reset();
}

void GetItemOutput::reset() {
    itemBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool GetItemOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedItem = jsmnGetVal(ITEM_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedItem != NULL) {
        itemBeenSet = true;
        if (!item.jsonDeserialize(serializedItem)) {
            toReturn = false;
        }
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedItem != NULL) {
        delete[] serializedItem;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString GetItemOutput::getErrorType() const {
    return errorType;
}

MinimalString GetItemOutput::getErrorMessage() const {
    return errorMessage;
}

void GetItemOutput::setItem(MinimalMap<AttributeValue > item) {
    itemBeenSet = true;
    this->item = item;
}

void GetItemOutput::setConsumedCapacity(ConsumedCapacity consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<AttributeValue > GetItemOutput::getItem() const {
    return item;
}

ConsumedCapacity GetItemOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

DeleteTableInput::DeleteTableInput() {
    reset();
}

void DeleteTableInput::reset() {
    tableNameBeenSet = false;
}

bool DeleteTableInput::requiredAreSet() const {
    return tableNameBeenSet;
}

MinimalString DeleteTableInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (tableNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void DeleteTableInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

MinimalString DeleteTableInput::getTableName() const {
    return tableName;
}

DescribeTableOutput::DescribeTableOutput() {
    reset();
}

void DescribeTableOutput::reset() {
    tableBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool DescribeTableOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedTable = jsmnGetVal(TABLE_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedTable != NULL) {
        tableBeenSet = true;
        if (!table.jsonDeserialize(serializedTable)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedTable != NULL) {
        delete[] serializedTable;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString DescribeTableOutput::getErrorType() const {
    return errorType;
}

MinimalString DescribeTableOutput::getErrorMessage() const {
    return errorMessage;
}

void DescribeTableOutput::setTable(TableDescription table) {
    tableBeenSet = true;
    this->table = table;
}

TableDescription DescribeTableOutput::getTable() const {
    return table;
}

ScanOutput::ScanOutput() {
    reset();
}

void ScanOutput::reset() {
    lastEvaluatedKeyBeenSet = false;
    itemsBeenSet = false;
    countBeenSet = false;
    scannedCountBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool ScanOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedLastEvaluatedKey = jsmnGetVal(LASTEVALUATEDKEY_KEY, jsonCStr, tokens, tCount);
    char* serializedItems = jsmnGetVal(ITEMS_KEY, jsonCStr, tokens, tCount);
    char* serializedCount = jsmnGetVal(COUNT_KEY, jsonCStr, tokens, tCount);
    char* serializedScannedCount = jsmnGetVal(SCANNEDCOUNT_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedLastEvaluatedKey != NULL) {
        lastEvaluatedKeyBeenSet = true;
        if (!lastEvaluatedKey.jsonDeserialize(serializedLastEvaluatedKey)) {
            toReturn = false;
        }
    }
    if (serializedItems != NULL) {
        itemsBeenSet = true;
        if (!items.jsonDeserialize(serializedItems)) {
            toReturn = false;
        }
    }
    if (serializedCount != NULL) {
        countBeenSet = true;
        count = atoi(serializedCount);
    }
    if (serializedScannedCount != NULL) {
        scannedCountBeenSet = true;
        scannedCount = atoi(serializedScannedCount);
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedLastEvaluatedKey != NULL) {
        delete[] serializedLastEvaluatedKey;
    }
    if (serializedItems != NULL) {
        delete[] serializedItems;
    }
    if (serializedCount != NULL) {
        delete[] serializedCount;
    }
    if (serializedScannedCount != NULL) {
        delete[] serializedScannedCount;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString ScanOutput::getErrorType() const {
    return errorType;
}

MinimalString ScanOutput::getErrorMessage() const {
    return errorMessage;
}

void ScanOutput::setLastEvaluatedKey(MinimalMap<AttributeValue > lastEvaluatedKey) {
    lastEvaluatedKeyBeenSet = true;
    this->lastEvaluatedKey = lastEvaluatedKey;
}

void ScanOutput::setItems(MinimalList<MinimalMap<AttributeValue > > items) {
    itemsBeenSet = true;
    this->items = items;
}

void ScanOutput::setCount(int count) {
    countBeenSet = true;
    this->count = count;
}

void ScanOutput::setScannedCount(int scannedCount) {
    scannedCountBeenSet = true;
    this->scannedCount = scannedCount;
}

void ScanOutput::setConsumedCapacity(ConsumedCapacity consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<AttributeValue > ScanOutput::getLastEvaluatedKey() const {
    return lastEvaluatedKey;
}

MinimalList<MinimalMap<AttributeValue > > ScanOutput::getItems() const {
    return items;
}

int ScanOutput::getCount() const {
    return count;
}

int ScanOutput::getScannedCount() const {
    return scannedCount;
}

ConsumedCapacity ScanOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

BatchGetItemInput::BatchGetItemInput() {
    reset();
}

void BatchGetItemInput::reset() {
    requestItemsBeenSet = false;
    returnConsumedCapacityBeenSet = false;
}

bool BatchGetItemInput::requiredAreSet() const {
    return requestItemsBeenSet;
}

MinimalString BatchGetItemInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (requestItemsBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (requestItemsBeenSet) {
        MinimalString pairValue = requestItems.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(REQUESTITEMS_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void BatchGetItemInput::setRequestItems(MinimalMap<KeysAndAttributes > requestItems) {
    requestItemsBeenSet = true;
    this->requestItems = requestItems;
}

void BatchGetItemInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

MinimalMap<KeysAndAttributes > BatchGetItemInput::getRequestItems() const {
    return requestItems;
}

ReturnConsumedCapacity BatchGetItemInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

UpdateItemInput::UpdateItemInput() {
    reset();
}

void UpdateItemInput::reset() {
    returnItemCollectionMetricsBeenSet = false;
    returnValuesBeenSet = false;
    keyBeenSet = false;
    conditionalOperatorBeenSet = false;
    expectedBeenSet = false;
    tableNameBeenSet = false;
    attributeUpdatesBeenSet = false;
    returnConsumedCapacityBeenSet = false;
}

bool UpdateItemInput::requiredAreSet() const {
    return tableNameBeenSet && keyBeenSet;
}

MinimalString UpdateItemInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (returnItemCollectionMetricsBeenSet) {
        itemCount++;
    }
    if (returnValuesBeenSet) {
        itemCount++;
    }
    if (keyBeenSet) {
        itemCount++;
    }
    if (conditionalOperatorBeenSet) {
        itemCount++;
    }
    if (expectedBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (attributeUpdatesBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (returnItemCollectionMetricsBeenSet) {
        MinimalString pairValue = MinimalString(RETURNITEMCOLLECTIONMETRICS_LOOKUP[returnItemCollectionMetrics]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNITEMCOLLECTIONMETRICS_KEY, pairValue);
    }
    if (returnValuesBeenSet) {
        MinimalString pairValue = MinimalString(RETURNVALUE_LOOKUP[returnValues]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNVALUES_KEY, pairValue);
    }
    if (keyBeenSet) {
        MinimalString pairValue = key.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEY_KEY, pairValue);
    }
    if (conditionalOperatorBeenSet) {
        MinimalString pairValue = MinimalString(CONDITIONALOPERATOR_LOOKUP[conditionalOperator]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONDITIONALOPERATOR_KEY, pairValue);
    }
    if (expectedBeenSet) {
        MinimalString pairValue = expected.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXPECTED_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (attributeUpdatesBeenSet) {
        MinimalString pairValue = attributeUpdates.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTEUPDATES_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void UpdateItemInput::setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics) {
    returnItemCollectionMetricsBeenSet = true;
    this->returnItemCollectionMetrics = returnItemCollectionMetrics;
}

void UpdateItemInput::setReturnValues(ReturnValue returnValues) {
    returnValuesBeenSet = true;
    this->returnValues = returnValues;
}

void UpdateItemInput::setKey(MinimalMap<AttributeValue > key) {
    keyBeenSet = true;
    this->key = key;
}

void UpdateItemInput::setConditionalOperator(ConditionalOperator conditionalOperator) {
    conditionalOperatorBeenSet = true;
    this->conditionalOperator = conditionalOperator;
}

void UpdateItemInput::setExpected(MinimalMap<ExpectedAttributeValue > expected) {
    expectedBeenSet = true;
    this->expected = expected;
}

void UpdateItemInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void UpdateItemInput::setAttributeUpdates(MinimalMap<AttributeValueUpdate > attributeUpdates) {
    attributeUpdatesBeenSet = true;
    this->attributeUpdates = attributeUpdates;
}

void UpdateItemInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

ReturnItemCollectionMetrics UpdateItemInput::getReturnItemCollectionMetrics() const {
    return returnItemCollectionMetrics;
}

ReturnValue UpdateItemInput::getReturnValues() const {
    return returnValues;
}

MinimalMap<AttributeValue > UpdateItemInput::getKey() const {
    return key;
}

ConditionalOperator UpdateItemInput::getConditionalOperator() const {
    return conditionalOperator;
}

MinimalMap<ExpectedAttributeValue > UpdateItemInput::getExpected() const {
    return expected;
}

MinimalString UpdateItemInput::getTableName() const {
    return tableName;
}

MinimalMap<AttributeValueUpdate > UpdateItemInput::getAttributeUpdates() const {
    return attributeUpdates;
}

ReturnConsumedCapacity UpdateItemInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

CreateTableOutput::CreateTableOutput() {
    reset();
}

void CreateTableOutput::reset() {
    tableDescriptionBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool CreateTableOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedTableDescription = jsmnGetVal(TABLEDESCRIPTION_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedTableDescription != NULL) {
        tableDescriptionBeenSet = true;
        if (!tableDescription.jsonDeserialize(serializedTableDescription)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedTableDescription != NULL) {
        delete[] serializedTableDescription;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString CreateTableOutput::getErrorType() const {
    return errorType;
}

MinimalString CreateTableOutput::getErrorMessage() const {
    return errorMessage;
}

void CreateTableOutput::setTableDescription(TableDescription tableDescription) {
    tableDescriptionBeenSet = true;
    this->tableDescription = tableDescription;
}

TableDescription CreateTableOutput::getTableDescription() const {
    return tableDescription;
}

UpdateTableInput::UpdateTableInput() {
    reset();
}

void UpdateTableInput::reset() {
    provisionedThroughputBeenSet = false;
    globalSecondaryIndexUpdatesBeenSet = false;
    tableNameBeenSet = false;
}

bool UpdateTableInput::requiredAreSet() const {
    return tableNameBeenSet;
}

MinimalString UpdateTableInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (provisionedThroughputBeenSet) {
        itemCount++;
    }
    if (globalSecondaryIndexUpdatesBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (provisionedThroughputBeenSet) {
        MinimalString pairValue = provisionedThroughput.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PROVISIONEDTHROUGHPUT_KEY, pairValue);
    }
    if (globalSecondaryIndexUpdatesBeenSet) {
        MinimalString pairValue = globalSecondaryIndexUpdates.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(GLOBALSECONDARYINDEXUPDATES_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void UpdateTableInput::setProvisionedThroughput(ProvisionedThroughput provisionedThroughput) {
    provisionedThroughputBeenSet = true;
    this->provisionedThroughput = provisionedThroughput;
}

void UpdateTableInput::setGlobalSecondaryIndexUpdates(MinimalList<GlobalSecondaryIndexUpdate > globalSecondaryIndexUpdates) {
    globalSecondaryIndexUpdatesBeenSet = true;
    this->globalSecondaryIndexUpdates = globalSecondaryIndexUpdates;
}

void UpdateTableInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

ProvisionedThroughput UpdateTableInput::getProvisionedThroughput() const {
    return provisionedThroughput;
}

MinimalList<GlobalSecondaryIndexUpdate > UpdateTableInput::getGlobalSecondaryIndexUpdates() const {
    return globalSecondaryIndexUpdates;
}

MinimalString UpdateTableInput::getTableName() const {
    return tableName;
}

GetItemInput::GetItemInput() {
    reset();
}

void GetItemInput::reset() {
    keyBeenSet = false;
    consistentReadBeenSet = false;
    attributesToGetBeenSet = false;
    tableNameBeenSet = false;
    returnConsumedCapacityBeenSet = false;
}

bool GetItemInput::requiredAreSet() const {
    return tableNameBeenSet && keyBeenSet;
}

MinimalString GetItemInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (keyBeenSet) {
        itemCount++;
    }
    if (consistentReadBeenSet) {
        itemCount++;
    }
    if (attributesToGetBeenSet) {
        itemCount++;
    }
    if (tableNameBeenSet) {
        itemCount++;
    }
    if (returnConsumedCapacityBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (keyBeenSet) {
        MinimalString pairValue = key.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(KEY_KEY, pairValue);
    }
    if (consistentReadBeenSet) {
        MinimalString pairValue = jsonSerializeBool(consistentRead);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(CONSISTENTREAD_KEY, pairValue);
    }
    if (attributesToGetBeenSet) {
        MinimalString pairValue = attributesToGet.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ATTRIBUTESTOGET_KEY, pairValue);
    }
    if (tableNameBeenSet) {
        MinimalString pairValue = tableName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(TABLENAME_KEY, pairValue);
    }
    if (returnConsumedCapacityBeenSet) {
        MinimalString pairValue = MinimalString(RETURNCONSUMEDCAPACITY_LOOKUP[returnConsumedCapacity]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(RETURNCONSUMEDCAPACITY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void GetItemInput::setKey(MinimalMap<AttributeValue > key) {
    keyBeenSet = true;
    this->key = key;
}

void GetItemInput::setConsistentRead(bool consistentRead) {
    consistentReadBeenSet = true;
    this->consistentRead = consistentRead;
}

void GetItemInput::setAttributesToGet(MinimalList<MinimalString > attributesToGet) {
    attributesToGetBeenSet = true;
    this->attributesToGet = attributesToGet;
}

void GetItemInput::setTableName(MinimalString tableName) {
    tableNameBeenSet = true;
    this->tableName = tableName;
}

void GetItemInput::setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity) {
    returnConsumedCapacityBeenSet = true;
    this->returnConsumedCapacity = returnConsumedCapacity;
}

MinimalMap<AttributeValue > GetItemInput::getKey() const {
    return key;
}

bool GetItemInput::getConsistentRead() const {
    return consistentRead;
}

MinimalList<MinimalString > GetItemInput::getAttributesToGet() const {
    return attributesToGet;
}

MinimalString GetItemInput::getTableName() const {
    return tableName;
}

ReturnConsumedCapacity GetItemInput::getReturnConsumedCapacity() const {
    return returnConsumedCapacity;
}

DeleteTableOutput::DeleteTableOutput() {
    reset();
}

void DeleteTableOutput::reset() {
    tableDescriptionBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool DeleteTableOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedTableDescription = jsmnGetVal(TABLEDESCRIPTION_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedTableDescription != NULL) {
        tableDescriptionBeenSet = true;
        if (!tableDescription.jsonDeserialize(serializedTableDescription)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedTableDescription != NULL) {
        delete[] serializedTableDescription;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString DeleteTableOutput::getErrorType() const {
    return errorType;
}

MinimalString DeleteTableOutput::getErrorMessage() const {
    return errorMessage;
}

void DeleteTableOutput::setTableDescription(TableDescription tableDescription) {
    tableDescriptionBeenSet = true;
    this->tableDescription = tableDescription;
}

TableDescription DeleteTableOutput::getTableDescription() const {
    return tableDescription;
}

PutItemOutput::PutItemOutput() {
    reset();
}

void PutItemOutput::reset() {
    attributesBeenSet = false;
    itemCollectionMetricsBeenSet = false;
    consumedCapacityBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool PutItemOutput::jsonDeserialize(MinimalString json) {
    reset();
    const char* jsonCStr = json.getCStr();
    int jsonLen = json.length();
    jsmn_parser p;
    jsmn_init(&p);
    int tCount = jsmn_parse(&p, jsonCStr, jsonLen, 0, 0);
    if (tCount < 0) {
        return false;
    }
    jsmntok_t* tokens = new jsmntok_t[tCount]();
    jsmn_init(&p);
    jsmn_parse(&p, jsonCStr, jsonLen, tokens, tCount);

    char* serializedAttributes = jsmnGetVal(ATTRIBUTES_KEY, jsonCStr, tokens, tCount);
    char* serializedItemCollectionMetrics = jsmnGetVal(ITEMCOLLECTIONMETRICS_KEY, jsonCStr, tokens, tCount);
    char* serializedConsumedCapacity = jsmnGetVal(CONSUMEDCAPACITY_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(DYNAMODB_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(DYNAMODB_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedAttributes != NULL) {
        attributesBeenSet = true;
        if (!attributes.jsonDeserialize(serializedAttributes)) {
            toReturn = false;
        }
    }
    if (serializedItemCollectionMetrics != NULL) {
        itemCollectionMetricsBeenSet = true;
        if (!itemCollectionMetrics.jsonDeserialize(serializedItemCollectionMetrics)) {
            toReturn = false;
        }
    }
    if (serializedConsumedCapacity != NULL) {
        consumedCapacityBeenSet = true;
        if (!consumedCapacity.jsonDeserialize(serializedConsumedCapacity)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedAttributes != NULL) {
        delete[] serializedAttributes;
    }
    if (serializedItemCollectionMetrics != NULL) {
        delete[] serializedItemCollectionMetrics;
    }
    if (serializedConsumedCapacity != NULL) {
        delete[] serializedConsumedCapacity;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    if (errorMessageCStr != NULL) {
        delete[] errorMessageCStr;
    }
    if (errorTypeCStr != NULL) {
        delete[] errorTypeCStr;
    }
    return toReturn;
}

MinimalString PutItemOutput::getErrorType() const {
    return errorType;
}

MinimalString PutItemOutput::getErrorMessage() const {
    return errorMessage;
}

void PutItemOutput::setAttributes(MinimalMap<AttributeValue > attributes) {
    attributesBeenSet = true;
    this->attributes = attributes;
}

void PutItemOutput::setItemCollectionMetrics(ItemCollectionMetrics itemCollectionMetrics) {
    itemCollectionMetricsBeenSet = true;
    this->itemCollectionMetrics = itemCollectionMetrics;
}

void PutItemOutput::setConsumedCapacity(ConsumedCapacity consumedCapacity) {
    consumedCapacityBeenSet = true;
    this->consumedCapacity = consumedCapacity;
}

MinimalMap<AttributeValue > PutItemOutput::getAttributes() const {
    return attributes;
}

ItemCollectionMetrics PutItemOutput::getItemCollectionMetrics() const {
    return itemCollectionMetrics;
}

ConsumedCapacity PutItemOutput::getConsumedCapacity() const {
    return consumedCapacity;
}

AmazonDynamoDBClient::AmazonDynamoDBClient() : AWSClient() {
    awsService = SERVICE;
}

BatchGetItemOutput AmazonDynamoDBClient::batchGetItem(BatchGetItemInput batchGetItemInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    BatchGetItemOutput batchGetItemOutput;

    if (!batchGetItemInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return batchGetItemOutput;
    }
    contentType = JSON_TYPE;
    target = BATCHGETITEM_TARGET;
    httpS = false;
    MinimalString payload = batchGetItemInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return batchGetItemOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return batchGetItemOutput;
    }
    if (!batchGetItemOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return batchGetItemOutput;
    }
    delete[] response;
    MinimalString errorType = batchGetItemOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(batchGetItemOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return batchGetItem(batchGetItemInput, actionError, false, httpStatusCode);
        }
    }
    return batchGetItemOutput;
}

BatchWriteItemOutput AmazonDynamoDBClient::batchWriteItem(BatchWriteItemInput batchWriteItemInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    BatchWriteItemOutput batchWriteItemOutput;

    if (!batchWriteItemInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return batchWriteItemOutput;
    }
    contentType = JSON_TYPE;
    target = BATCHWRITEITEM_TARGET;
    httpS = false;
    MinimalString payload = batchWriteItemInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return batchWriteItemOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return batchWriteItemOutput;
    }
    if (!batchWriteItemOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return batchWriteItemOutput;
    }
    delete[] response;
    MinimalString errorType = batchWriteItemOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(batchWriteItemOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return batchWriteItem(batchWriteItemInput, actionError, false, httpStatusCode);
        }
    }
    return batchWriteItemOutput;
}

CreateTableOutput AmazonDynamoDBClient::createTable(CreateTableInput createTableInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    CreateTableOutput createTableOutput;

    if (!createTableInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return createTableOutput;
    }
    contentType = JSON_TYPE;
    target = CREATETABLE_TARGET;
    httpS = false;
    MinimalString payload = createTableInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return createTableOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return createTableOutput;
    }
    if (!createTableOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return createTableOutput;
    }
    delete[] response;
    MinimalString errorType = createTableOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(createTableOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return createTable(createTableInput, actionError, false, httpStatusCode);
        }
    }
    return createTableOutput;
}

DeleteItemOutput AmazonDynamoDBClient::deleteItem(DeleteItemInput deleteItemInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    DeleteItemOutput deleteItemOutput;

    if (!deleteItemInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return deleteItemOutput;
    }
    contentType = JSON_TYPE;
    target = DELETEITEM_TARGET;
    httpS = false;
    MinimalString payload = deleteItemInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return deleteItemOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return deleteItemOutput;
    }
    if (!deleteItemOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return deleteItemOutput;
    }
    delete[] response;
    MinimalString errorType = deleteItemOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(deleteItemOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return deleteItem(deleteItemInput, actionError, false, httpStatusCode);
        }
    }
    return deleteItemOutput;
}

DeleteTableOutput AmazonDynamoDBClient::deleteTable(DeleteTableInput deleteTableInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    DeleteTableOutput deleteTableOutput;

    if (!deleteTableInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return deleteTableOutput;
    }
    contentType = JSON_TYPE;
    target = DELETETABLE_TARGET;
    httpS = false;
    MinimalString payload = deleteTableInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return deleteTableOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return deleteTableOutput;
    }
    if (!deleteTableOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return deleteTableOutput;
    }
    delete[] response;
    MinimalString errorType = deleteTableOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(deleteTableOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return deleteTable(deleteTableInput, actionError, false, httpStatusCode);
        }
    }
    return deleteTableOutput;
}

DescribeTableOutput AmazonDynamoDBClient::describeTable(DescribeTableInput describeTableInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    DescribeTableOutput describeTableOutput;

    if (!describeTableInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return describeTableOutput;
    }
    contentType = JSON_TYPE;
    target = DESCRIBETABLE_TARGET;
    httpS = false;
    MinimalString payload = describeTableInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return describeTableOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return describeTableOutput;
    }
    if (!describeTableOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return describeTableOutput;
    }
    delete[] response;
    MinimalString errorType = describeTableOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(describeTableOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return describeTable(describeTableInput, actionError, false, httpStatusCode);
        }
    }
    return describeTableOutput;
}

GetItemOutput AmazonDynamoDBClient::getItem(GetItemInput getItemInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    GetItemOutput getItemOutput;

    if (!getItemInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return getItemOutput;
    }
    contentType = JSON_TYPE;
    target = GETITEM_TARGET;
    httpS = false;
    MinimalString payload = getItemInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return getItemOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return getItemOutput;
    }
    if (!getItemOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return getItemOutput;
    }
    delete[] response;
    MinimalString errorType = getItemOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(getItemOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return getItem(getItemInput, actionError, false, httpStatusCode);
        }
    }
    return getItemOutput;
}

ListTablesOutput AmazonDynamoDBClient::listTables(ListTablesInput listTablesInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    ListTablesOutput listTablesOutput;

    if (!listTablesInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return listTablesOutput;
    }
    contentType = JSON_TYPE;
    target = LISTTABLES_TARGET;
    httpS = false;
    MinimalString payload = listTablesInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return listTablesOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return listTablesOutput;
    }
    if (!listTablesOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return listTablesOutput;
    }
    delete[] response;
    MinimalString errorType = listTablesOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(listTablesOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return listTables(listTablesInput, actionError, false, httpStatusCode);
        }
    }
    return listTablesOutput;
}

PutItemOutput AmazonDynamoDBClient::putItem(PutItemInput putItemInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    PutItemOutput putItemOutput;

    if (!putItemInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return putItemOutput;
    }
    contentType = JSON_TYPE;
    target = PUTITEM_TARGET;
    httpS = false;
    MinimalString payload = putItemInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return putItemOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return putItemOutput;
    }
    if (!putItemOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return putItemOutput;
    }
    delete[] response;
    MinimalString errorType = putItemOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(putItemOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return putItem(putItemInput, actionError, false, httpStatusCode);
        }
    }
    return putItemOutput;
}

QueryOutput AmazonDynamoDBClient::query(QueryInput queryInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    QueryOutput queryOutput;

    if (!queryInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return queryOutput;
    }
    contentType = JSON_TYPE;
    target = QUERY_TARGET;
    httpS = false;
    MinimalString payload = queryInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return queryOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return queryOutput;
    }
    if (!queryOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return queryOutput;
    }
    delete[] response;
    MinimalString errorType = queryOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(queryOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return query(queryInput, actionError, false, httpStatusCode);
        }
    }
    return queryOutput;
}

ScanOutput AmazonDynamoDBClient::scan(ScanInput scanInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    ScanOutput scanOutput;

    if (!scanInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return scanOutput;
    }
    contentType = JSON_TYPE;
    target = SCAN_TARGET;
    httpS = false;
    MinimalString payload = scanInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return scanOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return scanOutput;
    }
    if (!scanOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return scanOutput;
    }
    delete[] response;
    MinimalString errorType = scanOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(scanOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return scan(scanInput, actionError, false, httpStatusCode);
        }
    }
    return scanOutput;
}

UpdateItemOutput AmazonDynamoDBClient::updateItem(UpdateItemInput updateItemInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    UpdateItemOutput updateItemOutput;

    if (!updateItemInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return updateItemOutput;
    }
    contentType = JSON_TYPE;
    target = UPDATEITEM_TARGET;
    httpS = false;
    MinimalString payload = updateItemInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return updateItemOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return updateItemOutput;
    }
    if (!updateItemOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return updateItemOutput;
    }
    delete[] response;
    MinimalString errorType = updateItemOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(updateItemOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return updateItem(updateItemInput, actionError, false, httpStatusCode);
        }
    }
    return updateItemOutput;
}

UpdateTableOutput AmazonDynamoDBClient::updateTable(UpdateTableInput updateTableInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    UpdateTableOutput updateTableOutput;

    if (!updateTableInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return updateTableOutput;
    }
    contentType = JSON_TYPE;
    target = UPDATETABLE_TARGET;
    httpS = false;
    MinimalString payload = updateTableInput.jsonSerialize();
    char* request;
    if (httpClient->usesCurl()) {
        request = createCurlRequest(payload);
    } else {
        request = createRequest(payload);
    }
    char* response = sendData(request);
    delete[] request;
    if (response == NULL) {
        actionError = CONNECTION_ACTIONERROR;
        return updateTableOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return updateTableOutput;
    }
    if (!updateTableOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return updateTableOutput;
    }
    delete[] response;
    MinimalString errorType = updateTableOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), DYNAMODB_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(updateTableOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return updateTable(updateTableInput, actionError, false, httpStatusCode);
        }
    }
    return updateTableOutput;
}
