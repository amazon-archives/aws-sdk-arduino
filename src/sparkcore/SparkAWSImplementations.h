#ifndef AWSSPARKIMPLEMENTATIONS_H_
#define AWSSPARKIMPLEMENTATIONS_H_
#include "DeviceIndependentInterfaces.h"
/* application.h is Spark's standard library. Define TCPClient. */
#include <application.h>

/* HttpClient implementation to be used on the Spark Core device. */
class SparkHttpClient: public IHttpClient {
    TCPClient client;
public:
    SparkHttpClient();
    /* Send http request and return the response. */
    char* send(const char *request, const char* serverUrl, int port);
    /* Returns false. Client uses raw http/https. */
    bool usesCurl(void);
};

class SparkDateTimeProvider: public IDateTimeProvider {
    /* The time as a cstring in yyyyMMddHHmmss format. Is written to within and
     * returned by getDateTime(). */
    char dateTime[15];
public:
    SparkDateTimeProvider();
    /* Retrieve the current GMT date and time in yyyyMMddHHmmss format. */
    const char* getDateTime(void);
    /* Returns false because Spark has it's own mechanism for syncing that does
     * not require an argument. */
    bool syncTakesArg(void);
    /* Synchronizes Spark's date and time with Spark's servers. The dateTime
     * argument is ignored. */
    void sync(const char* dateTime);
};

#endif /* AWSSPARKIMPLEMENTATIONS_H_ */
