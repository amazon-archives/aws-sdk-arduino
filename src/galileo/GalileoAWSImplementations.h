#ifndef GALILEOAWSIMPLEMENTATIONS_H_
#define GALILEOAWSIMPLEMENTATIONS_H_
/* The EthernetClient is defined in this header. */
#include <Ethernet.h>
#include "DeviceIndependentInterfaces.h"

/* HttpClient implementation to be used on the Galileo device. */
class GalileoHttpClient: public IHttpClient {
    EthernetClient client;
public:
    GalileoHttpClient();
    /* Send http request and return the response. */
    char* send(const char *request, const char* serverUrl, int port);
    /* Returns false. Client uses raw http/https. */
    bool usesCurl(void);
};

/* IDateTimeProvider implementation to be used on the Galileo device. */
class GalileoDateTimeProvider: public IDateTimeProvider {
    /* These values set as default in constructor and updated in sync() */
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    /* The time in seconds that the GalileoDateTimeProvider was last synced. */
    unsigned long syncedTime;
    /* The time as a cstring in yyyyMMddHHmmss format. Is written to within and
     * returned by getDateTime(). */
    char dateTime[15];
public:
    GalileoDateTimeProvider();
    /* Retrieve the current GMT date and time in yyyyMMddHHmmss format. On
     * first call this will return a default date. sync() must then be called
     * with the current date. */
    const char* getDateTime(void);
    /* Returns true. Galileo requires an argument for sync because the device
     * does not intenally handle current time. */
    bool syncTakesArg(void);
    /* Synchronizes the GalileoDateTimeProvider's time given the current time
     * in yyyyMMddHHmmss format. */
    void sync(const char* dateTime);
};

#endif /* GALILEOAWSIMPLEMENTATIONS_H_ */
