#ifndef AWSESP2866IMPLEMENTATIONS_H_
#define AWSESP2866IMPLEMENTATIONS_H_
#include "DeviceIndependentInterfaces.h"
/* application.h is Esp8266's standard library. Define TCPClient. */
#include <ESP8266WiFi.h>

/* HttpClient implementation to be used on the Esp8266 Core device. */
class Esp8266HttpClient: public IHttpClient {
    WiFiClientSecure sclient;
    //TCPClient client;
public:
    Esp8266HttpClient();
    /* Send http request and return the response. */
    char* send(const char *request, const char* serverUrl, int port);
    /* Returns false. Client uses raw http/https. */
    bool usesCurl(void);
};

class Esp8266DateTimeProvider: public IDateTimeProvider {
    /* The time as a cstring in yyyyMMddHHmmss format. Is written to within and
     * returned by getDateTime(). */
    WiFiClient client;
    //char dateTime[15];
public:
    char dateTime[15];
    Esp8266DateTimeProvider();
    /* Retrieve the current GMT date and time in yyyyMMddHHmmss format. */
    const char* getDateTime(void);
    /* Returns false because Esp8266 has it's own mechanism for syncing that does
     * not require an argument. */
    bool syncTakesArg(void);
    /* Synchronizes Esp8266's date and time with Esp8266's servers. The dateTime
     * argument is ignored. */
    void sync(const char* dateTime);
};

#endif /* AWSESP2866IMPLEMENTATIONS_H_ */
