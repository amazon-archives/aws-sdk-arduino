#include "DeviceIndependentInterfaces.h"
#include "EdisonAWSImplementations.h"
/* Arduino.h is Arduino's standard library. Defines the Arduino String
 * object and the Arduino delay() procedure. */
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

void printWifiStatus() {

	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");

}


bool Edison_Wifi_Setup(char* pSSID, char* pPassword) {

	int tempStatus = WL_IDLE_STATUS;
	bool ret_val = true;
	enum {
		SECS_IN_MS = 7000
	};
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("WiFi shield not present");
		ret_val = false;
	}

	String fv = WiFi.firmwareVersion();
	if (fv != "1.1.0") {
		Serial.println("Please upgrade the firmware");
		ret_val = false;
	}
	while (tempStatus != WL_CONNECTED) {
		Serial.print("Attempting to connect to SSID: ");
		Serial.println(pSSID);
		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		tempStatus = WiFi.begin(pSSID, pPassword);
		// wait 10 seconds for connection:
		delay(SECS_IN_MS);
	}

	return ret_val;
}


int delayTime = 500;

EdisonHttpClient::EdisonHttpClient() {
}

char* EdisonHttpClient::send(const char* request, const char* serverUrl,
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

bool EdisonHttpClient::usesCurl() {
    /* Does not use curl command. */
    return false;
}

EdisonHttpCurlClient::EdisonHttpCurlClient() {
}

char* EdisonHttpCurlClient::send(const char* request, const char* serverUrl,
        int port) {
    // call curl as a system call, passing 'request' as the system call command
    //system (request + " >/dev/ttyGS0");
    char* buffer = new char[1000]();
    sprintf(buffer, "%s %s", request, ">/dev/ttyGS0");
    char* response = new char[2 + 1]();
    buffer[1000] = '\0';
    system (buffer);
    return response;
}

bool EdisonHttpCurlClient::usesCurl() {
    /* Use curl command. */
    return true;
}


EdisonDateTimeProvider::EdisonDateTimeProvider() {
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

const char* EdisonDateTimeProvider::getDateTime() {
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

bool EdisonDateTimeProvider::syncTakesArg(void) {
    return true;
}

void EdisonDateTimeProvider::sync(const char* dateTime) {
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
