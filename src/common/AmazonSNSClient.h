#ifndef AMAZONSNSCLIENT_H_
#define AMAZONSNSCLIENT_H_
#include "AWSFoundationalTypes.h"
#include "AWSClient2.h"

class PublishInput {
    MinimalString targetArn;
    MinimalString message;
    bool targetArnBeenSet;
    bool messageBeenSet;
    void reset();
public:
    PublishInput();
    bool requiredAreSet() const;
    void setMessage(MinimalString message);
    void setTargetArn(MinimalString targetArn);
    MinimalString getMessage() const;
    MinimalString getTargetArn() const;
    MinimalString serialize() const;
};

class PublishOutput {
    MinimalString messageId;
    bool messageIdBeenSet;
    MinimalString errorType;
    MinimalString errorMessage;
    void reset();
public:
    PublishOutput();
    MinimalString getErrorType() const;
    MinimalString getErrorMessage() const;
    void setMessageId(MinimalString messageId);
    MinimalString getMessageId() const;
};

class AmazonSNSClient : public AWSClient2 {
public:
    AmazonSNSClient();
    PublishOutput publish(PublishInput input, ActionError& actionError);
};

#endif /* AMAZONSNSCLIENT_H_ */
