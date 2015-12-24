#ifndef AMAZONIOTCLIENT_H_
#define AMAZONIOTCLIENT_H_
#include "AWSClient2.h"


// class Shadow {
//     MinimalString shadow;
//     void reset();
// public:
//     void setShadow(shadow) const;
// };



class AmazonIOTClient : public AWSClient2 {
public:
    AmazonIOTClient();

    char* update_shadow(MinimalString url, MinimalString shadow, ActionError& actionError);
};

#endif /* AMAZONSNSCLIENT_H_ */
