#ifndef AMAZONIOTCLIENT_H_
#define AMAZONIOTCLIENT_H_
#include "AWSClient4.h"


// class Shadow {
//     MinimalString shadow;
//     void reset();
// public:
//     void setShadow(shadow) const;
// };



class AmazonIOTClient : public AWSClient4 {
public:
    AmazonIOTClient();

    char* update_shadow(MinimalString shadow, ActionError& actionError);
};

#endif /* AMAZONIOTCLIENT_H_ */
