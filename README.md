# Version 1 of the Experimental AWS SDK for Arduino

An experimental SDK for contacting AWS Services on Arduino-compatible devices. Currently with DynamoDB support and potential Kinesis support.

See samples for Spark Core and Intel Galileo to get started. 

All DynamoDB operations are supported. The code for creating, serializing, and deserializing Kinesis input and output objects is included, but the devices that the experimental SDK has been tested on do not have readily available HTTPS support. This code has been included so it can be used by those who want to do further experimenting with Kinesis and HTTPS.

The SDK is extensible to non-Arduino-compatible devices by implementing the interfaces in `DeviceIndependentInterfaces.cpp`/`.h`. See `SparkAWSImplementations.cpp`/`.h` and `GalileoAWSImplementations.cpp`/`.h for examples of this.

Happy experimenting!
