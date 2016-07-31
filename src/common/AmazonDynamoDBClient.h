#ifndef AMAZONDYNAMODBCLIENT_H_
#define AMAZONDYNAMODBCLIENT_H_
#include "AWSFoundationalTypes.h"
#include "AWSClient.h"

enum ReturnValue{
    NONE_RETURNVALUE,
    ALL_OLD_RETURNVALUE,
    UPDATED_OLD_RETURNVALUE,
    ALL_NEW_RETURNVALUE,
    UPDATED_NEW_RETURNVALUE
};

enum ConditionalOperator{
    AND_CONDITIONALOPERATOR,
    OR_CONDITIONALOPERATOR
};

enum TableStatus{
    CREATING_TABLESTATUS,
    UPDATING_TABLESTATUS,
    DELETING_TABLESTATUS,
    ACTIVE_TABLESTATUS
};

/* <p>If set to <code>TOTAL</code>, the response includes <i>ConsumedCapacity</i> data for tables and indexes. If set to <code>INDEXES</code>, the response includes <i>ConsumedCapacity</i> for indexes. If set to <code>NONE</code> (the default), <i>ConsumedCapacity</i> is not included in the response.</p> */
enum ReturnConsumedCapacity{
    INDEXES_RETURNCONSUMEDCAPACITY,
    TOTAL_RETURNCONSUMEDCAPACITY,
    NONE_RETURNCONSUMEDCAPACITY
};

enum ProjectionType{
    ALL_PROJECTIONTYPE,
    KEYS_ONLY_PROJECTIONTYPE,
    INCLUDE_PROJECTIONTYPE
};

enum ComparisonOperator{
    EQ_COMPARISONOPERATOR,
    NE_COMPARISONOPERATOR,
    IN_COMPARISONOPERATOR,
    LE_COMPARISONOPERATOR,
    LT_COMPARISONOPERATOR,
    GE_COMPARISONOPERATOR,
    GT_COMPARISONOPERATOR,
    BETWEEN_COMPARISONOPERATOR,
    NOT_NULL_COMPARISONOPERATOR,
    NULL_COMPARISONOPERATOR,
    CONTAINS_COMPARISONOPERATOR,
    NOT_CONTAINS_COMPARISONOPERATOR,
    BEGINS_WITH_COMPARISONOPERATOR
};

enum ReturnItemCollectionMetrics{
    SIZE_RETURNITEMCOLLECTIONMETRICS,
    NONE_RETURNITEMCOLLECTIONMETRICS
};

enum ScalarAttributeType{
    S_SCALARATTRIBUTETYPE,
    N_SCALARATTRIBUTETYPE,
    B_SCALARATTRIBUTETYPE
};

enum IndexStatus{
    CREATING_INDEXSTATUS,
    UPDATING_INDEXSTATUS,
    DELETING_INDEXSTATUS,
    ACTIVE_INDEXSTATUS
};

enum Select{
    ALL_ATTRIBUTES_SELECT,
    ALL_PROJECTED_ATTRIBUTES_SELECT,
    SPECIFIC_ATTRIBUTES_SELECT,
    COUNT_SELECT
};

enum KeyType{
    HASH_KEYTYPE,
    RANGE_KEYTYPE
};

enum AttributeAction{
    ADD_ATTRIBUTEACTION,
    PUT_ATTRIBUTEACTION,
    DELETE_ATTRIBUTEACTION
};

/* <p>Represents <i>a single element</i> of a key schema. A key schema specifies the attributes that make up the primary key of a table, or the key attributes of an index.</p><p>A <i>KeySchemaElement</i> represents exactly one attribute of the primary key. For example, a hash type primary key would be represented by one <i>KeySchemaElement</i>. A hash-and-range type primary key would require one <i>KeySchemaElement</i> for the hash attribute, and another <i>KeySchemaElement</i> for the range attribute.</p> */
class KeySchemaElement{
    KeyType keyType;
    MinimalString attributeName;
    bool keyTypeBeenSet;
    bool attributeNameBeenSet;
    void reset();
public:
    KeySchemaElement();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setKeyType(KeyType keyType);
    void setAttributeName(MinimalString attributeName);
    KeyType getKeyType() const;
    MinimalString getAttributeName() const;
};

/* <p>Represents attributes that are copied (projected) from the table into an index. These are in addition to the primary key attributes and index key attributes, which are automatically projected.</p> */
class Projection{
    ProjectionType projectionType;
    MinimalList<MinimalString > nonKeyAttributes;
    bool projectionTypeBeenSet;
    bool nonKeyAttributesBeenSet;
    void reset();
public:
    Projection();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setProjectionType(ProjectionType projectionType);
    void setNonKeyAttributes(MinimalList<MinimalString > nonKeyAttributes);
    ProjectionType getProjectionType() const;
    MinimalList<MinimalString > getNonKeyAttributes() const;
};

/* <p>Represents the provisioned throughput settings for the table, consisting of read and write capacity units, along with data about increases and decreases.</p> */
class ProvisionedThroughputDescription{
    long writeCapacityUnits;
    long numberOfDecreasesToday;
    MinimalString lastIncreaseDateTime;
    MinimalString lastDecreaseDateTime;
    long readCapacityUnits;
    bool writeCapacityUnitsBeenSet;
    bool numberOfDecreasesTodayBeenSet;
    bool lastIncreaseDateTimeBeenSet;
    bool lastDecreaseDateTimeBeenSet;
    bool readCapacityUnitsBeenSet;
    void reset();
public:
    ProvisionedThroughputDescription();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setWriteCapacityUnits(long writeCapacityUnits);
    void setNumberOfDecreasesToday(long numberOfDecreasesToday);
    void setLastIncreaseDateTime(MinimalString lastIncreaseDateTime);
    void setLastDecreaseDateTime(MinimalString lastDecreaseDateTime);
    void setReadCapacityUnits(long readCapacityUnits);
    long getWriteCapacityUnits() const;
    long getNumberOfDecreasesToday() const;
    MinimalString getLastIncreaseDateTime() const;
    MinimalString getLastDecreaseDateTime() const;
    long getReadCapacityUnits() const;
};

/* <p>Represents the data for an attribute. You can set one, and only one, of the elements.</p> */
class AttributeValue{
    MinimalList<MinimalString > SS;
    MinimalList<MinimalString > BS;
    MinimalString B;
    MinimalString S;
    MinimalList<MinimalString > NS;
    MinimalString N;
    bool SSBeenSet;
    bool BSBeenSet;
    bool BBeenSet;
    bool SBeenSet;
    bool NSBeenSet;
    bool NBeenSet;
    void reset();
public:
    AttributeValue();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setSS(MinimalList<MinimalString > SSS);
    void setBS(MinimalList<MinimalString > BS);
    void setB(MinimalString B);
    void setS(MinimalString S);
    void setNS(MinimalList<MinimalString > NS);
    void setN(MinimalString N);
    MinimalList<MinimalString > getSS() const;
    MinimalList<MinimalString > getBS() const;
    MinimalString getB() const;
    MinimalString getS() const;
    MinimalList<MinimalString > getNS() const;
    MinimalString getN() const;
};

/* <p>Represents the provisioned throughput settings for a specified table or index. The settings can be modified using the <i>UpdateTable</i> operation.</p><p>For current minimum and maximum provisioned throughput values, see <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Limits.html">Limits</a> in the Amazon DynamoDB Developer Guide.</p> */
class ProvisionedThroughput{
    long writeCapacityUnits;
    long readCapacityUnits;
    bool writeCapacityUnitsBeenSet;
    bool readCapacityUnitsBeenSet;
    void reset();
public:
    ProvisionedThroughput();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setWriteCapacityUnits(long writeCapacityUnits);
    void setReadCapacityUnits(long readCapacityUnits);
    long getWriteCapacityUnits() const;
    long getReadCapacityUnits() const;
};

/* <p>Represents an attribute for describing the key schema for the table and indexes.</p> */
class AttributeDefinition{
    ScalarAttributeType attributeType;
    MinimalString attributeName;
    bool attributeTypeBeenSet;
    bool attributeNameBeenSet;
    void reset();
public:
    AttributeDefinition();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setAttributeType(ScalarAttributeType attributeType);
    void setAttributeName(MinimalString attributeName);
    ScalarAttributeType getAttributeType() const;
    MinimalString getAttributeName() const;
};

/* <p>Represents the properties of a local secondary index.</p> */
class LocalSecondaryIndexDescription{
    Projection projection;
    long itemCount;
    long indexSizeBytes;
    MinimalList<KeySchemaElement > keySchema;
    MinimalString indexName;
    bool projectionBeenSet;
    bool itemCountBeenSet;
    bool indexSizeBytesBeenSet;
    bool keySchemaBeenSet;
    bool indexNameBeenSet;
    void reset();
public:
    LocalSecondaryIndexDescription();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setProjection(Projection projection);
    void setItemCount(long itemCount);
    void setIndexSizeBytes(long indexSizeBytes);
    void setKeySchema(MinimalList<KeySchemaElement > keySchema);
    void setIndexName(MinimalString indexName);
    Projection getProjection() const;
    long getItemCount() const;
    long getIndexSizeBytes() const;
    MinimalList<KeySchemaElement > getKeySchema() const;
    MinimalString getIndexName() const;
};

/* <p>Represents the properties of a global secondary index.</p> */
class GlobalSecondaryIndexDescription{
    Projection projection;
    ProvisionedThroughputDescription provisionedThroughput;
    IndexStatus indexStatus;
    long itemCount;
    long indexSizeBytes;
    MinimalList<KeySchemaElement > keySchema;
    MinimalString indexName;
    bool projectionBeenSet;
    bool provisionedThroughputBeenSet;
    bool indexStatusBeenSet;
    bool itemCountBeenSet;
    bool indexSizeBytesBeenSet;
    bool keySchemaBeenSet;
    bool indexNameBeenSet;
    void reset();
public:
    GlobalSecondaryIndexDescription();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setProjection(Projection projection);
    void setProvisionedThroughput(ProvisionedThroughputDescription provisionedThroughput);
    void setIndexStatus(IndexStatus indexStatus);
    void setItemCount(long itemCount);
    void setIndexSizeBytes(long indexSizeBytes);
    void setKeySchema(MinimalList<KeySchemaElement > keySchema);
    void setIndexName(MinimalString indexName);
    Projection getProjection() const;
    ProvisionedThroughputDescription getProvisionedThroughput() const;
    IndexStatus getIndexStatus() const;
    long getItemCount() const;
    long getIndexSizeBytes() const;
    MinimalList<KeySchemaElement > getKeySchema() const;
    MinimalString getIndexName() const;
};

/* <p>Represents the amount of provisioned throughput capacity consumed on a table or an index. </p> */
class Capacity{
    double capacityUnits;
    bool capacityUnitsBeenSet;
    void reset();
public:
    Capacity();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setCapacityUnits(double capacityUnits);
    double getCapacityUnits() const;
};

/* <p>Represents a request to perform a <i>PutItem</i> operation on an item.</p> */
class PutRequest{
    MinimalMap<AttributeValue > item;
    bool itemBeenSet;
    void reset();
public:
    PutRequest();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setItem(MinimalMap<AttributeValue > item);
    MinimalMap<AttributeValue > getItem() const;
};

/* <p>Represents a request to perform a <i>DeleteItem</i> operation on an item.</p> */
class DeleteRequest{
    MinimalMap<AttributeValue > key;
    bool keyBeenSet;
    void reset();
public:
    DeleteRequest();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setKey(MinimalMap<AttributeValue > key);
    MinimalMap<AttributeValue > getKey() const;
};

/* <p>Represents the new provisioned throughput settings to be applied to a global secondary index.</p> */
class UpdateGlobalSecondaryIndexAction{
    ProvisionedThroughput provisionedThroughput;
    MinimalString indexName;
    bool provisionedThroughputBeenSet;
    bool indexNameBeenSet;
    void reset();
public:
    UpdateGlobalSecondaryIndexAction();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setProvisionedThroughput(ProvisionedThroughput provisionedThroughput);
    void setIndexName(MinimalString indexName);
    ProvisionedThroughput getProvisionedThroughput() const;
    MinimalString getIndexName() const;
};

/* <p>Represents the properties of a table.</p> */
class TableDescription{
    ProvisionedThroughputDescription provisionedThroughput;
    MinimalString creationDateTime;
    long itemCount;
    MinimalList<GlobalSecondaryIndexDescription > globalSecondaryIndexes;
    TableStatus tableStatus;
    MinimalString tableName;
    MinimalList<LocalSecondaryIndexDescription > localSecondaryIndexes;
    MinimalList<KeySchemaElement > keySchema;
    MinimalList<AttributeDefinition > attributeDefinitions;
    long tableSizeBytes;
    bool provisionedThroughputBeenSet;
    bool creationDateTimeBeenSet;
    bool itemCountBeenSet;
    bool globalSecondaryIndexesBeenSet;
    bool tableStatusBeenSet;
    bool tableNameBeenSet;
    bool localSecondaryIndexesBeenSet;
    bool keySchemaBeenSet;
    bool attributeDefinitionsBeenSet;
    bool tableSizeBytesBeenSet;
    void reset();
public:
    TableDescription();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setProvisionedThroughput(ProvisionedThroughputDescription provisionedThroughput);
    void setCreationDateTime(MinimalString creationDateTime);
    void setItemCount(long itemCount);
    void setGlobalSecondaryIndexes(MinimalList<GlobalSecondaryIndexDescription > globalSecondaryIndexes);
    void setTableStatus(TableStatus tableStatus);
    void setTableName(MinimalString tableName);
    void setLocalSecondaryIndexes(MinimalList<LocalSecondaryIndexDescription > localSecondaryIndexes);
    void setKeySchema(MinimalList<KeySchemaElement > keySchema);
    void setAttributeDefinitions(MinimalList<AttributeDefinition > attributeDefinitions);
    void setTableSizeBytes(long tableSizeBytes);
    ProvisionedThroughputDescription getProvisionedThroughput() const;
    MinimalString getCreationDateTime() const;
    long getItemCount() const;
    MinimalList<GlobalSecondaryIndexDescription > getGlobalSecondaryIndexes() const;
    TableStatus getTableStatus() const;
    MinimalString getTableName() const;
    MinimalList<LocalSecondaryIndexDescription > getLocalSecondaryIndexes() const;
    MinimalList<KeySchemaElement > getKeySchema() const;
    MinimalList<AttributeDefinition > getAttributeDefinitions() const;
    long getTableSizeBytes() const;
};

/* <p>Represents the capacity units consumed by an operation. The data returned includes the total provisioned throughput consumed, along with statistics for the table and any indexes involved in the operation. <i>ConsumedCapacity</i> is only returned if it was asked for in the request. For more information, see <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/ProvisionedThroughputIntro.html">Provisioned Throughput</a> in the Amazon DynamoDB Developer Guide.</p> */
class ConsumedCapacity{
    double capacityUnits;
    MinimalMap<Capacity > globalSecondaryIndexes;
    MinimalString tableName;
    MinimalMap<Capacity > localSecondaryIndexes;
    Capacity table;
    bool capacityUnitsBeenSet;
    bool globalSecondaryIndexesBeenSet;
    bool tableNameBeenSet;
    bool localSecondaryIndexesBeenSet;
    bool tableBeenSet;
    void reset();
public:
    ConsumedCapacity();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setCapacityUnits(double capacityUnits);
    void setGlobalSecondaryIndexes(MinimalMap<Capacity > globalSecondaryIndexes);
    void setTableName(MinimalString tableName);
    void setLocalSecondaryIndexes(MinimalMap<Capacity > localSecondaryIndexes);
    void setTable(Capacity table);
    double getCapacityUnits() const;
    MinimalMap<Capacity > getGlobalSecondaryIndexes() const;
    MinimalString getTableName() const;
    MinimalMap<Capacity > getLocalSecondaryIndexes() const;
    Capacity getTable() const;
};

/* <p>Information about item collections, if any, that were affected by the operation. <i>ItemCollectionMetrics</i> is only returned if it was asked for in the request. If the table does not have any local secondary indexes, this information is not returned in the response.</p> */
class ItemCollectionMetrics{
    MinimalList<SerializableDouble > sizeEstimateRangeGB;
    MinimalMap<AttributeValue > itemCollectionKey;
    bool sizeEstimateRangeGBBeenSet;
    bool itemCollectionKeyBeenSet;
    void reset();
public:
    ItemCollectionMetrics();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setSizeEstimateRangeGB(MinimalList<SerializableDouble > sizeEstimateRangeGB);
    void setItemCollectionKey(MinimalMap<AttributeValue > itemCollectionKey);
    MinimalList<SerializableDouble > getSizeEstimateRangeGB() const;
    MinimalMap<AttributeValue > getItemCollectionKey() const;
};

/* <p>Represents a condition to be compared with an attribute value. This condition can be used with <i>DeleteItem</i>, <i>PutItem</i> or <i>UpdateItem</i> operations; if the comparison evaluates to true, the operation succeeds; if not the operation fails. You can use <i>ExpectedAttributeValue</i> in one of two different ways:</p><ul><li><p>Use <i>AttributeValueList</i> to specify one or more values to compare against an attribute. Use <i>ComparisonOperator</i> to specify how you want to perform the comparison. If the comparison evaluates to true, then the conditional operation succeeds.</p></li><li><p>Use <i>Value</i> to specify a value that DynamoDB will compare against an attribute. If the values match, then <i>ExpectedAttributeValue</i> evaluates to true and the conditional operation succeeds. Optionally, you can also set <i>Exists</i> to false, indicating that you <i>do not</i> expect to find the attribute value in the table. In this case, the conditional operation succeeds only if the comparison evaluates to false.</p></li></ul><p><i>Value</i> and <i>Exists</i> are incompatible with <i>AttributeValueList</i> and <i>ComparisonOperator</i>. If you attempt to use both sets of parameters at once, DynamoDB will throw a <i>ValidationException</i>.</p><important><p>The <i>Value</i> and <i>Exists</i> parameters are deprecated. Even though DynamoDB will continue to support these parameters, we recommend that you use <i>AttributeValueList</i> and <i>ComparisonOperator</i> instead. <i>AttributeValueList</i> and <i>ComparisonOperator</i> let you construct a much wider range of conditions than is possible with <i>Value</i> and <i>Exists</i>.</p></important> */
class ExpectedAttributeValue{
    AttributeValue value;
    bool exists;
    MinimalList<AttributeValue > attributeValueList;
    ComparisonOperator comparisonOperator;
    bool valueBeenSet;
    bool existsBeenSet;
    bool attributeValueListBeenSet;
    bool comparisonOperatorBeenSet;
    void reset();
public:
    ExpectedAttributeValue();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setValue(AttributeValue value);
    void setExists(bool exists);
    void setAttributeValueList(MinimalList<AttributeValue > attributeValueList);
    void setComparisonOperator(ComparisonOperator comparisonOperator);
    AttributeValue getValue() const;
    bool getExists() const;
    MinimalList<AttributeValue > getAttributeValueList() const;
    ComparisonOperator getComparisonOperator() const;
};

/* <p>Represents the selection criteria for a <i>Query</i> or <i>Scan</i> operation:</p><ul><li><p>For a <i>Query</i> operation, <i>Condition</i> is used for specifying the <i>KeyConditions</i> to use when querying a table or an index. For <i>KeyConditions</i>, only the following comparison operators are supported:</p><p><code>EQ | LE | LT | GE | GT | BEGINS_WITH | BETWEEN</code></p><p><i>Condition</i> is also used in a <i>QueryFilter</i>, which evaluates the query results and returns only the desired values.</p></li><li><p>For a <i>Scan</i> operation, <i>Condition</i> is used in a <i>ScanFilter</i>, which evaluates the scan results and returns only the desired values.</p></li></ul> */
class Condition{
    MinimalList<AttributeValue > attributeValueList;
    ComparisonOperator comparisonOperator;
    bool attributeValueListBeenSet;
    bool comparisonOperatorBeenSet;
    void reset();
public:
    Condition();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setAttributeValueList(MinimalList<AttributeValue > attributeValueList);
    void setComparisonOperator(ComparisonOperator comparisonOperator);
    MinimalList<AttributeValue > getAttributeValueList() const;
    ComparisonOperator getComparisonOperator() const;
};

/* <p>Represents an operation to perform - either <i>DeleteItem</i> or <i>PutItem</i>. You can only specify one of these operations, not both, in a single <i>WriteRequest</i>. If you do need to perform both of these operations, you will need to specify two separate <i>WriteRequest</i> objects.</p> */
class WriteRequest{
    DeleteRequest deleteRequest;
    PutRequest putRequest;
    bool deleteRequestBeenSet;
    bool putRequestBeenSet;
    void reset();
public:
    WriteRequest();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setDeleteRequest(DeleteRequest deleteRequest);
    void setPutRequest(PutRequest putRequest);
    DeleteRequest getDeleteRequest() const;
    PutRequest getPutRequest() const;
};

/* <p>Represents a set of primary keys and, for each key, the attributes to retrieve from the table.</p><p>For each primary key, you must provide <i>all</i> of the key attributes. For example, with a hash type primary key, you only need to specify the hash attribute. For a hash-and-range type primary key, you must specify <i>both</i> the hash attribute and the range attribute.</p> */
class KeysAndAttributes{
    bool consistentRead;
    MinimalList<MinimalString > attributesToGet;
    MinimalList<MinimalMap<AttributeValue > > keys;
    bool consistentReadBeenSet;
    bool attributesToGetBeenSet;
    bool keysBeenSet;
    void reset();
public:
    KeysAndAttributes();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setConsistentRead(bool consistentRead);
    void setAttributesToGet(MinimalList<MinimalString > attributesToGet);
    void setKeys(MinimalList<MinimalMap<AttributeValue > > keys);
    bool getConsistentRead() const;
    MinimalList<MinimalString > getAttributesToGet() const;
    MinimalList<MinimalMap<AttributeValue > > getKeys() const;
};

/* <p>Represents a local secondary index.</p> */
class LocalSecondaryIndex{
    Projection projection;
    MinimalList<KeySchemaElement > keySchema;
    MinimalString indexName;
    bool projectionBeenSet;
    bool keySchemaBeenSet;
    bool indexNameBeenSet;
    void reset();
public:
    LocalSecondaryIndex();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setProjection(Projection projection);
    void setKeySchema(MinimalList<KeySchemaElement > keySchema);
    void setIndexName(MinimalString indexName);
    Projection getProjection() const;
    MinimalList<KeySchemaElement > getKeySchema() const;
    MinimalString getIndexName() const;
};

/* <p>Represents a global secondary index.</p> */
class GlobalSecondaryIndex{
    Projection projection;
    ProvisionedThroughput provisionedThroughput;
    MinimalList<KeySchemaElement > keySchema;
    MinimalString indexName;
    bool projectionBeenSet;
    bool provisionedThroughputBeenSet;
    bool keySchemaBeenSet;
    bool indexNameBeenSet;
    void reset();
public:
    GlobalSecondaryIndex();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setProjection(Projection projection);
    void setProvisionedThroughput(ProvisionedThroughput provisionedThroughput);
    void setKeySchema(MinimalList<KeySchemaElement > keySchema);
    void setIndexName(MinimalString indexName);
    Projection getProjection() const;
    ProvisionedThroughput getProvisionedThroughput() const;
    MinimalList<KeySchemaElement > getKeySchema() const;
    MinimalString getIndexName() const;
};

/* <p>For the <i>UpdateItem</i> operation, represents the attributes to be modified, the action to perform on each, and the new value for each.</p><p>Attribute values cannot be null; string and binary type attributes must have lengths greater than zero; and set type attributes must not be empty. Requests with empty values will be rejected with a <i>ValidationException</i>.</p> */
class AttributeValueUpdate{
    AttributeValue value;
    AttributeAction action;
    bool valueBeenSet;
    bool actionBeenSet;
    void reset();
public:
    AttributeValueUpdate();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setValue(AttributeValue value);
    void setAction(AttributeAction action);
    AttributeValue getValue() const;
    AttributeAction getAction() const;
};

/* <p>Represents the new provisioned throughput settings to apply to a global secondary index.</p> */
class GlobalSecondaryIndexUpdate{
    UpdateGlobalSecondaryIndexAction update;
    bool updateBeenSet;
    void reset();
public:
    GlobalSecondaryIndexUpdate();
    bool jsonDeserialize(MinimalString json);
    MinimalString jsonSerialize() const;
    void setUpdate(UpdateGlobalSecondaryIndexAction update);
    UpdateGlobalSecondaryIndexAction getUpdate() const;
};

/* <p>Represents the output of a <i>ListTables</i> operation.</p> */
class ListTablesOutput{
    MinimalString lastEvaluatedTableName;
    MinimalList<MinimalString > tableNames;
    bool lastEvaluatedTableNameBeenSet;
    bool tableNamesBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    ListTablesOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setLastEvaluatedTableName(MinimalString lastEvaluatedTableName);
    void setTableNames(MinimalList<MinimalString > tableNames);
    MinimalString getLastEvaluatedTableName() const;
    MinimalList<MinimalString > getTableNames() const;
};

/* <p>Represents the output of an <i>UpdateTable</i> operation.</p> */
class UpdateTableOutput{
    TableDescription tableDescription;
    bool tableDescriptionBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    UpdateTableOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setTableDescription(TableDescription tableDescription);
    TableDescription getTableDescription() const;
};

/* <p>Represents the output of an <i>UpdateItem</i> operation.</p> */
class UpdateItemOutput{
    MinimalMap<AttributeValue > attributes;
    ItemCollectionMetrics itemCollectionMetrics;
    ConsumedCapacity consumedCapacity;
    bool attributesBeenSet;
    bool itemCollectionMetricsBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    UpdateItemOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setAttributes(MinimalMap<AttributeValue > attributes);
    void setItemCollectionMetrics(ItemCollectionMetrics itemCollectionMetrics);
    void setConsumedCapacity(ConsumedCapacity consumedCapacity);
    MinimalMap<AttributeValue > getAttributes() const;
    ItemCollectionMetrics getItemCollectionMetrics() const;
    ConsumedCapacity getConsumedCapacity() const;
};

/* <p>Represents the input of a <i>DeleteItem</i> operation.</p> */
class DeleteItemInput{
    ReturnItemCollectionMetrics returnItemCollectionMetrics;
    ReturnValue returnValues;
    MinimalMap<AttributeValue > key;
    ConditionalOperator conditionalOperator;
    MinimalMap<ExpectedAttributeValue > expected;
    MinimalString tableName;
    ReturnConsumedCapacity returnConsumedCapacity;
    bool returnItemCollectionMetricsBeenSet;
    bool returnValuesBeenSet;
    bool keyBeenSet;
    bool conditionalOperatorBeenSet;
    bool expectedBeenSet;
    bool tableNameBeenSet;
    bool returnConsumedCapacityBeenSet;
    void reset();
public:
    DeleteItemInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics);
    void setReturnValues(ReturnValue returnValues);
    void setKey(MinimalMap<AttributeValue > key);
    void setConditionalOperator(ConditionalOperator conditionalOperator);
    void setExpected(MinimalMap<ExpectedAttributeValue > expected);
    void setTableName(MinimalString tableName);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    ReturnItemCollectionMetrics getReturnItemCollectionMetrics() const;
    ReturnValue getReturnValues() const;
    MinimalMap<AttributeValue > getKey() const;
    ConditionalOperator getConditionalOperator() const;
    MinimalMap<ExpectedAttributeValue > getExpected() const;
    MinimalString getTableName() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
};

/* <p>Represents the input of a <i>Scan</i> operation.</p> */
class ScanInput{
    MinimalMap<Condition > scanFilter;
    Select select;
    int totalSegments;
    ConditionalOperator conditionalOperator;
    int segment;
    MinimalList<MinimalString > attributesToGet;
    MinimalString tableName;
    ReturnConsumedCapacity returnConsumedCapacity;
    int limit;
    MinimalMap<AttributeValue > exclusiveStartKey;
    bool scanFilterBeenSet;
    bool selectBeenSet;
    bool totalSegmentsBeenSet;
    bool conditionalOperatorBeenSet;
    bool segmentBeenSet;
    bool attributesToGetBeenSet;
    bool tableNameBeenSet;
    bool returnConsumedCapacityBeenSet;
    bool limitBeenSet;
    bool exclusiveStartKeyBeenSet;
    void reset();
public:
    ScanInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setScanFilter(MinimalMap<Condition > scanFilter);
    void setSelect(Select select);
    void setTotalSegments(int totalSegments);
    void setConditionalOperator(ConditionalOperator conditionalOperator);
    void setSegment(int segment);
    void setAttributesToGet(MinimalList<MinimalString > attributesToGet);
    void setTableName(MinimalString tableName);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    void setLimit(int limit);
    void setExclusiveStartKey(MinimalMap<AttributeValue > exclusiveStartKey);
    MinimalMap<Condition > getScanFilter() const;
    Select getSelect() const;
    int getTotalSegments() const;
    ConditionalOperator getConditionalOperator() const;
    int getSegment() const;
    MinimalList<MinimalString > getAttributesToGet() const;
    MinimalString getTableName() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
    int getLimit() const;
    MinimalMap<AttributeValue > getExclusiveStartKey() const;
};

/* <p>Represents the input of a <i>ListTables</i> operation.</p> */
class ListTablesInput{
    MinimalString exclusiveStartTableName;
    int limit;
    bool exclusiveStartTableNameBeenSet;
    bool limitBeenSet;
    void reset();
public:
    ListTablesInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setExclusiveStartTableName(MinimalString exclusiveStartTableName);
    void setLimit(int limit);
    MinimalString getExclusiveStartTableName() const;
    int getLimit() const;
};

/* <p>Represents the output of a <i>BatchWriteItem</i> operation.</p> */
class BatchWriteItemOutput{
    MinimalMap<MinimalList<WriteRequest > > unprocessedItems;
    MinimalMap<MinimalList<ItemCollectionMetrics > > itemCollectionMetrics;
    MinimalList<ConsumedCapacity > consumedCapacity;
    bool unprocessedItemsBeenSet;
    bool itemCollectionMetricsBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    BatchWriteItemOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setUnprocessedItems(MinimalMap<MinimalList<WriteRequest > > unprocessedItems);
    void setItemCollectionMetrics(MinimalMap<MinimalList<ItemCollectionMetrics > > itemCollectionMetrics);
    void setConsumedCapacity(MinimalList<ConsumedCapacity > consumedCapacity);
    MinimalMap<MinimalList<WriteRequest > > getUnprocessedItems() const;
    MinimalMap<MinimalList<ItemCollectionMetrics > > getItemCollectionMetrics() const;
    MinimalList<ConsumedCapacity > getConsumedCapacity() const;
};

/* <p>Represents the input of a <i>Query</i> operation.</p> */
class QueryInput{
    bool scanIndexForward;
    Select select;
    bool consistentRead;
    ConditionalOperator conditionalOperator;
    MinimalMap<Condition > queryFilter;
    MinimalList<MinimalString > attributesToGet;
    MinimalMap<Condition > keyConditions;
    MinimalString tableName;
    MinimalString indexName;
    ReturnConsumedCapacity returnConsumedCapacity;
    int limit;
    MinimalMap<AttributeValue > exclusiveStartKey;
    bool scanIndexForwardBeenSet;
    bool selectBeenSet;
    bool consistentReadBeenSet;
    bool conditionalOperatorBeenSet;
    bool queryFilterBeenSet;
    bool attributesToGetBeenSet;
    bool keyConditionsBeenSet;
    bool tableNameBeenSet;
    bool indexNameBeenSet;
    bool returnConsumedCapacityBeenSet;
    bool limitBeenSet;
    bool exclusiveStartKeyBeenSet;
    void reset();
public:
    QueryInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setScanIndexForward(bool scanIndexForward);
    void setSelect(Select select);
    void setConsistentRead(bool consistentRead);
    void setConditionalOperator(ConditionalOperator conditionalOperator);
    void setQueryFilter(MinimalMap<Condition > queryFilter);
    void setAttributesToGet(MinimalList<MinimalString > attributesToGet);
    void setKeyConditions(MinimalMap<Condition > keyConditions);
    void setTableName(MinimalString tableName);
    void setIndexName(MinimalString indexName);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    void setLimit(int limit);
    void setExclusiveStartKey(MinimalMap<AttributeValue > exclusiveStartKey);
    bool getScanIndexForward() const;
    Select getSelect() const;
    bool getConsistentRead() const;
    ConditionalOperator getConditionalOperator() const;
    MinimalMap<Condition > getQueryFilter() const;
    MinimalList<MinimalString > getAttributesToGet() const;
    MinimalMap<Condition > getKeyConditions() const;
    MinimalString getTableName() const;
    MinimalString getIndexName() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
    int getLimit() const;
    MinimalMap<AttributeValue > getExclusiveStartKey() const;
};

/* <p>Represents the input of a <i>BatchWriteItem</i> operation.</p> */
class BatchWriteItemInput{
    ReturnItemCollectionMetrics returnItemCollectionMetrics;
    MinimalMap<MinimalList<WriteRequest > > requestItems;
    ReturnConsumedCapacity returnConsumedCapacity;
    bool returnItemCollectionMetricsBeenSet;
    bool requestItemsBeenSet;
    bool returnConsumedCapacityBeenSet;
    void reset();
public:
    BatchWriteItemInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics);
    void setRequestItems(MinimalMap<MinimalList<WriteRequest > > requestItems);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    ReturnItemCollectionMetrics getReturnItemCollectionMetrics() const;
    MinimalMap<MinimalList<WriteRequest > > getRequestItems() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
};

/* <p>Represents the output of a <i>Query</i> operation.</p> */
class QueryOutput{
    MinimalMap<AttributeValue > lastEvaluatedKey;
    MinimalList<MinimalMap<AttributeValue > > items;
    int count;
    int scannedCount;
    ConsumedCapacity consumedCapacity;
    bool lastEvaluatedKeyBeenSet;
    bool itemsBeenSet;
    bool countBeenSet;
    bool scannedCountBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    QueryOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setLastEvaluatedKey(MinimalMap<AttributeValue > lastEvaluatedKey);
    void setItems(MinimalList<MinimalMap<AttributeValue > > items);
    void setCount(int count);
    void setScannedCount(int scannedCount);
    void setConsumedCapacity(ConsumedCapacity consumedCapacity);
    MinimalMap<AttributeValue > getLastEvaluatedKey() const;
    MinimalList<MinimalMap<AttributeValue > > getItems() const;
    int getCount() const;
    int getScannedCount() const;
    ConsumedCapacity getConsumedCapacity() const;
};

/* <p>Represents the output of a <i>DeleteItem</i> operation.</p> */
class DeleteItemOutput{
    MinimalMap<AttributeValue > attributes;
    ItemCollectionMetrics itemCollectionMetrics;
    ConsumedCapacity consumedCapacity;
    bool attributesBeenSet;
    bool itemCollectionMetricsBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    DeleteItemOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setAttributes(MinimalMap<AttributeValue > attributes);
    void setItemCollectionMetrics(ItemCollectionMetrics itemCollectionMetrics);
    void setConsumedCapacity(ConsumedCapacity consumedCapacity);
    MinimalMap<AttributeValue > getAttributes() const;
    ItemCollectionMetrics getItemCollectionMetrics() const;
    ConsumedCapacity getConsumedCapacity() const;
};

/* <p>Represents the output of a <i>BatchGetItem</i> operation.</p> */
class BatchGetItemOutput{
    MinimalMap<MinimalList<MinimalMap<AttributeValue > > > responses;
    MinimalMap<KeysAndAttributes > unprocessedKeys;
    MinimalList<ConsumedCapacity > consumedCapacity;
    bool responsesBeenSet;
    bool unprocessedKeysBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    BatchGetItemOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setResponses(MinimalMap<MinimalList<MinimalMap<AttributeValue > > > responses);
    void setUnprocessedKeys(MinimalMap<KeysAndAttributes > unprocessedKeys);
    void setConsumedCapacity(MinimalList<ConsumedCapacity > consumedCapacity);
    MinimalMap<MinimalList<MinimalMap<AttributeValue > > > getResponses() const;
    MinimalMap<KeysAndAttributes > getUnprocessedKeys() const;
    MinimalList<ConsumedCapacity > getConsumedCapacity() const;
};

/* <p>Represents the input of a <i>CreateTable</i> operation.</p> */
class CreateTableInput{
    ProvisionedThroughput provisionedThroughput;
    MinimalList<GlobalSecondaryIndex > globalSecondaryIndexes;
    MinimalString tableName;
    MinimalList<LocalSecondaryIndex > localSecondaryIndexes;
    MinimalList<KeySchemaElement > keySchema;
    MinimalList<AttributeDefinition > attributeDefinitions;
    bool provisionedThroughputBeenSet;
    bool globalSecondaryIndexesBeenSet;
    bool tableNameBeenSet;
    bool localSecondaryIndexesBeenSet;
    bool keySchemaBeenSet;
    bool attributeDefinitionsBeenSet;
    void reset();
public:
    CreateTableInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setProvisionedThroughput(ProvisionedThroughput provisionedThroughput);
    void setGlobalSecondaryIndexes(MinimalList<GlobalSecondaryIndex > globalSecondaryIndexes);
    void setTableName(MinimalString tableName);
    void setLocalSecondaryIndexes(MinimalList<LocalSecondaryIndex > localSecondaryIndexes);
    void setKeySchema(MinimalList<KeySchemaElement > keySchema);
    void setAttributeDefinitions(MinimalList<AttributeDefinition > attributeDefinitions);
    ProvisionedThroughput getProvisionedThroughput() const;
    MinimalList<GlobalSecondaryIndex > getGlobalSecondaryIndexes() const;
    MinimalString getTableName() const;
    MinimalList<LocalSecondaryIndex > getLocalSecondaryIndexes() const;
    MinimalList<KeySchemaElement > getKeySchema() const;
    MinimalList<AttributeDefinition > getAttributeDefinitions() const;
};

/* <p>Represents the input of a <i>DescribeTable</i> operation.</p> */
class DescribeTableInput{
    MinimalString tableName;
    bool tableNameBeenSet;
    void reset();
public:
    DescribeTableInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setTableName(MinimalString tableName);
    MinimalString getTableName() const;
};

/* <p>Represents the input of a <i>PutItem</i> operation.</p> */
class PutItemInput{
    ReturnItemCollectionMetrics returnItemCollectionMetrics;
    ReturnValue returnValues;
    MinimalMap<AttributeValue > item;
    ConditionalOperator conditionalOperator;
    MinimalMap<ExpectedAttributeValue > expected;
    MinimalString tableName;
    ReturnConsumedCapacity returnConsumedCapacity;
    bool returnItemCollectionMetricsBeenSet;
    bool returnValuesBeenSet;
    bool itemBeenSet;
    bool conditionalOperatorBeenSet;
    bool expectedBeenSet;
    bool tableNameBeenSet;
    bool returnConsumedCapacityBeenSet;
    void reset();
public:
    PutItemInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics);
    void setReturnValues(ReturnValue returnValues);
    void setItem(MinimalMap<AttributeValue > item);
    void setConditionalOperator(ConditionalOperator conditionalOperator);
    void setExpected(MinimalMap<ExpectedAttributeValue > expected);
    void setTableName(MinimalString tableName);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    ReturnItemCollectionMetrics getReturnItemCollectionMetrics() const;
    ReturnValue getReturnValues() const;
    MinimalMap<AttributeValue > getItem() const;
    ConditionalOperator getConditionalOperator() const;
    MinimalMap<ExpectedAttributeValue > getExpected() const;
    MinimalString getTableName() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
};

/* <p>Represents the output of a <i>GetItem</i> operation.</p> */
class GetItemOutput{
    MinimalMap<AttributeValue > item;
    ConsumedCapacity consumedCapacity;
    bool itemBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    GetItemOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setItem(MinimalMap<AttributeValue > item);
    void setConsumedCapacity(ConsumedCapacity consumedCapacity);
    MinimalMap<AttributeValue > getItem() const;
    ConsumedCapacity getConsumedCapacity() const;
};

/* <p>Represents the input of a <i>DeleteTable</i> operation.</p> */
class DeleteTableInput{
    MinimalString tableName;
    bool tableNameBeenSet;
    void reset();
public:
    DeleteTableInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setTableName(MinimalString tableName);
    MinimalString getTableName() const;
};

/* <p>Represents the output of a <i>DescribeTable</i> operation.</p> */
class DescribeTableOutput{
    TableDescription table;
    bool tableBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    DescribeTableOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setTable(TableDescription table);
    TableDescription getTable() const;
};

/* <p>Represents the output of a <i>Scan</i> operation.</p> */
class ScanOutput{
    MinimalMap<AttributeValue > lastEvaluatedKey;
    MinimalList<MinimalMap<AttributeValue > > items;
    int count;
    int scannedCount;
    ConsumedCapacity consumedCapacity;
    bool lastEvaluatedKeyBeenSet;
    bool itemsBeenSet;
    bool countBeenSet;
    bool scannedCountBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    ScanOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setLastEvaluatedKey(MinimalMap<AttributeValue > lastEvaluatedKey);
    void setItems(MinimalList<MinimalMap<AttributeValue > > items);
    void setCount(int count);
    void setScannedCount(int scannedCount);
    void setConsumedCapacity(ConsumedCapacity consumedCapacity);
    MinimalMap<AttributeValue > getLastEvaluatedKey() const;
    MinimalList<MinimalMap<AttributeValue > > getItems() const;
    int getCount() const;
    int getScannedCount() const;
    ConsumedCapacity getConsumedCapacity() const;
};

/* <p>Represents the input of a <i>BatchGetItem</i> operation.</p> */
class BatchGetItemInput{
    MinimalMap<KeysAndAttributes > requestItems;
    ReturnConsumedCapacity returnConsumedCapacity;
    bool requestItemsBeenSet;
    bool returnConsumedCapacityBeenSet;
    void reset();
public:
    BatchGetItemInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setRequestItems(MinimalMap<KeysAndAttributes > requestItems);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    MinimalMap<KeysAndAttributes > getRequestItems() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
};

/* <p>Represents the input of an <i>UpdateItem</i> operation.</p> */
class UpdateItemInput{
    ReturnItemCollectionMetrics returnItemCollectionMetrics;
    ReturnValue returnValues;
    MinimalMap<AttributeValue > key;
    ConditionalOperator conditionalOperator;
    MinimalMap<ExpectedAttributeValue > expected;
    MinimalString tableName;
    MinimalMap<AttributeValueUpdate > attributeUpdates;
    ReturnConsumedCapacity returnConsumedCapacity;
    bool returnItemCollectionMetricsBeenSet;
    bool returnValuesBeenSet;
    bool keyBeenSet;
    bool conditionalOperatorBeenSet;
    bool expectedBeenSet;
    bool tableNameBeenSet;
    bool attributeUpdatesBeenSet;
    bool returnConsumedCapacityBeenSet;
    void reset();
public:
    UpdateItemInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setReturnItemCollectionMetrics(ReturnItemCollectionMetrics returnItemCollectionMetrics);
    void setReturnValues(ReturnValue returnValues);
    void setKey(MinimalMap<AttributeValue > key);
    void setConditionalOperator(ConditionalOperator conditionalOperator);
    void setExpected(MinimalMap<ExpectedAttributeValue > expected);
    void setTableName(MinimalString tableName);
    void setAttributeUpdates(MinimalMap<AttributeValueUpdate > attributeUpdates);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    ReturnItemCollectionMetrics getReturnItemCollectionMetrics() const;
    ReturnValue getReturnValues() const;
    MinimalMap<AttributeValue > getKey() const;
    ConditionalOperator getConditionalOperator() const;
    MinimalMap<ExpectedAttributeValue > getExpected() const;
    MinimalString getTableName() const;
    MinimalMap<AttributeValueUpdate > getAttributeUpdates() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
};

/* <p>Represents the output of a <i>CreateTable</i> operation.</p> */
class CreateTableOutput{
    TableDescription tableDescription;
    bool tableDescriptionBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    CreateTableOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setTableDescription(TableDescription tableDescription);
    TableDescription getTableDescription() const;
};

/* <p>Represents the input of an <i>UpdateTable</i> operation.</p> */
class UpdateTableInput{
    ProvisionedThroughput provisionedThroughput;
    MinimalList<GlobalSecondaryIndexUpdate > globalSecondaryIndexUpdates;
    MinimalString tableName;
    bool provisionedThroughputBeenSet;
    bool globalSecondaryIndexUpdatesBeenSet;
    bool tableNameBeenSet;
    void reset();
public:
    UpdateTableInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setProvisionedThroughput(ProvisionedThroughput provisionedThroughput);
    void setGlobalSecondaryIndexUpdates(MinimalList<GlobalSecondaryIndexUpdate > globalSecondaryIndexUpdates);
    void setTableName(MinimalString tableName);
    ProvisionedThroughput getProvisionedThroughput() const;
    MinimalList<GlobalSecondaryIndexUpdate > getGlobalSecondaryIndexUpdates() const;
    MinimalString getTableName() const;
};

/* <p>Represents the input of a <i>GetItem</i> operation.</p> */
class GetItemInput{
    MinimalMap<AttributeValue > key;
    bool consistentRead;
    MinimalList<MinimalString > attributesToGet;
    MinimalString tableName;
    ReturnConsumedCapacity returnConsumedCapacity;
    bool keyBeenSet;
    bool consistentReadBeenSet;
    bool attributesToGetBeenSet;
    bool tableNameBeenSet;
    bool returnConsumedCapacityBeenSet;
    void reset();
public:
    GetItemInput();
    bool requiredAreSet() const;
    MinimalString jsonSerialize() const;
    void setKey(MinimalMap<AttributeValue > key);
    void setConsistentRead(bool consistentRead);
    void setAttributesToGet(MinimalList<MinimalString > attributesToGet);
    void setTableName(MinimalString tableName);
    void setReturnConsumedCapacity(ReturnConsumedCapacity returnConsumedCapacity);
    MinimalMap<AttributeValue > getKey() const;
    bool getConsistentRead() const;
    MinimalList<MinimalString > getAttributesToGet() const;
    MinimalString getTableName() const;
    ReturnConsumedCapacity getReturnConsumedCapacity() const;
};

/* <p>Represents the output of a <i>DeleteTable</i> operation.</p> */
class DeleteTableOutput{
    TableDescription tableDescription;
    bool tableDescriptionBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    DeleteTableOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setTableDescription(TableDescription tableDescription);
    TableDescription getTableDescription() const;
};

/* <p>Represents the output of a <i>PutItem</i> operation.</p> */
class PutItemOutput{
    MinimalMap<AttributeValue > attributes;
    ItemCollectionMetrics itemCollectionMetrics;
    ConsumedCapacity consumedCapacity;
    bool attributesBeenSet;
    bool itemCollectionMetricsBeenSet;
    bool consumedCapacityBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    PutItemOutput();
    bool jsonDeserialize(MinimalString json);
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setAttributes(MinimalMap<AttributeValue > attributes);
    void setItemCollectionMetrics(ItemCollectionMetrics itemCollectionMetrics);
    void setConsumedCapacity(ConsumedCapacity consumedCapacity);
    MinimalMap<AttributeValue > getAttributes() const;
    ItemCollectionMetrics getItemCollectionMetrics() const;
    ConsumedCapacity getConsumedCapacity() const;
};

class AmazonDynamoDBClient : public AWSClient {
public:
    AmazonDynamoDBClient();
    /* <p>The <i>BatchGetItem</i> operation returns the attributes of one or more items from one or more tables. You identify requested items by primary key.</p><p>A single operation can retrieve up to 1 MB of data, which can contain as many as 100 items. <i>BatchGetItem</i> will return a partial result if the response size limit is exceeded, the table's provisioned throughput is exceeded, or an internal processing failure occurs. If a partial result is returned, the operation returns a value for <i>UnprocessedKeys</i>. You can use this value to retry the operation starting with the next item to get.</p><p>For example, if you ask to retrieve 100 items, but each individual item is 50 KB in size, the system returns 20 items (1 MB) and an appropriate <i>UnprocessedKeys</i> value so you can get the next page of results. If desired, your application can include its own logic to assemble the pages of results into one dataset.</p><p>If <i>none</i> of the items can be processed due to insufficient provisioned throughput on all of the tables in the request, then <i>BatchGetItem</i> will throw a <i>ProvisionedThroughputExceededException</i>. If <i>at least one</i> of the items is successfully processed, then <i>BatchGetItem</i> completes successfully, while returning the keys of the unread items in <i>UnprocessedKeys</i>.</p><important><p>If DynamoDB returns any unprocessed items, you should retry the batch operation on those items. However, <i>we strongly recommend that you use an exponential backoff algorithm</i>. If you retry the batch operation immediately, the underlying read or write requests can still fail due to throttling on the individual tables. If you delay the batch operation using exponential backoff, the individual requests in the batch are much more likely to succeed.</p><p>For more information, go to <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/ErrorHandling.html#BatchOperations">Batch Operations and Error Handling</a> in the Amazon DynamoDB Developer Guide.</p></important><p>By default, <i>BatchGetItem</i> performs eventually consistent reads on every table in the request. If you want strongly consistent reads instead, you can set <i>ConsistentRead</i> to <code>true</code> for any or all tables.</p><p>In order to minimize response latency, <i>BatchGetItem</i> retrieves items in parallel.</p><p>When designing your application, keep in mind that DynamoDB does not return attributes in any particular order. To help parse the response by item, include the primary key values for the items in your request in the <i>AttributesToGet</i> parameter.</p><p>If a requested item does not exist, it is not returned in the result. Requests for nonexistent items consume the minimum read capacity units according to the type of read. For more information, see <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/WorkingWithTables.html#CapacityUnitCalculations">Capacity Units Calculations</a> in the Amazon DynamoDB Developer Guide.</p> */
    BatchGetItemOutput batchGetItem(BatchGetItemInput batchGetItemInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>The <i>BatchWriteItem</i> operation puts or deletes multiple items in one or more tables. A single call to <i>BatchWriteItem</i> can write up to 1 MB of data, which can comprise as many as 25 put or delete requests. Individual items to be written can be as large as 64 KB.</p><p>The individual <i>PutItem</i> and <i>DeleteItem</i> operations specified in <i>BatchWriteItem</i> are atomic; however <i>BatchWriteItem</i> as a whole is not. If any requested operations fail because the table's provisioned throughput is exceeded or an internal processing failure occurs, the failed operations are returned in the <i>UnprocessedItems</i> response parameter. You can investigate and optionally resend the requests. Typically, you would call <i>BatchWriteItem</i> in a loop. Each iteration would check for unprocessed items and submit a new <i>BatchWriteItem</i> request with those unprocessed items until all items have been processed.</p><p>Note that if <i>none</i> of the items can be processed due to insufficient provisioned throughput on all of the tables in the request, then <i>BatchGetItem</i> will throw a <i>ProvisionedThroughputExceededException</i>.</p><important><p>If DynamoDB returns any unprocessed items, you should retry the batch operation on those items. However, <i>we strongly recommend that you use an exponential backoff algorithm</i>. If you retry the batch operation immediately, the underlying read or write requests can still fail due to throttling on the individual tables. If you delay the batch operation using exponential backoff, the individual requests in the batch are much more likely to succeed.</p><p>For more information, go to <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/ErrorHandling.html#BatchOperations">Batch Operations and Error Handling</a> in the Amazon DynamoDB Developer Guide.</p></important><p>With <i>BatchWriteItem</i>, you can efficiently write or delete large amounts of data, such as from Amazon Elastic MapReduce (EMR), or copy data from another database into DynamoDB. In order to improve performance with these large-scale operations, <i>BatchWriteItem</i> does not behave in the same way as individual <i>PutItem</i> and <i>DeleteItem</i> calls would For example, you cannot specify conditions on individual put and delete requests, and <i>BatchWriteItem</i> does not return deleted items in the response.</p><p>If you use a programming language that supports concurrency, such as Java, you can use threads to write items in parallel. Your application must include the necessary logic to manage the threads. With languages that don't support threading, such as PHP, you must update or delete the specified items one at a time. In both situations, <i>BatchWriteItem</i> provides an alternative where the API performs the specified put and delete operations in parallel, giving you the power of the thread pool approach without having to introduce complexity into your application.</p><p>Parallel processing reduces latency, but each specified put and delete request consumes the same number of write capacity units whether it is processed in parallel or not. Delete operations on nonexistent items consume one write capacity unit.</p><p>If one or more of the following is true, DynamoDB rejects the entire batch write operation:</p><ul><li><p>One or more tables specified in the <i>BatchWriteItem</i> request does not exist.</p></li><li><p>Primary key attributes specified on an item in the request do not match those in the corresponding table's primary key schema.</p></li><li><p>You try to perform multiple operations on the same item in the same <i>BatchWriteItem</i> request. For example, you cannot put and delete the same item in the same <i>BatchWriteItem</i> request. </p></li><li><p>The total request size exceeds 1 MB.</p></li><li><p>Any individual item in a batch exceeds 64 KB.</p></li></ul> */
    BatchWriteItemOutput batchWriteItem(BatchWriteItemInput batchWriteItemInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>The <i>CreateTable</i> operation adds a new table to your account. In an AWS account, table names must be unique within each region. That is, you can have two tables with same name if you create the tables in different regions.</p><p><i>CreateTable</i> is an asynchronous operation. Upon receiving a <i>CreateTable</i> request, DynamoDB immediately returns a response with a <i>TableStatus</i> of <code>CREATING</code>. After the table is created, DynamoDB sets the <i>TableStatus</i> to <code>ACTIVE</code>. You can perform read and write operations only on an <code>ACTIVE</code> table. </p><p>If you want to create multiple tables with secondary indexes on them, you must create them sequentially. Only one table with secondary indexes can be in the <code>CREATING</code> state at any given time.</p><p>You can use the <i>DescribeTable</i> API to check the table status.</p> */
    CreateTableOutput createTable(CreateTableInput createTableInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>Deletes a single item in a table by primary key. You can perform a conditional delete operation that deletes the item if it exists, or if it has an expected attribute value.</p><p>In addition to deleting an item, you can also return the item's attribute values in the same operation, using the <i>ReturnValues</i> parameter.</p><p>Unless you specify conditions, the <i>DeleteItem</i> is an idempotent operation; running it multiple times on the same item or attribute does <i>not</i> result in an error response.</p><p>Conditional deletes are useful for only deleting items if specific conditions are met. If those conditions are met, DynamoDB performs the delete. Otherwise, the item is not deleted. </p> */
    DeleteItemOutput deleteItem(DeleteItemInput deleteItemInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>The <i>DeleteTable</i> operation deletes a table and all of its items. After a <i>DeleteTable</i> request, the specified table is in the <code>DELETING</code> state until DynamoDB completes the deletion. If the table is in the <code>ACTIVE</code> state, you can delete it. If a table is in <code>CREATING</code> or <code>UPDATING</code> states, then DynamoDB returns a <i>ResourceInUseException</i>. If the specified table does not exist, DynamoDB returns a <i>ResourceNotFoundException</i>. If table is already in the <code>DELETING</code> state, no error is returned. </p><p>When you delete a table, any indexes on that table are also deleted.</p><p>Use the <i>DescribeTable</i> API to check the status of the table. </p> */
    DeleteTableOutput deleteTable(DeleteTableInput deleteTableInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>Returns information about the table, including the current status of the table, when it was created, the primary key schema, and any indexes on the table.</p> */
    DescribeTableOutput describeTable(DescribeTableInput describeTableInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>The <i>GetItem</i> operation returns a set of attributes for the item with the given primary key. If there is no matching item, <i>GetItem</i> does not return any data.</p><p><i>GetItem</i> provides an eventually consistent read by default. If your application requires a strongly consistent read, set <i>ConsistentRead</i> to <code>true</code>. Although a strongly consistent read might take more time than an eventually consistent read, it always returns the last updated value.</p> */
    GetItemOutput getItem(GetItemInput getItemInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>Returns an array of table names associated with the current account and endpoint. The output from <i>ListTables</i> is paginated, with each page returning a maximum of 100 table names.</p> */
    ListTablesOutput listTables(ListTablesInput listTablesInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>Creates a new item, or replaces an old item with a new item. If an item already exists in the specified table with the same primary key, the new item completely replaces the existing item. You can perform a conditional put (insert a new item if one with the specified primary key doesn't exist), or replace an existing item if it has certain attribute values. </p><p>In addition to putting an item, you can also return the item's attribute values in the same operation, using the <i>ReturnValues</i> parameter.</p><p>When you add an item, the primary key attribute(s) are the only required attributes. Attribute values cannot be null. String and binary type attributes must have lengths greater than zero. Set type attributes cannot be empty. Requests with empty values will be rejected with a <i>ValidationException</i>.</p><p>You can request that <i>PutItem</i> return either a copy of the old item (before the update) or a copy of the new item (after the update). For more information, see the <i>ReturnValues</i> description.</p><p>For more information about using this API, see <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/WorkingWithItems.html">Working with Items</a> in the Amazon DynamoDB Developer Guide.</p> */
    PutItemOutput putItem(PutItemInput putItemInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>A <i>Query</i> operation directly accesses items from a table using the table primary key, or from an index using the index key. You must provide a specific hash key value. You can narrow the scope of the query by using comparison operators on the range key value, or on the index key. You can use the <i>ScanIndexForward</i> parameter to get results in forward or reverse order, by range key or by index key. </p><p>Queries that do not return results consume the minimum read capacity units according to the type of read.</p><p>If the total number of items meeting the query criteria exceeds the result set size limit of 1 MB, the query stops and results are returned to the user with a <i>LastEvaluatedKey</i> to continue the query in a subsequent operation. Unlike a <i>Scan</i> operation, a <i>Query</i> operation never returns an empty result set <i>and</i> a <i>LastEvaluatedKey</i>. The <i>LastEvaluatedKey</i> is only provided if the results exceed 1 MB, or if you have used <i>Limit</i>. </p><p>You can query a table, a local secondary index, or a global secondary index. For a query on a table or on a local secondary index, you can set <i>ConsistentRead</i> to true and obtain a strongly consistent result. Global secondary indexes support eventually consistent reads only, so do not specify <i>ConsistentRead</i> when querying a global secondary index.</p> */
    QueryOutput query(QueryInput queryInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>The <i>Scan</i> operation returns one or more items and item attributes by accessing every item in the table. To have DynamoDB return fewer items, you can provide a <i>ScanFilter</i>.</p><p>If the total number of scanned items exceeds the maximum data set size limit of 1 MB, the scan stops and results are returned to the user with a <i>LastEvaluatedKey</i> to continue the scan in a subsequent operation. The results also include the number of items exceeding the limit. A scan can result in no table data meeting the filter criteria. </p><p>The result set is eventually consistent. </p><p>By default, <i>Scan</i> operations proceed sequentially; however, for faster performance on large tables, applications can request a parallel <i>Scan</i> by specifying the <i>Segment</i> and <i>TotalSegments</i> parameters. For more information, see <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/QueryAndScan.html#QueryAndScanParallelScan">Parallel Scan</a> in the Amazon DynamoDB Developer Guide.</p> */
    ScanOutput scan(ScanInput scanInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p> Edits an existing item's attributes, or inserts a new item if it does not already exist. You can put, delete, or add attribute values. You can also perform a conditional update (insert a new attribute name-value pair if it doesn't exist, or replace an existing name-value pair if it has certain expected attribute values).</p><p>In addition to updating an item, you can also return the item's attribute values in the same operation, using the <i>ReturnValues</i> parameter.</p> */
    UpdateItemOutput updateItem(UpdateItemInput updateItemInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
    /* <p>Updates the provisioned throughput for the given table. Setting the throughput for a table helps you manage performance and is part of the provisioned throughput feature of DynamoDB.</p><p>The provisioned throughput values can be upgraded or downgraded based on the maximums and minimums listed in the <a href="http://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Limits.html">Limits</a> section in the Amazon DynamoDB Developer Guide.</p><p>The table must be in the <code>ACTIVE</code> state for this operation to succeed. <i>UpdateTable</i> is an asynchronous operation; while executing the operation, the table is in the <code>UPDATING</code> state. While the table is in the <code>UPDATING</code> state, the table still has the provisioned throughput from before the call. The new provisioned throughput setting is in effect only when the table returns to the <code>ACTIVE</code> state after the <i>UpdateTable</i> operation. </p><p>You cannot add, modify or delete indexes using <i>UpdateTable</i>. Indexes can only be defined at table creation time.</p> */
    UpdateTableOutput updateTable(UpdateTableInput updateTableInput, ActionError& actionError, bool retry = true, int* httpStatusCode = NULL);
};

#endif /* AMAZONDYNAMODBCLIENT_H_ */
