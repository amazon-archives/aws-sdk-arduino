#include "AmazonKinesisClient.h"
#include "AWSFoundationalTypes.h"
#include "jsmn.h"
#include "Utils.h"
#include <stdlib.h>

static const char* SERVICE = "kinesis";
static const char* JSON_TYPE = "application/x-amz-json-1.1";
static const char* CREATESTREAM_TARGET = "Kinesis_20131202.CreateStream";
static const char* DELETESTREAM_TARGET = "Kinesis_20131202.DeleteStream";
static const char* DESCRIBESTREAM_TARGET = "Kinesis_20131202.DescribeStream";
static const char* GETRECORDS_TARGET = "Kinesis_20131202.GetRecords";
static const char* GETSHARDITERATOR_TARGET = "Kinesis_20131202.GetShardIterator";
static const char* LISTSTREAMS_TARGET = "Kinesis_20131202.ListStreams";
static const char* MERGESHARDS_TARGET = "Kinesis_20131202.MergeShards";
static const char* PUTRECORD_TARGET = "Kinesis_20131202.PutRecord";
static const char* SPLITSHARD_TARGET = "Kinesis_20131202.SplitShard";
static const char* KINESIS_ERROR_TYPE_KEY = "__type";
static const char* KINESIS_ERROR_MESSAGE_KEY = "message";
static const char* KINESIS_SIGNATURE_ERROR_TYPE = "InvalidSignatureException";

static const char* SHARDID_KEY = "ShardId";
static const char* SEQUENCENUMBER_KEY = "SequenceNumber";
static const char* STARTINGSEQUENCENUMBER_KEY = "StartingSequenceNumber";
static const char* ENDINGSEQUENCENUMBER_KEY = "EndingSequenceNumber";
static const char* PARENTSHARDID_KEY = "ParentShardId";
static const char* SEQUENCENUMBERRANGE_KEY = "SequenceNumberRange";
static const char* HASHKEYRANGE_KEY = "HashKeyRange";
static const char* ADJACENTPARENTSHARDID_KEY = "AdjacentParentShardId";
static const char* STREAMDESCRIPTION_KEY = "StreamDescription";
static const char* HASMORESHARDS_KEY = "HasMoreShards";
static const char* STREAMNAME_KEY = "StreamName";
static const char* STREAMARN_KEY = "StreamARN";
static const char* SHARDS_KEY = "Shards";
static const char* STREAMSTATUS_KEY = "StreamStatus";
static const char* DATA_KEY = "Data";
static const char* EXPLICITHASHKEY_KEY = "ExplicitHashKey";
static const char* SEQUENCENUMBERFORORDERING_KEY = "SequenceNumberForOrdering";
static const char* PARTITIONKEY_KEY = "PartitionKey";
static const char* EXCLUSIVESTARTSTREAMNAME_KEY = "ExclusiveStartStreamName";
static const char* LIMIT_KEY = "Limit";
static const char* NEXTSHARDITERATOR_KEY = "NextShardIterator";
static const char* RECORDS_KEY = "Records";
static const char* STREAMNAMES_KEY = "StreamNames";
static const char* HASMORESTREAMS_KEY = "HasMoreStreams";
static const char* STARTINGHASHKEY_KEY = "StartingHashKey";
static const char* ENDINGHASHKEY_KEY = "EndingHashKey";
static const char* NEWSTARTINGHASHKEY_KEY = "NewStartingHashKey";
static const char* SHARDTOSPLIT_KEY = "ShardToSplit";
static const char* SHARDCOUNT_KEY = "ShardCount";
static const char* SHARDITERATOR_KEY = "ShardIterator";
static const char* EXCLUSIVESTARTSHARDID_KEY = "ExclusiveStartShardId";
static const char* ADJACENTSHARDTOMERGE_KEY = "AdjacentShardToMerge";
static const char* SHARDTOMERGE_KEY = "ShardToMerge";
static const char* SHARDITERATORTYPE_KEY = "ShardIteratorType";

const char* SHARDITERATORTYPE_LOOKUP[] = {
    "\"AT_SEQUENCE_NUMBER\"",
    "\"AFTER_SEQUENCE_NUMBER\"",
    "\"TRIM_HORIZON\"",
    "\"LATEST\""
};

const char* STREAMSTATUS_LOOKUP[] = {
    "\"CREATING\"",
    "\"DELETING\"",
    "\"ACTIVE\"",
    "\"UPDATING\""
};

HashKeyRange::HashKeyRange() {
    reset();
}

void HashKeyRange::reset() {
    startingHashKeyBeenSet = false;
    endingHashKeyBeenSet = false;
}

bool HashKeyRange::jsonDeserialize(MinimalString json) {
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

    char* serializedStartingHashKey = jsmnGetVal(STARTINGHASHKEY_KEY, jsonCStr, tokens, tCount);
    char* serializedEndingHashKey = jsmnGetVal(ENDINGHASHKEY_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedStartingHashKey != NULL) {
        startingHashKeyBeenSet = true;
        startingHashKey = MinimalString(serializedStartingHashKey);
    }
    if (serializedEndingHashKey != NULL) {
        endingHashKeyBeenSet = true;
        endingHashKey = MinimalString(serializedEndingHashKey);
    }
    if (serializedStartingHashKey != NULL) {
        delete[] serializedStartingHashKey;
    }
    if (serializedEndingHashKey != NULL) {
        delete[] serializedEndingHashKey;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString HashKeyRange::jsonSerialize() const {
    int itemCount = 0;
    if (startingHashKeyBeenSet) {
        itemCount++;
    }
    if (endingHashKeyBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (startingHashKeyBeenSet) {
        MinimalString pairValue = startingHashKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STARTINGHASHKEY_KEY, pairValue);
    }
    if (endingHashKeyBeenSet) {
        MinimalString pairValue = endingHashKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ENDINGHASHKEY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void HashKeyRange::setStartingHashKey(MinimalString startingHashKey) {
    startingHashKeyBeenSet = true;
    this->startingHashKey = startingHashKey;
}

void HashKeyRange::setEndingHashKey(MinimalString endingHashKey) {
    endingHashKeyBeenSet = true;
    this->endingHashKey = endingHashKey;
}

MinimalString HashKeyRange::getStartingHashKey() const {
    return startingHashKey;
}

MinimalString HashKeyRange::getEndingHashKey() const {
    return endingHashKey;
}

SequenceNumberRange::SequenceNumberRange() {
    reset();
}

void SequenceNumberRange::reset() {
    startingSequenceNumberBeenSet = false;
    endingSequenceNumberBeenSet = false;
}

bool SequenceNumberRange::jsonDeserialize(MinimalString json) {
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

    char* serializedStartingSequenceNumber = jsmnGetVal(STARTINGSEQUENCENUMBER_KEY, jsonCStr, tokens, tCount);
    char* serializedEndingSequenceNumber = jsmnGetVal(ENDINGSEQUENCENUMBER_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedStartingSequenceNumber != NULL) {
        startingSequenceNumberBeenSet = true;
        startingSequenceNumber = MinimalString(serializedStartingSequenceNumber);
    }
    if (serializedEndingSequenceNumber != NULL) {
        endingSequenceNumberBeenSet = true;
        endingSequenceNumber = MinimalString(serializedEndingSequenceNumber);
    }
    if (serializedStartingSequenceNumber != NULL) {
        delete[] serializedStartingSequenceNumber;
    }
    if (serializedEndingSequenceNumber != NULL) {
        delete[] serializedEndingSequenceNumber;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString SequenceNumberRange::jsonSerialize() const {
    int itemCount = 0;
    if (startingSequenceNumberBeenSet) {
        itemCount++;
    }
    if (endingSequenceNumberBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (startingSequenceNumberBeenSet) {
        MinimalString pairValue = startingSequenceNumber.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STARTINGSEQUENCENUMBER_KEY, pairValue);
    }
    if (endingSequenceNumberBeenSet) {
        MinimalString pairValue = endingSequenceNumber.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ENDINGSEQUENCENUMBER_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void SequenceNumberRange::setStartingSequenceNumber(MinimalString startingSequenceNumber) {
    startingSequenceNumberBeenSet = true;
    this->startingSequenceNumber = startingSequenceNumber;
}

void SequenceNumberRange::setEndingSequenceNumber(MinimalString endingSequenceNumber) {
    endingSequenceNumberBeenSet = true;
    this->endingSequenceNumber = endingSequenceNumber;
}

MinimalString SequenceNumberRange::getStartingSequenceNumber() const {
    return startingSequenceNumber;
}

MinimalString SequenceNumberRange::getEndingSequenceNumber() const {
    return endingSequenceNumber;
}

Shard::Shard() {
    reset();
}

void Shard::reset() {
    parentShardIdBeenSet = false;
    sequenceNumberRangeBeenSet = false;
    shardIdBeenSet = false;
    hashKeyRangeBeenSet = false;
    adjacentParentShardIdBeenSet = false;
}

bool Shard::jsonDeserialize(MinimalString json) {
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

    char* serializedParentShardId = jsmnGetVal(PARENTSHARDID_KEY, jsonCStr, tokens, tCount);
    char* serializedSequenceNumberRange = jsmnGetVal(SEQUENCENUMBERRANGE_KEY, jsonCStr, tokens, tCount);
    char* serializedShardId = jsmnGetVal(SHARDID_KEY, jsonCStr, tokens, tCount);
    char* serializedHashKeyRange = jsmnGetVal(HASHKEYRANGE_KEY, jsonCStr, tokens, tCount);
    char* serializedAdjacentParentShardId = jsmnGetVal(ADJACENTPARENTSHARDID_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedParentShardId != NULL) {
        parentShardIdBeenSet = true;
        parentShardId = MinimalString(serializedParentShardId);
    }
    if (serializedSequenceNumberRange != NULL) {
        sequenceNumberRangeBeenSet = true;
        if (!sequenceNumberRange.jsonDeserialize(serializedSequenceNumberRange)) {
            toReturn = false;
        }
    }
    if (serializedShardId != NULL) {
        shardIdBeenSet = true;
        shardId = MinimalString(serializedShardId);
    }
    if (serializedHashKeyRange != NULL) {
        hashKeyRangeBeenSet = true;
        if (!hashKeyRange.jsonDeserialize(serializedHashKeyRange)) {
            toReturn = false;
        }
    }
    if (serializedAdjacentParentShardId != NULL) {
        adjacentParentShardIdBeenSet = true;
        adjacentParentShardId = MinimalString(serializedAdjacentParentShardId);
    }
    if (serializedParentShardId != NULL) {
        delete[] serializedParentShardId;
    }
    if (serializedSequenceNumberRange != NULL) {
        delete[] serializedSequenceNumberRange;
    }
    if (serializedShardId != NULL) {
        delete[] serializedShardId;
    }
    if (serializedHashKeyRange != NULL) {
        delete[] serializedHashKeyRange;
    }
    if (serializedAdjacentParentShardId != NULL) {
        delete[] serializedAdjacentParentShardId;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString Shard::jsonSerialize() const {
    int itemCount = 0;
    if (parentShardIdBeenSet) {
        itemCount++;
    }
    if (sequenceNumberRangeBeenSet) {
        itemCount++;
    }
    if (shardIdBeenSet) {
        itemCount++;
    }
    if (hashKeyRangeBeenSet) {
        itemCount++;
    }
    if (adjacentParentShardIdBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (parentShardIdBeenSet) {
        MinimalString pairValue = parentShardId.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PARENTSHARDID_KEY, pairValue);
    }
    if (sequenceNumberRangeBeenSet) {
        MinimalString pairValue = sequenceNumberRange.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SEQUENCENUMBERRANGE_KEY, pairValue);
    }
    if (shardIdBeenSet) {
        MinimalString pairValue = shardId.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDID_KEY, pairValue);
    }
    if (hashKeyRangeBeenSet) {
        MinimalString pairValue = hashKeyRange.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(HASHKEYRANGE_KEY, pairValue);
    }
    if (adjacentParentShardIdBeenSet) {
        MinimalString pairValue = adjacentParentShardId.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ADJACENTPARENTSHARDID_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void Shard::setParentShardId(MinimalString parentShardId) {
    parentShardIdBeenSet = true;
    this->parentShardId = parentShardId;
}

void Shard::setSequenceNumberRange(SequenceNumberRange sequenceNumberRange) {
    sequenceNumberRangeBeenSet = true;
    this->sequenceNumberRange = sequenceNumberRange;
}

void Shard::setShardId(MinimalString shardId) {
    shardIdBeenSet = true;
    this->shardId = shardId;
}

void Shard::setHashKeyRange(HashKeyRange hashKeyRange) {
    hashKeyRangeBeenSet = true;
    this->hashKeyRange = hashKeyRange;
}

void Shard::setAdjacentParentShardId(MinimalString adjacentParentShardId) {
    adjacentParentShardIdBeenSet = true;
    this->adjacentParentShardId = adjacentParentShardId;
}

MinimalString Shard::getParentShardId() const {
    return parentShardId;
}

SequenceNumberRange Shard::getSequenceNumberRange() const {
    return sequenceNumberRange;
}

MinimalString Shard::getShardId() const {
    return shardId;
}

HashKeyRange Shard::getHashKeyRange() const {
    return hashKeyRange;
}

MinimalString Shard::getAdjacentParentShardId() const {
    return adjacentParentShardId;
}

Record::Record() {
    reset();
}

void Record::reset() {
    dataBeenSet = false;
    partitionKeyBeenSet = false;
    sequenceNumberBeenSet = false;
}

bool Record::jsonDeserialize(MinimalString json) {
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

    char* serializedData = jsmnGetVal(DATA_KEY, jsonCStr, tokens, tCount);
    char* serializedPartitionKey = jsmnGetVal(PARTITIONKEY_KEY, jsonCStr, tokens, tCount);
    char* serializedSequenceNumber = jsmnGetVal(SEQUENCENUMBER_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedData != NULL) {
        dataBeenSet = true;
        data = MinimalString(serializedData);
    }
    if (serializedPartitionKey != NULL) {
        partitionKeyBeenSet = true;
        partitionKey = MinimalString(serializedPartitionKey);
    }
    if (serializedSequenceNumber != NULL) {
        sequenceNumberBeenSet = true;
        sequenceNumber = MinimalString(serializedSequenceNumber);
    }
    if (serializedData != NULL) {
        delete[] serializedData;
    }
    if (serializedPartitionKey != NULL) {
        delete[] serializedPartitionKey;
    }
    if (serializedSequenceNumber != NULL) {
        delete[] serializedSequenceNumber;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString Record::jsonSerialize() const {
    int itemCount = 0;
    if (dataBeenSet) {
        itemCount++;
    }
    if (partitionKeyBeenSet) {
        itemCount++;
    }
    if (sequenceNumberBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (dataBeenSet) {
        MinimalString pairValue = data.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(DATA_KEY, pairValue);
    }
    if (partitionKeyBeenSet) {
        MinimalString pairValue = partitionKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PARTITIONKEY_KEY, pairValue);
    }
    if (sequenceNumberBeenSet) {
        MinimalString pairValue = sequenceNumber.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SEQUENCENUMBER_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void Record::setData(MinimalString data) {
    dataBeenSet = true;
    this->data = data;
}

void Record::setPartitionKey(MinimalString partitionKey) {
    partitionKeyBeenSet = true;
    this->partitionKey = partitionKey;
}

void Record::setSequenceNumber(MinimalString sequenceNumber) {
    sequenceNumberBeenSet = true;
    this->sequenceNumber = sequenceNumber;
}

MinimalString Record::getData() const {
    return data;
}

MinimalString Record::getPartitionKey() const {
    return partitionKey;
}

MinimalString Record::getSequenceNumber() const {
    return sequenceNumber;
}

StreamDescription::StreamDescription() {
    reset();
}

void StreamDescription::reset() {
    hasMoreShardsBeenSet = false;
    streamNameBeenSet = false;
    streamARNBeenSet = false;
    shardsBeenSet = false;
    streamStatusBeenSet = false;
}

bool StreamDescription::jsonDeserialize(MinimalString json) {
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

    char* serializedHasMoreShards = jsmnGetVal(HASMORESHARDS_KEY, jsonCStr, tokens, tCount);
    char* serializedStreamName = jsmnGetVal(STREAMNAME_KEY, jsonCStr, tokens, tCount);
    char* serializedStreamARN = jsmnGetVal(STREAMARN_KEY, jsonCStr, tokens, tCount);
    char* serializedShards = jsmnGetVal(SHARDS_KEY, jsonCStr, tokens, tCount);
    char* serializedStreamStatus = jsmnGetVal(STREAMSTATUS_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedHasMoreShards != NULL) {
        hasMoreShardsBeenSet = true;
        jsonDeserializeBool(serializedHasMoreShards);
    }
    if (serializedStreamName != NULL) {
        streamNameBeenSet = true;
        streamName = MinimalString(serializedStreamName);
    }
    if (serializedStreamARN != NULL) {
        streamARNBeenSet = true;
        streamARN = MinimalString(serializedStreamARN);
    }
    if (serializedShards != NULL) {
        shardsBeenSet = true;
        if (!shards.jsonDeserialize(serializedShards)) {
            toReturn = false;
        }
    }
    if (serializedStreamStatus != NULL) {
        streamStatusBeenSet = true;
        int enumInt = jsonDeserializeEnum(STREAMSTATUS_LOOKUP, 4, MinimalString(serializedStreamStatus));
        if (enumInt == -1) {
                toReturn = false;
        }
    }
    if (serializedHasMoreShards != NULL) {
        delete[] serializedHasMoreShards;
    }
    if (serializedStreamName != NULL) {
        delete[] serializedStreamName;
    }
    if (serializedStreamARN != NULL) {
        delete[] serializedStreamARN;
    }
    if (serializedShards != NULL) {
        delete[] serializedShards;
    }
    if (serializedStreamStatus != NULL) {
        delete[] serializedStreamStatus;
    }
    if (tokens != NULL) {
        delete[] tokens;
    }
    return toReturn;
}

MinimalString StreamDescription::jsonSerialize() const {
    int itemCount = 0;
    if (hasMoreShardsBeenSet) {
        itemCount++;
    }
    if (streamNameBeenSet) {
        itemCount++;
    }
    if (streamARNBeenSet) {
        itemCount++;
    }
    if (shardsBeenSet) {
        itemCount++;
    }
    if (streamStatusBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (hasMoreShardsBeenSet) {
        MinimalString pairValue = jsonSerializeBool(hasMoreShards);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(HASMORESHARDS_KEY, pairValue);
    }
    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
    }
    if (streamARNBeenSet) {
        MinimalString pairValue = streamARN.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMARN_KEY, pairValue);
    }
    if (shardsBeenSet) {
        MinimalString pairValue = shards.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDS_KEY, pairValue);
    }
    if (streamStatusBeenSet) {
        MinimalString pairValue = MinimalString(STREAMSTATUS_LOOKUP[streamStatus]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMSTATUS_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void StreamDescription::setHasMoreShards(bool hasMoreShards) {
    hasMoreShardsBeenSet = true;
    this->hasMoreShards = hasMoreShards;
}

void StreamDescription::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

void StreamDescription::setStreamARN(MinimalString streamARN) {
    streamARNBeenSet = true;
    this->streamARN = streamARN;
}

void StreamDescription::setShards(MinimalList<Shard > shards) {
    shardsBeenSet = true;
    this->shards = shards;
}

void StreamDescription::setStreamStatus(StreamStatus streamStatus) {
    streamStatusBeenSet = true;
    this->streamStatus = streamStatus;
}

bool StreamDescription::getHasMoreShards() const {
    return hasMoreShards;
}

MinimalString StreamDescription::getStreamName() const {
    return streamName;
}

MinimalString StreamDescription::getStreamARN() const {
    return streamARN;
}

MinimalList<Shard > StreamDescription::getShards() const {
    return shards;
}

StreamStatus StreamDescription::getStreamStatus() const {
    return streamStatus;
}

GetShardIteratorInput::GetShardIteratorInput() {
    reset();
}

void GetShardIteratorInput::reset() {
    startingSequenceNumberBeenSet = false;
    streamNameBeenSet = false;
    shardIteratorTypeBeenSet = false;
    shardIdBeenSet = false;
}

bool GetShardIteratorInput::requiredAreSet() const {
    return streamNameBeenSet && shardIdBeenSet && shardIteratorTypeBeenSet;
}

MinimalString GetShardIteratorInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (startingSequenceNumberBeenSet) {
        itemCount++;
    }
    if (streamNameBeenSet) {
        itemCount++;
    }
    if (shardIteratorTypeBeenSet) {
        itemCount++;
    }
    if (shardIdBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (startingSequenceNumberBeenSet) {
        MinimalString pairValue = startingSequenceNumber.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STARTINGSEQUENCENUMBER_KEY, pairValue);
    }
    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
    }
    if (shardIteratorTypeBeenSet) {
        MinimalString pairValue = MinimalString(SHARDITERATORTYPE_LOOKUP[shardIteratorType]);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDITERATORTYPE_KEY, pairValue);
    }
    if (shardIdBeenSet) {
        MinimalString pairValue = shardId.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDID_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void GetShardIteratorInput::setStartingSequenceNumber(MinimalString startingSequenceNumber) {
    startingSequenceNumberBeenSet = true;
    this->startingSequenceNumber = startingSequenceNumber;
}

void GetShardIteratorInput::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

void GetShardIteratorInput::setShardIteratorType(ShardIteratorType shardIteratorType) {
    shardIteratorTypeBeenSet = true;
    this->shardIteratorType = shardIteratorType;
}

void GetShardIteratorInput::setShardId(MinimalString shardId) {
    shardIdBeenSet = true;
    this->shardId = shardId;
}

MinimalString GetShardIteratorInput::getStartingSequenceNumber() const {
    return startingSequenceNumber;
}

MinimalString GetShardIteratorInput::getStreamName() const {
    return streamName;
}

ShardIteratorType GetShardIteratorInput::getShardIteratorType() const {
    return shardIteratorType;
}

MinimalString GetShardIteratorInput::getShardId() const {
    return shardId;
}

GetRecordsInput::GetRecordsInput() {
    reset();
}

void GetRecordsInput::reset() {
    shardIteratorBeenSet = false;
    limitBeenSet = false;
}

bool GetRecordsInput::requiredAreSet() const {
    return shardIteratorBeenSet;
}

MinimalString GetRecordsInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (shardIteratorBeenSet) {
        itemCount++;
    }
    if (limitBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (shardIteratorBeenSet) {
        MinimalString pairValue = shardIterator.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDITERATOR_KEY, pairValue);
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

void GetRecordsInput::setShardIterator(MinimalString shardIterator) {
    shardIteratorBeenSet = true;
    this->shardIterator = shardIterator;
}

void GetRecordsInput::setLimit(int limit) {
    limitBeenSet = true;
    this->limit = limit;
}

MinimalString GetRecordsInput::getShardIterator() const {
    return shardIterator;
}

int GetRecordsInput::getLimit() const {
    return limit;
}

MergeShardsInput::MergeShardsInput() {
    reset();
}

void MergeShardsInput::reset() {
    adjacentShardToMergeBeenSet = false;
    streamNameBeenSet = false;
    shardToMergeBeenSet = false;
}

bool MergeShardsInput::requiredAreSet() const {
    return streamNameBeenSet && shardToMergeBeenSet && adjacentShardToMergeBeenSet;
}

MinimalString MergeShardsInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (adjacentShardToMergeBeenSet) {
        itemCount++;
    }
    if (streamNameBeenSet) {
        itemCount++;
    }
    if (shardToMergeBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (adjacentShardToMergeBeenSet) {
        MinimalString pairValue = adjacentShardToMerge.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(ADJACENTSHARDTOMERGE_KEY, pairValue);
    }
    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
    }
    if (shardToMergeBeenSet) {
        MinimalString pairValue = shardToMerge.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDTOMERGE_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void MergeShardsInput::setAdjacentShardToMerge(MinimalString adjacentShardToMerge) {
    adjacentShardToMergeBeenSet = true;
    this->adjacentShardToMerge = adjacentShardToMerge;
}

void MergeShardsInput::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

void MergeShardsInput::setShardToMerge(MinimalString shardToMerge) {
    shardToMergeBeenSet = true;
    this->shardToMerge = shardToMerge;
}

MinimalString MergeShardsInput::getAdjacentShardToMerge() const {
    return adjacentShardToMerge;
}

MinimalString MergeShardsInput::getStreamName() const {
    return streamName;
}

MinimalString MergeShardsInput::getShardToMerge() const {
    return shardToMerge;
}

DescribeStreamInput::DescribeStreamInput() {
    reset();
}

void DescribeStreamInput::reset() {
    exclusiveStartShardIdBeenSet = false;
    streamNameBeenSet = false;
    limitBeenSet = false;
}

bool DescribeStreamInput::requiredAreSet() const {
    return streamNameBeenSet;
}

MinimalString DescribeStreamInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (exclusiveStartShardIdBeenSet) {
        itemCount++;
    }
    if (streamNameBeenSet) {
        itemCount++;
    }
    if (limitBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (exclusiveStartShardIdBeenSet) {
        MinimalString pairValue = exclusiveStartShardId.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXCLUSIVESTARTSHARDID_KEY, pairValue);
    }
    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
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

void DescribeStreamInput::setExclusiveStartShardId(MinimalString exclusiveStartShardId) {
    exclusiveStartShardIdBeenSet = true;
    this->exclusiveStartShardId = exclusiveStartShardId;
}

void DescribeStreamInput::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

void DescribeStreamInput::setLimit(int limit) {
    limitBeenSet = true;
    this->limit = limit;
}

MinimalString DescribeStreamInput::getExclusiveStartShardId() const {
    return exclusiveStartShardId;
}

MinimalString DescribeStreamInput::getStreamName() const {
    return streamName;
}

int DescribeStreamInput::getLimit() const {
    return limit;
}

DeleteStreamInput::DeleteStreamInput() {
    reset();
}

void DeleteStreamInput::reset() {
    streamNameBeenSet = false;
}

bool DeleteStreamInput::requiredAreSet() const {
    return streamNameBeenSet;
}

MinimalString DeleteStreamInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (streamNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void DeleteStreamInput::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

MinimalString DeleteStreamInput::getStreamName() const {
    return streamName;
}

GetShardIteratorOutput::GetShardIteratorOutput() {
    reset();
}

void GetShardIteratorOutput::reset() {
    shardIteratorBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool GetShardIteratorOutput::jsonDeserialize(MinimalString json) {
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

    char* serializedShardIterator = jsmnGetVal(SHARDITERATOR_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(KINESIS_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(KINESIS_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedShardIterator != NULL) {
        shardIteratorBeenSet = true;
        shardIterator = MinimalString(serializedShardIterator);
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedShardIterator != NULL) {
        delete[] serializedShardIterator;
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

MinimalString GetShardIteratorOutput::getErrorType() const {
    return errorType;
}

MinimalString GetShardIteratorOutput::getErrorMessage() const {
    return errorMessage;
}

void GetShardIteratorOutput::setShardIterator(MinimalString shardIterator) {
    shardIteratorBeenSet = true;
    this->shardIterator = shardIterator;
}

MinimalString GetShardIteratorOutput::getShardIterator() const {
    return shardIterator;
}

CreateStreamInput::CreateStreamInput() {
    reset();
}

void CreateStreamInput::reset() {
    shardCountBeenSet = false;
    streamNameBeenSet = false;
}

bool CreateStreamInput::requiredAreSet() const {
    return streamNameBeenSet && shardCountBeenSet;
}

MinimalString CreateStreamInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (shardCountBeenSet) {
        itemCount++;
    }
    if (streamNameBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (shardCountBeenSet) {
        MinimalString pairValue = jsonSerializeInt(shardCount);
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDCOUNT_KEY, pairValue);
    }
    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void CreateStreamInput::setShardCount(int shardCount) {
    shardCountBeenSet = true;
    this->shardCount = shardCount;
}

void CreateStreamInput::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

int CreateStreamInput::getShardCount() const {
    return shardCount;
}

MinimalString CreateStreamInput::getStreamName() const {
    return streamName;
}

SplitShardInput::SplitShardInput() {
    reset();
}

void SplitShardInput::reset() {
    newStartingHashKeyBeenSet = false;
    streamNameBeenSet = false;
    shardToSplitBeenSet = false;
}

bool SplitShardInput::requiredAreSet() const {
    return streamNameBeenSet && shardToSplitBeenSet && newStartingHashKeyBeenSet;
}

MinimalString SplitShardInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (newStartingHashKeyBeenSet) {
        itemCount++;
    }
    if (streamNameBeenSet) {
        itemCount++;
    }
    if (shardToSplitBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (newStartingHashKeyBeenSet) {
        MinimalString pairValue = newStartingHashKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(NEWSTARTINGHASHKEY_KEY, pairValue);
    }
    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
    }
    if (shardToSplitBeenSet) {
        MinimalString pairValue = shardToSplit.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SHARDTOSPLIT_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void SplitShardInput::setNewStartingHashKey(MinimalString newStartingHashKey) {
    newStartingHashKeyBeenSet = true;
    this->newStartingHashKey = newStartingHashKey;
}

void SplitShardInput::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

void SplitShardInput::setShardToSplit(MinimalString shardToSplit) {
    shardToSplitBeenSet = true;
    this->shardToSplit = shardToSplit;
}

MinimalString SplitShardInput::getNewStartingHashKey() const {
    return newStartingHashKey;
}

MinimalString SplitShardInput::getStreamName() const {
    return streamName;
}

MinimalString SplitShardInput::getShardToSplit() const {
    return shardToSplit;
}

ListStreamsOutput::ListStreamsOutput() {
    reset();
}

void ListStreamsOutput::reset() {
    streamNamesBeenSet = false;
    hasMoreStreamsBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool ListStreamsOutput::jsonDeserialize(MinimalString json) {
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

    char* serializedStreamNames = jsmnGetVal(STREAMNAMES_KEY, jsonCStr, tokens, tCount);
    char* serializedHasMoreStreams = jsmnGetVal(HASMORESTREAMS_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(KINESIS_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(KINESIS_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedStreamNames != NULL) {
        streamNamesBeenSet = true;
        if (!streamNames.jsonDeserialize(serializedStreamNames)) {
            toReturn = false;
        }
    }
    if (serializedHasMoreStreams != NULL) {
        hasMoreStreamsBeenSet = true;
        jsonDeserializeBool(serializedHasMoreStreams);
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedStreamNames != NULL) {
        delete[] serializedStreamNames;
    }
    if (serializedHasMoreStreams != NULL) {
        delete[] serializedHasMoreStreams;
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

MinimalString ListStreamsOutput::getErrorType() const {
    return errorType;
}

MinimalString ListStreamsOutput::getErrorMessage() const {
    return errorMessage;
}

void ListStreamsOutput::setStreamNames(MinimalList<MinimalString > streamNames) {
    streamNamesBeenSet = true;
    this->streamNames = streamNames;
}

void ListStreamsOutput::setHasMoreStreams(bool hasMoreStreams) {
    hasMoreStreamsBeenSet = true;
    this->hasMoreStreams = hasMoreStreams;
}

MinimalList<MinimalString > ListStreamsOutput::getStreamNames() const {
    return streamNames;
}

bool ListStreamsOutput::getHasMoreStreams() const {
    return hasMoreStreams;
}

GetRecordsOutput::GetRecordsOutput() {
    reset();
}

void GetRecordsOutput::reset() {
    nextShardIteratorBeenSet = false;
    recordsBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool GetRecordsOutput::jsonDeserialize(MinimalString json) {
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

    char* serializedNextShardIterator = jsmnGetVal(NEXTSHARDITERATOR_KEY, jsonCStr, tokens, tCount);
    char* serializedRecords = jsmnGetVal(RECORDS_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(KINESIS_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(KINESIS_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedNextShardIterator != NULL) {
        nextShardIteratorBeenSet = true;
        nextShardIterator = MinimalString(serializedNextShardIterator);
    }
    if (serializedRecords != NULL) {
        recordsBeenSet = true;
        if (!records.jsonDeserialize(serializedRecords)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedNextShardIterator != NULL) {
        delete[] serializedNextShardIterator;
    }
    if (serializedRecords != NULL) {
        delete[] serializedRecords;
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

MinimalString GetRecordsOutput::getErrorType() const {
    return errorType;
}

MinimalString GetRecordsOutput::getErrorMessage() const {
    return errorMessage;
}

void GetRecordsOutput::setNextShardIterator(MinimalString nextShardIterator) {
    nextShardIteratorBeenSet = true;
    this->nextShardIterator = nextShardIterator;
}

void GetRecordsOutput::setRecords(MinimalList<Record > records) {
    recordsBeenSet = true;
    this->records = records;
}

MinimalString GetRecordsOutput::getNextShardIterator() const {
    return nextShardIterator;
}

MinimalList<Record > GetRecordsOutput::getRecords() const {
    return records;
}

ListStreamsInput::ListStreamsInput() {
    reset();
}

void ListStreamsInput::reset() {
    exclusiveStartStreamNameBeenSet = false;
    limitBeenSet = false;
}

bool ListStreamsInput::requiredAreSet() const {
    return true;
}

MinimalString ListStreamsInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (exclusiveStartStreamNameBeenSet) {
        itemCount++;
    }
    if (limitBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (exclusiveStartStreamNameBeenSet) {
        MinimalString pairValue = exclusiveStartStreamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXCLUSIVESTARTSTREAMNAME_KEY, pairValue);
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

void ListStreamsInput::setExclusiveStartStreamName(MinimalString exclusiveStartStreamName) {
    exclusiveStartStreamNameBeenSet = true;
    this->exclusiveStartStreamName = exclusiveStartStreamName;
}

void ListStreamsInput::setLimit(int limit) {
    limitBeenSet = true;
    this->limit = limit;
}

MinimalString ListStreamsInput::getExclusiveStartStreamName() const {
    return exclusiveStartStreamName;
}

int ListStreamsInput::getLimit() const {
    return limit;
}

PutRecordInput::PutRecordInput() {
    reset();
}

void PutRecordInput::reset() {
    dataBeenSet = false;
    explicitHashKeyBeenSet = false;
    sequenceNumberForOrderingBeenSet = false;
    streamNameBeenSet = false;
    partitionKeyBeenSet = false;
}

bool PutRecordInput::requiredAreSet() const {
    return streamNameBeenSet && dataBeenSet && partitionKeyBeenSet;
}

MinimalString PutRecordInput::jsonSerialize() const {
    if (!requiredAreSet()) {
        MinimalString emptyStr;
        return emptyStr;
    }
    int itemCount = 0;
    if (dataBeenSet) {
        itemCount++;
    }
    if (explicitHashKeyBeenSet) {
        itemCount++;
    }
    if (sequenceNumberForOrderingBeenSet) {
        itemCount++;
    }
    if (streamNameBeenSet) {
        itemCount++;
    }
    if (partitionKeyBeenSet) {
        itemCount++;
    }

    MinimalKeyValuePair<MinimalString, MinimalString>* pairs = new MinimalKeyValuePair<MinimalString, MinimalString> [itemCount]();
    int pairsAssigned = 0;

    if (dataBeenSet) {
        MinimalString pairValue = data.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(DATA_KEY, pairValue);
    }
    if (explicitHashKeyBeenSet) {
        MinimalString pairValue = explicitHashKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(EXPLICITHASHKEY_KEY, pairValue);
    }
    if (sequenceNumberForOrderingBeenSet) {
        MinimalString pairValue = sequenceNumberForOrdering.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(SEQUENCENUMBERFORORDERING_KEY, pairValue);
    }
    if (streamNameBeenSet) {
        MinimalString pairValue = streamName.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(STREAMNAME_KEY, pairValue);
    }
    if (partitionKeyBeenSet) {
        MinimalString pairValue = partitionKey.jsonSerialize();
        pairValue.setAlreadySerialized(true);
        pairs[pairsAssigned++] = MinimalKeyValuePair<MinimalString,MinimalString>(PARTITIONKEY_KEY, pairValue);
    }

    MinimalMap<MinimalString> map(pairs, itemCount);
    delete[] pairs;
    return map.jsonSerialize();
}

void PutRecordInput::setData(MinimalString data) {
    dataBeenSet = true;
    this->data = data;
}

void PutRecordInput::setExplicitHashKey(MinimalString explicitHashKey) {
    explicitHashKeyBeenSet = true;
    this->explicitHashKey = explicitHashKey;
}

void PutRecordInput::setSequenceNumberForOrdering(MinimalString sequenceNumberForOrdering) {
    sequenceNumberForOrderingBeenSet = true;
    this->sequenceNumberForOrdering = sequenceNumberForOrdering;
}

void PutRecordInput::setStreamName(MinimalString streamName) {
    streamNameBeenSet = true;
    this->streamName = streamName;
}

void PutRecordInput::setPartitionKey(MinimalString partitionKey) {
    partitionKeyBeenSet = true;
    this->partitionKey = partitionKey;
}

MinimalString PutRecordInput::getData() const {
    return data;
}

MinimalString PutRecordInput::getExplicitHashKey() const {
    return explicitHashKey;
}

MinimalString PutRecordInput::getSequenceNumberForOrdering() const {
    return sequenceNumberForOrdering;
}

MinimalString PutRecordInput::getStreamName() const {
    return streamName;
}

MinimalString PutRecordInput::getPartitionKey() const {
    return partitionKey;
}

KinesisErrorCheckingOnlyOutput::KinesisErrorCheckingOnlyOutput() {
    reset();
}

void KinesisErrorCheckingOnlyOutput::reset() {
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool KinesisErrorCheckingOnlyOutput::jsonDeserialize(MinimalString json) {
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

    char* errorTypeCStr = jsmnGetVal(KINESIS_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(KINESIS_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
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

MinimalString KinesisErrorCheckingOnlyOutput::getErrorType() const {
    return errorType;
}

MinimalString KinesisErrorCheckingOnlyOutput::getErrorMessage() const {
    return errorMessage;
}

DescribeStreamOutput::DescribeStreamOutput() {
    reset();
}

void DescribeStreamOutput::reset() {
    streamDescriptionBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool DescribeStreamOutput::jsonDeserialize(MinimalString json) {
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

    char* serializedStreamDescription = jsmnGetVal(STREAMDESCRIPTION_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(KINESIS_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(KINESIS_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedStreamDescription != NULL) {
        streamDescriptionBeenSet = true;
        if (!streamDescription.jsonDeserialize(serializedStreamDescription)) {
            toReturn = false;
        }
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedStreamDescription != NULL) {
        delete[] serializedStreamDescription;
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

MinimalString DescribeStreamOutput::getErrorType() const {
    return errorType;
}

MinimalString DescribeStreamOutput::getErrorMessage() const {
    return errorMessage;
}

void DescribeStreamOutput::setStreamDescription(StreamDescription streamDescription) {
    streamDescriptionBeenSet = true;
    this->streamDescription = streamDescription;
}

StreamDescription DescribeStreamOutput::getStreamDescription() const {
    return streamDescription;
}

PutRecordOutput::PutRecordOutput() {
    reset();
}

void PutRecordOutput::reset() {
    shardIdBeenSet = false;
    sequenceNumberBeenSet = false;
    errorType = MinimalString();
    errorMessage = MinimalString();
}

bool PutRecordOutput::jsonDeserialize(MinimalString json) {
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

    char* serializedShardId = jsmnGetVal(SHARDID_KEY, jsonCStr, tokens, tCount);
    char* serializedSequenceNumber = jsmnGetVal(SEQUENCENUMBER_KEY, jsonCStr, tokens, tCount);
    char* errorTypeCStr = jsmnGetVal(KINESIS_ERROR_TYPE_KEY, jsonCStr, tokens, tCount);
    char* errorMessageCStr = jsmnGetVal(KINESIS_ERROR_MESSAGE_KEY, jsonCStr, tokens, tCount);

    bool toReturn = true;

    if (serializedShardId != NULL) {
        shardIdBeenSet = true;
        shardId = MinimalString(serializedShardId);
    }
    if (serializedSequenceNumber != NULL) {
        sequenceNumberBeenSet = true;
        sequenceNumber = MinimalString(serializedSequenceNumber);
    }
    if (errorMessageCStr != NULL) {
        errorMessage = MinimalString(errorMessageCStr);
    }
    if (errorTypeCStr != NULL) {
        errorType = MinimalString(errorTypeCStr);
    }
    if (serializedShardId != NULL) {
        delete[] serializedShardId;
    }
    if (serializedSequenceNumber != NULL) {
        delete[] serializedSequenceNumber;
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

MinimalString PutRecordOutput::getErrorType() const {
    return errorType;
}

MinimalString PutRecordOutput::getErrorMessage() const {
    return errorMessage;
}

void PutRecordOutput::setShardId(MinimalString shardId) {
    shardIdBeenSet = true;
    this->shardId = shardId;
}

void PutRecordOutput::setSequenceNumber(MinimalString sequenceNumber) {
    sequenceNumberBeenSet = true;
    this->sequenceNumber = sequenceNumber;
}

MinimalString PutRecordOutput::getShardId() const {
    return shardId;
}

MinimalString PutRecordOutput::getSequenceNumber() const {
    return sequenceNumber;
}

AmazonKinesisClient::AmazonKinesisClient() : AWSClient() {
    awsService = SERVICE;
}

KinesisErrorCheckingOnlyOutput AmazonKinesisClient::createStream(CreateStreamInput createStreamInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    KinesisErrorCheckingOnlyOutput kinesisErrorCheckingOnlyOutput;

    if (!createStreamInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return kinesisErrorCheckingOnlyOutput;
    }
    contentType = JSON_TYPE;
    target = CREATESTREAM_TARGET;
    httpS = true;
    MinimalString payload = createStreamInput.jsonSerialize();
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
        return kinesisErrorCheckingOnlyOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    if (!kinesisErrorCheckingOnlyOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    delete[] response;
    MinimalString errorType = kinesisErrorCheckingOnlyOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(kinesisErrorCheckingOnlyOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return createStream(createStreamInput, actionError, false, httpStatusCode);
        }
    }
    return kinesisErrorCheckingOnlyOutput;
}

KinesisErrorCheckingOnlyOutput AmazonKinesisClient::deleteStream(DeleteStreamInput deleteStreamInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    KinesisErrorCheckingOnlyOutput kinesisErrorCheckingOnlyOutput;

    if (!deleteStreamInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return kinesisErrorCheckingOnlyOutput;
    }
    contentType = JSON_TYPE;
    target = DELETESTREAM_TARGET;
    httpS = true;
    MinimalString payload = deleteStreamInput.jsonSerialize();
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
        return kinesisErrorCheckingOnlyOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    if (!kinesisErrorCheckingOnlyOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    delete[] response;
    MinimalString errorType = kinesisErrorCheckingOnlyOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(kinesisErrorCheckingOnlyOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return deleteStream(deleteStreamInput, actionError, false, httpStatusCode);
        }
    }
    return kinesisErrorCheckingOnlyOutput;
}

DescribeStreamOutput AmazonKinesisClient::describeStream(DescribeStreamInput describeStreamInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    DescribeStreamOutput describeStreamOutput;

    if (!describeStreamInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return describeStreamOutput;
    }
    contentType = JSON_TYPE;
    target = DESCRIBESTREAM_TARGET;
    httpS = true;
    MinimalString payload = describeStreamInput.jsonSerialize();
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
        return describeStreamOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return describeStreamOutput;
    }
    if (!describeStreamOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return describeStreamOutput;
    }
    delete[] response;
    MinimalString errorType = describeStreamOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(describeStreamOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return describeStream(describeStreamInput, actionError, false, httpStatusCode);
        }
    }
    return describeStreamOutput;
}

GetRecordsOutput AmazonKinesisClient::getRecords(GetRecordsInput getRecordsInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    GetRecordsOutput getRecordsOutput;

    if (!getRecordsInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return getRecordsOutput;
    }
    contentType = JSON_TYPE;
    target = GETRECORDS_TARGET;
    httpS = true;
    MinimalString payload = getRecordsInput.jsonSerialize();
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
        return getRecordsOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return getRecordsOutput;
    }
    if (!getRecordsOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return getRecordsOutput;
    }
    delete[] response;
    MinimalString errorType = getRecordsOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(getRecordsOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return getRecords(getRecordsInput, actionError, false, httpStatusCode);
        }
    }
    return getRecordsOutput;
}

GetShardIteratorOutput AmazonKinesisClient::getShardIterator(GetShardIteratorInput getShardIteratorInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    GetShardIteratorOutput getShardIteratorOutput;

    if (!getShardIteratorInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return getShardIteratorOutput;
    }
    contentType = JSON_TYPE;
    target = GETSHARDITERATOR_TARGET;
    httpS = true;
    MinimalString payload = getShardIteratorInput.jsonSerialize();
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
        return getShardIteratorOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return getShardIteratorOutput;
    }
    if (!getShardIteratorOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return getShardIteratorOutput;
    }
    delete[] response;
    MinimalString errorType = getShardIteratorOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(getShardIteratorOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return getShardIterator(getShardIteratorInput, actionError, false, httpStatusCode);
        }
    }
    return getShardIteratorOutput;
}

ListStreamsOutput AmazonKinesisClient::listStreams(ListStreamsInput listStreamsInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    ListStreamsOutput listStreamsOutput;

    if (!listStreamsInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return listStreamsOutput;
    }
    contentType = JSON_TYPE;
    target = LISTSTREAMS_TARGET;
    httpS = true;
    MinimalString payload = listStreamsInput.jsonSerialize();
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
        return listStreamsOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return listStreamsOutput;
    }
    if (!listStreamsOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return listStreamsOutput;
    }
    delete[] response;
    MinimalString errorType = listStreamsOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(listStreamsOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return listStreams(listStreamsInput, actionError, false, httpStatusCode);
        }
    }
    return listStreamsOutput;
}

KinesisErrorCheckingOnlyOutput AmazonKinesisClient::mergeShards(MergeShardsInput mergeShardsInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    KinesisErrorCheckingOnlyOutput kinesisErrorCheckingOnlyOutput;

    if (!mergeShardsInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return kinesisErrorCheckingOnlyOutput;
    }
    contentType = JSON_TYPE;
    target = MERGESHARDS_TARGET;
    httpS = true;
    MinimalString payload = mergeShardsInput.jsonSerialize();
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
        return kinesisErrorCheckingOnlyOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    if (!kinesisErrorCheckingOnlyOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    delete[] response;
    MinimalString errorType = kinesisErrorCheckingOnlyOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(kinesisErrorCheckingOnlyOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return mergeShards(mergeShardsInput, actionError, false, httpStatusCode);
        }
    }
    return kinesisErrorCheckingOnlyOutput;
}

PutRecordOutput AmazonKinesisClient::putRecord(PutRecordInput putRecordInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    PutRecordOutput putRecordOutput;

    if (!putRecordInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return putRecordOutput;
    }
    contentType = JSON_TYPE;
    target = PUTRECORD_TARGET;
    httpS = true;
    MinimalString payload = putRecordInput.jsonSerialize();
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
        return putRecordOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return putRecordOutput;
    }
    if (!putRecordOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return putRecordOutput;
    }
    delete[] response;
    MinimalString errorType = putRecordOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(putRecordOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return putRecord(putRecordInput, actionError, false, httpStatusCode);
        }
    }
    return putRecordOutput;
}

KinesisErrorCheckingOnlyOutput AmazonKinesisClient::splitShard(SplitShardInput splitShardInput, ActionError& actionError, bool retry, int* httpStatusCode) {
    actionError = NONE_ACTIONERROR;
    KinesisErrorCheckingOnlyOutput kinesisErrorCheckingOnlyOutput;

    if (!splitShardInput.requiredAreSet()) {
        actionError = MISSING_REQUIRED_ARGS_ACTIONERROR;
        return kinesisErrorCheckingOnlyOutput;
    }
    contentType = JSON_TYPE;
    target = SPLITSHARD_TARGET;
    httpS = true;
    MinimalString payload = splitShardInput.jsonSerialize();
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
        return kinesisErrorCheckingOnlyOutput;
    }
    if (httpStatusCode) {
        *httpStatusCode = findHttpStatusCode(response);
    }
    int start, end;
    if (!findJsonStartEnd(response, &start, &end)) {
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    if (!kinesisErrorCheckingOnlyOutput.jsonDeserialize(MinimalString(response + start, end - start + 1))) {
        actionError = RESPONSE_PARSING_ACTIONERROR;
        delete[] response;
        return kinesisErrorCheckingOnlyOutput;
    }
    delete[] response;
    MinimalString errorType = kinesisErrorCheckingOnlyOutput.getErrorType();
    if (errorType.length() != 0) {
        actionError = INVALID_REQUEST_ACTIONERROR;
        if (retry && !strcmp(errorType.getCStr(), KINESIS_SIGNATURE_ERROR_TYPE)) {
            const char* syncArg = dateTimeProvider->syncTakesArg() ? getTimeFromInvalidSignatureMessage(kinesisErrorCheckingOnlyOutput.getErrorMessage().getCStr()) : 0;
            dateTimeProvider->sync(syncArg);
            return splitShard(splitShardInput, actionError, false, httpStatusCode);
        }
    }
    return kinesisErrorCheckingOnlyOutput;
}
