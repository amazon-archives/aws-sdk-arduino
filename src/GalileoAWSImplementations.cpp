#include "DeviceIndependentInterfaces.h"
#include "GalileoAWSImplementations.h"
/* Arduino.h is Arduino's standard library. Defines the Arduino String
 * object and the Arduino delay() procedure. */
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

int delayTime = 500;

GalileoHttpClient::GalileoHttpClient() {
}

char* GalileoHttpClient::send(const char* request, const char* serverUrl,
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
}

bool GalileoHttpClient::usesCurl() {
    /* Does not use curl command. */
    return false;
}

GalileoDateTimeProvider::GalileoDateTimeProvider() {
    /* These are initialized to defaults. The defaults will cause the request
     * to fail, but the AWS Services response will contain the correct time
     * which will be fed to sync(). */
    year = 2000;
    month = 1;
    day = 1;
    hour = 0;
    minute = 0;
    second = 0;
    /* Never synced so syncTime is 0. */
    syncedTime = 0;
}

const char* GalileoDateTimeProvider::getDateTime() {
    const int secondsPerMinute = 60;
    const int secondsPerHour = secondsPerMinute * 60;
    const long int secondsPerDay = secondsPerHour * 24;
    /* Determine how many seconds have passed since the last sync, break those
     * seconds down into days/hours/minutes/seconds, then form the new time
     * string. */
    long int newSecond = ((millis() - syncedTime) / 1000) + second
            + (secondsPerMinute * minute) + (secondsPerHour * hour)
            + (secondsPerDay * day);
    int newDay = newSecond / secondsPerDay;
    newSecond = newSecond % secondsPerDay;
    int newHour = newSecond / secondsPerHour;
    newSecond = newSecond % secondsPerHour;
    int newMinute = newSecond / secondsPerMinute;
    newSecond = newSecond % secondsPerMinute;

    /* Without cumbersome data about how many days a month we cannot factor the
     * extra time into more months. AWS Services give different errors that do
     * not provide us with the current time if the date is invalid rather than
     * innacurate. However, going past the number of days in a month (i.e
     * giving 30 for February) is valid, so long as the days does not exceed
     *  31. */
    newDay = newDay > 31 ? 31 : newDay;

    /* Copy the values to dateTime and return it. */
    sprintf(dateTime, "%.4d%.2d%.2d%.2d%.2d%.2d", year, month, newDay, newHour,
            newMinute, (int) newSecond);
    return dateTime;
}

bool GalileoDateTimeProvider::syncTakesArg(void) {
    return true;
}

void GalileoDateTimeProvider::sync(const char* dateTime) {
    /* If given a null dateTime, do nothing. */
    if (dateTime) {
        /* Set synced time to the number of milliseconds since the sketch
         * started. */
        syncedTime = millis();
        /* Copy each substring into the buffer then convert it to an integer. */
        char buffer[5];
        sprintf(buffer, "%.4s", dateTime);
        year = atoi(buffer);
        sprintf(buffer, "%.2s", dateTime + 4);
        month = atoi(buffer);
        sprintf(buffer, "%.2s", dateTime + 6);
        day = atoi(buffer);
        sprintf(buffer, "%.2s", dateTime + 8);
        hour = atoi(buffer);
        sprintf(buffer, "%.2s", dateTime + 10);
        minute = atoi(buffer);
        sprintf(buffer, "%.2s", dateTime + 12);
        second = atoi(buffer);

        /* If these values come out as 0, change them to valid AWS date values
         * (i.e. cannot have month 0). */
        year = year ? year : 2000;
        month = month ? month : 1;
        day = day ? day : 1;
    }
}
