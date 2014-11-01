#ifndef AWSDEVICEINDEPENDENTINTERFACES_H_
#define AWSDEVICEINDEPENDENTINTERFACES_H_

/* HTTPClient Interface. Wraps the functionality of the existing TCPClient, and
 * EthernetClient for Spark Core and Intel Galileo Respectively. */
class IHttpClient {
public:
    virtual ~IHttpClient();
    /* Send http request and return the response. */
    virtual char* send(const char *request, const char* serverUrl,
            int port) = 0;
    /* Returns true if the client uses a curl command, false if the client uses
     * raw http/https. */
    virtual bool usesCurl(void) = 0;
};

/* Interface for setting and retrieving the current time. Required for sigv4
 * signing. */
class IDateTimeProvider {
public:
    virtual ~IDateTimeProvider();
    /* Retrieve the current GMT date and time in yyyyMMddHHmmss format. */
    virtual const char* getDateTime(void) = 0;
    /* Return true if the sync function requires the current time as in
     * argument. */
    virtual bool syncTakesArg(void) = 0;
    /* Called if AWS Service reports in accurate time. Sets the provider to
     * current time. If syncTakesArg() returns true, this argument takes the
     * current GMT date and time in yyyyMMddHHmmss format. Else, the nput value
     * is ignored and may be null. */
    virtual void sync(const char* dateTime) = 0;
};

#endif /* AWSDEVICEINDEPENDENTINTERFACES_H_ */
