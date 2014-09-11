#include <stdio.h>
/* application.h is Spark's standard library. Defines the Arduino String
 * object, the Arduino delay() procedure, and the Spark TCPClient. */
#include <application.h>
#include "SparkAWSImplementations.h"
#include "DeviceIndependentInterfaces.h"

int delayTime = 500;

SparkHttpClient::SparkHttpClient() {
}

char* SparkHttpClient::send(const char* request, const char* serverUrl,
        int port) {
    /* Arduino String to build the response with. */
    String responseBuilder = "";
    if (client.connect(serverUrl, port)) {
        /* Send the requests */
        client.println(request);
        client.println();
        /* Read the request into responseBuilder. */
        delay(delayTime);
        while (client.available()) {
            char c = client.read();
            responseBuilder.concat(c);
        }
        client.stop();
    } else {
        client.stop();
        /* Error connecting. */
        return 0;
    }
    /* Copy responseBuilder into char* */
    int len = responseBuilder.length();
    char* response = new char[len + 1]();
    responseBuilder.toCharArray(response, len + 1);
    return response;
    return 0;
}

bool SparkHttpClient::usesCurl() {
    /* Does not use curl command. */
    return false;
}

SparkDateTimeProvider::SparkDateTimeProvider() {
    /* No need to sync, spark sychronizes time on startup. */
}

const char* SparkDateTimeProvider::getDateTime() {
    /* Spark provides the current time, so all we have to do is format it. */
    sprintf(dateTime, "%.4d%.2d%.2d%.2d%.2d%.2d", Time.year(), Time.month(),
            Time.day(), Time.hour(), Time.minute(), Time.second());
    return dateTime;
}

bool SparkDateTimeProvider::syncTakesArg(void) {
    return false;
}

void SparkDateTimeProvider::sync(const char* dateTime) {
    /* Use Spark's servers to synchronize current time. */
    Spark.syncTime();
}
