#ifndef MTKAWSIMPLEMENTATIONS_H_
#define MTKAWSIMPLEMENTATIONS_H_
/* The EthernetClient is defined in this header. */
#include <arduino.h>
#include <LWiFi.h>
#include <LWiFiClient.h>

#include "DeviceIndependentInterfaces.h"

/* HttpClient implementation to be used on the Mtk device. */
class MtkHttpClient: public IHttpClient {
    LWiFiClient client;
public:
    MtkHttpClient();
    /* Send http request and return the response. */
    char* send(const char *request, const char* serverUrl, int port);
    /* Returns false. Client uses raw http/https. */
    bool usesCurl(void);
protected:
    char* sendHTTP(const char *request, const char* serverUrl, int port);
    char* sendHTTPS(const char *request, const char* serverUrl, int port);
};

/* Experimental HttpClient for Mtk that takes a curl command. This class is needed for
 * testing purposes */
class MtkHttpCurlClient: public IHttpClient {
public:
    MtkHttpCurlClient();
    /* Send http request and return the response.*/
    char* send(const char *request, const char* serverUrl, int port);
    /* Returns true. Client uses a curl command. */
    bool usesCurl(void);
};

/* IDateTimeProvider implementation to be used on the Mtk device. */
class MtkDateTimeProvider: public IDateTimeProvider {
    /* These values set as default in constructor and updated in sync() */
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    /* The time in seconds that the MtkDateTimeProvider was last synced. */
    unsigned long syncedTime;
    /* The time as a cstring in yyyyMMddHHmmss format. Is written to within and
     * returned by getDateTime(). */
    char dateTime[15];
public:
    MtkDateTimeProvider();
    /* Retrieve the current GMT date and time in yyyyMMddHHmmss format. On
     * first call this will return a default date. sync() must then be called
     * with the current date. */
    const char* getDateTime(void);
    /* Returns true. Mtk requires an argument for sync because the device
     * does not intenally handle current time. */
    bool syncTakesArg(void);
    /* Synchronizes the MtkDateTimeProvider's time given the current time
     * in yyyyMMddHHmmss format. */
    void sync(const char* dateTime);
};

void Mtk_Wifi_Setup(const char* pSSID, const char* pPassword);
void printWifiStatus();

#endif /* MTKAWSIMPLEMENTATIONS_H_ */
