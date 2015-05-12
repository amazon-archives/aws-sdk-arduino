#include "DeviceIndependentInterfaces.h"
#include "MtkAWSImplementations.h"
#include "vmssl.h"
#include "LTask.h"
/* Arduino.h is Arduino's standard library. Defines the Arduino String
 * object and the Arduino delay() procedure. */
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

int delayTime = 500;

MtkHttpClient::MtkHttpClient() {
}

struct MtkHttpContext
{
    const char *request;
    const char *serverUrl;
    VMINT port;
    String response;
    VMINT data_sent;
    VMINT data_read;
};
MtkHttpContext *pContext;

void sslCb(VMINT handle, VMINT event) {
    
    // This callback is invoked in LinkIt main thread
    
    VMINT ret;
    VMCHAR buf[52] = {0,};
    
#define Serial Serial1
    Serial.print("sslCb event=");
    Serial.println(event);
    
    switch(event) {
        case VM_SSL_EVT_CONNECTED:
        case VM_SSL_EVT_CAN_WRITE:
        {
            const size_t requestLength = strlen(pContext->request);
            
            ret = vm_ssl_write(handle, (VMUINT8*)pContext->request + pContext->data_sent, requestLength);
            if(ret >= 0) {
                pContext->data_sent += ret;
            }
            break;
        }
        case VM_SSL_EVT_CAN_READ:
            // make sure there is an terminating NULL
            ret = vm_ssl_read(handle, (VMUINT8*)buf, sizeof(buf) - 1);
            while(ret > 0) {
                pContext->response.concat(buf);
                memset(buf, 0, sizeof(buf));
                // make sure there is an terminating NULL
                ret = vm_ssl_read(handle, (VMUINT8*)buf, sizeof(buf) - 1);
                pContext->data_read += ret;
            }
            
            if(ret == VM_TCP_READ_EOF) {
                vm_ssl_close(handle);
                
                // Allow LTask.remoteCall() to return
                LTask.post_signal();
            }
            break;
        case VM_SSL_EVT_PIPE_BROKEN:
        case VM_SSL_EVT_HOST_NOT_FOUND:
        case VM_SSL_EVT_PIPE_CLOSED:
        case VM_SSL_EVT_HANDSHAKE_FAILED:
        case VM_SSL_EVT_CERTIFICATE_VALIDATION_FAILED:
            vm_ssl_close(handle);
            
            // Allow LTask.remoteCall() to return
            LTask.post_signal();
            break;
        default:
            break;
    }
}

boolean sendHttps(void* user_data) {
    // This function should be executed in LinkIt main thread.
    
    // Initialize SSL connection
    vm_ssl_cntx ssl_cntx = {0};
    ssl_cntx.authmod = VM_SSL_VERIFY_NONE;  // Do not limit the encryption type of the server.
    ssl_cntx.connection_callback = sslCb;   // SSL event handler callback. This will be
    ssl_cntx.host = (VMCHAR*)pContext->serverUrl;
    ssl_cntx.port = pContext->port;
    ssl_cntx.ua = NULL;
    vm_ssl_connect(&ssl_cntx);
    
    // By returning false in this function,
    // LTask.remoteCall will block the execution
    // of the Arduino thread until LTask.post_signal() is called.
    // We shall invoke LTask.post_signal() in
    // the event handler callback.
    return false;
    
}

char* MtkHttpClient::sendHTTPS(const char *request, const char* serverUrl, int port)
{
#if 1  // for debug input param
    Serial.print("Req=");
    Serial.println(request);
    Serial.print("URL=");
    Serial.println(serverUrl);
    Serial.print("Port=");
    Serial.println(port);
#endif
    
    // This method is invoked in Arduino thread
    //
    // Use LTask.remoteCall to invoke the sendHttps()
    // function in LinkIt main thread. vm_ssl APIs must be
    // executed in LinkIt main thread.
    MtkHttpContext context;
    context.data_read = context.data_sent = 0;
    context.request = request;
    context.serverUrl = serverUrl;
    context.port = port;
    context.response = "";
    pContext = &context;
    LTask.remoteCall(sendHttps, &context);
    
    // Build the response - TODO: when is this response released?
    int len = context.response.length();
    char* response = new char[len + 1]();
    context.response.toCharArray(response, len + 1);
    
#if 1
    Serial.println("returned response:");
    Serial.println(response);
#endif
    
    return response;
}

char* MtkHttpClient::sendHTTP(const char *request, const char* serverUrl, int port)
{
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

char* MtkHttpClient::send(const char* request, const char* serverUrl, int port) {
    // TODO: probably not the best way to detect protocol...
    switch(port)
    {
        case 80:
            return sendHTTP(request, serverUrl, port);
        case 443:
            return sendHTTPS(request, serverUrl, port);
        default:
            return sendHTTP(request, serverUrl, port);
    }
}

bool MtkHttpClient::usesCurl() {
    /* Does not use curl command. */
    return false;
}
// /*
MtkHttpCurlClient::MtkHttpCurlClient() {
}

char* MtkHttpCurlClient::send(const char* request, const char* serverUrl,
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

bool MtkHttpCurlClient::usesCurl() {
    // Use curl command
    return false;  // Mtk board does not support Curl
}
//*/

MtkDateTimeProvider::MtkDateTimeProvider() {
    /* These are initialized to defaults. The defaults will cause the request
     * to fail, but the AWS Services response will contain the correct time
     * which will be fed to sync(). */
    
#if 1
    year = 2000;
    month = 1;
    day = 1;
    hour = 0;
    minute = 0;
    second = 0;
    /* Never synced so syncTime is 0. */
    syncedTime = 0;
#else
    year = 2015;
    month = 4;
    day = 27;
    hour = 9;
    minute = 18;
    second = 0;
    /* Never synced so syncTime is 0. */
    syncedTime = 0;
#endif
}

const char* MtkDateTimeProvider::getDateTime() {
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

bool MtkDateTimeProvider::syncTakesArg(void) {
    return true;
}

void MtkDateTimeProvider::sync(const char* dateTime) {
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


void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(LWiFi.SSID());
    // print your LWiFi shield's IP address:
    IPAddress ip = LWiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    // print the received signal strength:
    long rssi = LWiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
void Mtk_Wifi_Setup(const char* pSSID, const char* pPassword) {
    // attempt to connect to Wifi network:
    LWiFi.begin();
    while (!LWiFi.connectWPA(pSSID, pPassword)) {
        delay(1000);
        Serial.println("retry WiFi AP");
    }
}
