#ifndef AMAZONS3CLIENT_H_
#define AMAZONS3CLIENT_H_
#include "AWSClient4.h"

class AmazonS3Client : public AWSClient4 {
public:
    AmazonS3Client();
    char* get(MinimalString uri, ActionError& actionError);
};

#endif /* AMAZONS3CLIENT_H_ */
