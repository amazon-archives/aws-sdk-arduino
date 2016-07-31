# Version 1 of the Experimental AWS SDK for Arduino

An experimental SDK for contacting AWS Services on Arduino-compatible devices. Currently it supports Amazon DynamoDB, Amazon Kinesis and Amazon SNS. More services coming soon.

All Amazon DynamoDB operations are supported. The code for creating, serializing, and deserializing Kinesis input and output objects is included, but the devices that the experimental SDK has been tested on do not have readily available HTTPS support. This code has been included so it can be used by those who want to do further experimenting with Kinesis and HTTPS.

The SDK is extensible to non-Arduino-compatible devices by implementing the interfaces in `DeviceIndependentInterfaces.cpp`/`.h`. See `SparkAWSImplementations.cpp`/`.h` and `EdisonAWSImplementations.cpp`/`.h` for examples of this.

## Folder Structure

	* /common contains all the common source code
	* /sparkcore contains Spark IO Core device-specific implementations
	* /edison contains Intel Edison device-specfic implementations
	* /galileo contains Intel Galileo device-specific implementations
    * /mediatek contains MediaTek LinkIt One device-specific implementations


Depending on the device that you are working, Simply copy the those device-specific implementations to the Common directory so you can test out your samples using Arduino IDE.

Happy experimenting!

## Getting Started with the Samples

Trying the samples is a good way to get started with using the SDK.

Getting the samples working has the following steps: setting up the DynamoDB table, importing the SDK and copying over the sample, creating the `keys.h` and `keys.cpp` files, and setting up the hardware. These steps are outlined for the samples for both the Spark core and Intel Galileo, so be sure you are following only the directions corresponding to the device and sample you are using.

If you are using a device other than Spark or Galileo, you may want to read through these steps anyway before implementing the interfaces in `DeviceIndependentInterfaces.cpp`/`.h` for your device.

### Step 1: Setting up the DynamoDB Table

For either device you will need to set up a DynamoDB table with the same name, hash key, and range key as in the sample you are using. These values are defined as constants in the sample, i.e. `HASH_KEY_NAME` and `TABLE_NAME`.

You can follow the steps below to get the tables set up with the right values, chosing the right set of instructions based on which sample you are using.

#### Table used by SparkGetItemSample and GalileoSample:

* Log into the [AWS Console](http://console.aws.amazon.com/) and navigate to DynamoDB.
* Click on the "Create Table" button.
* Enter "AWSArduinoSDKDemo" as the *Table Name*, "DemoName" as the *Hash Attribute Name*, and "id" as the *Range Attribute Name*. Be sure to mark *DemoName* as *String* and *id* as *Number*.
* For this example, we won't add indexes, so just press continue on the *Add Indexes (optional)* page.
* Just one *Read Capacity Unit* and one *Write Capacity Unit* will be enough for this demo. Press continue with these values.
* Uncheck *Use Basic Alarms* and continue again.
* Check that the information is correct on the *Review* page, then create the table!
* After the table has finished creating, double click on it to explore it. Here you should press the *New Item* button and create an item with the following values:
  * "DemoName": *String*, "Colors"
  * "id": *Number*, "1"
  * "R", *Number* "255"
  * "G", *Number* "255"
  * "B", *Number* "255"

#### Table used by SparkPutItemSample:

* Log into the [AWS Console](http://console.aws.amazon.com/) and navigate to DynamoDB.
* Click on the "Create Table" button.
* Enter "AWSArduinoSDKTests" as the *Table Name*, "device" as the *Hash Attribute Name*, and "Time" as the *Range Attribute Name*. Be sure to mark both as *String*.
* For this example, we won't add indexes, so just press continue on the *Add Indexes (optional)* page.
* Just one *Read Capacity Unit* and one *Write Capacity Unit* will be enough for this demo. Press continue with these values.
* Uncheck *Use Basic Alarms* and continue again.
* Check that the information is correct on the *Review* page, then create the table!

### Step 2: Importing SDK and Copying Sample

This step is different for the Spark Core and Intel Galileo.

#### Connected Maraca Sample (Edison/SparkCore/MediaTek)

follow the step by step guide: http://bit.ly/aws-iot-hackseries

#### Intel Galileo/Edison Sample

With Galileo or Edison, you should be using the Arduino IDE from Intel as it includes Galileo and Edison libraries. [Link to Intel-Arduino IDE](https://communities.intel.com/docs/DOC-22226).

Make an `AWSArduinoSDK` directory in the Arduino IDE's `libraries` directory (e.g. `~/Arduino/libraries/AWSArduinoSDK`).

Move all of the files from the SDK's `src/common` directory into the `AWSArduinoSDK` directory. Import the library.

Create a new sketch with the Arduino IDE and copy and paste the sample code into it.


#### Spark IO Core Sample

This assumes you already have your Spark set up and are able to program it with Spark Build. If you do not, head over to [Spark's website](http://docs.spark.io/).

Open up the Spark Build web page and create a new app. Name it whatever you would like.

Copy the contents of the sample you are using into the `.ino` file of your new app.

Next you need to import the SDK. Because the Spark Build IDE isn't local to your machine, you can't just `cp` the files over. Instead use the "+" tab in the top right corner of the Spark Build page to create a new file for each `.cpp`/`.h` file in the `src/` directory, except `GalileoAWSImplementations` and `AmazonKinesisClient`. Then copy and paste the contents of each file.



### Step 3: Creating `keys.h` and `keys.cpp`

You will need to create and add `keys.h` and `keys.cpp` into the `AWSArduinoSDK` directory you made. These files define the `awsKeyID` and `awsSecKey` values used by the sketch, the files may be structured as following:

```
// keys.h
#ifndef KEYS_H_
#define KEYS_H_

extern const char* awsKeyID;  // Declare these variables to
extern const char* awsSecKey; // be accessible by the sketch

#endif
```

```
// keys.cpp
#include "keys.h"

const char* awsKeyID = "YOUR AWS KEY ID HERE";
const char* awsSecKey = "YOUR AWS SECRET KEY HERE";
```

Add these files as you added the source files. That is: with Spark, use the "+" button, and with Galileo, move them to the `AWSArduinoSDK` directory under Arduino's `libraries` directory.

### Step 4: Setting up Hardware

To use the samples you must have the correct breadboard wiring. The samples use different wiring, but use the following rules to create them:

Buttons: Connect buttons by wiring one leg of the button to the 3v or 5v pin. Connect one leg to ground with a resistor, and also wire it to the pin that is reading the value.

RGB LED: For the multicolored LED, wire the cathode (the longest leg) to ground, then connect the remaining 3 legs to the corresponding input pins with a resistor.

#### Spark Core

Both spark samples use just one button connected to the D2 pin.

#### Intel Galileo

This sample uses five buttons and a RGB LED.

The RGB LED has the red leg connected to pin 6, the green leg connected to pin 9, and the blue leg connected to pin 10.

Buttons:

* Button for performing PutItem should be connected to pin 2
* Button for performing GetItem should be connected to pin 4
* Button for changing red color value should be connected to pin 7
* Button for changing green color value should be connected to pin 8
* Button for changing blue color value should be connected to pin 12

For Galileo/Edison, after the wiring is finished, you should be able to connect it to power, connect it to your computer via usb, and compile and upload the code with the Arduino IDE. Be sure to refer to the comments in the samples for help.

For Spark, after the wiring is finished, you should be able to connect it to your computer via USB, and *Flash* the code. Be sure to refer to the comments in the samples for help.

#### ESP8266

You can use these libraries with the [Arduino ESP8266](https://github.com/esp8266/arduino):.

```
#include <ESP8266WiFi.h>
#include <AmazonIOTClient.h>
#include "Esp8266AWSImplementations.h"

Esp8266HttpClient httpClient;
Esp8266DateTimeProvider dateTimeProvider;

AmazonIOTClient iotClient;
ActionError actionError;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WAP
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  iotClient.setAWSRegion("eu-west-1");
  iotClient.setAWSEndpoint("amazonaws.com");
  iotClient.setAWSDomain("foobar.iot.eu-west-1.amazonaws.com");
  iotClient.setAWSPath("/things/example-1/shadow");
  iotClient.setAWSKeyID("ID");
  iotClient.setAWSSecretKey("SECRET");
  iotClient.setHttpClient(&httpClient);
  iotClient.setDateTimeProvider(&dateTimeProvider);
}

void loop(){
  char* shadow = "{\"state\":{\"reported\": {\"foobar\": "bar"}}}";

  char* result = iotClient.update_shadow(shadow, actionError);
  Serial.print(result);

  delay(60000);
}

```
