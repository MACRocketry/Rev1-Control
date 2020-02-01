// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//
 
// #include <Wire.h>
 
 
// void setup()
// {
//   Wire.begin();
 
//   Serial.begin(9600);
// 	Serial.println("hello!");
//   // while (!Serial);             // Leonardo: wait for serial monitor
//   Serial.println("\nI2C Scanner");
// }
 
 
// void loop()
// {
//   byte error, address;
//   int nDevices;
 
//   Serial.println("Scanning...");
 
//   nDevices = 0;
//   for(address = 1; address < 127; address++ )
//   {
//     // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
 
//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address<16)
//         Serial.print("0");
//       Serial.print(address,HEX);
//       Serial.println("  !");
 
//       nDevices++;
//     }
//     else if (error==4)
//     {
//       Serial.print("Unknown error at address 0x");
//       if (address<16)
//         Serial.print("0");
//       Serial.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");
 
//   delay(5000);           // wait 5 seconds for next scan
// }

#include <SparkFunMPU9250-DMP.h>
#include <Arduino.h>
#include <SdCard.h>
#include "dataLogger.h"

#define DEBUG 1
#define SerialPort Serial

#define PRINT(args...) 			\
do{								\
	if(DEBUG){					\
	SerialPort.print(args);	\
	}							\
} while (0);					\

#define PRINTLN(args...) 			\
do{								\
	if(DEBUG){					\
	SerialPort.println(args);	\
	}							\
} while (0);					\

mpu9250DMP imu;
SdCard *sd;
imuData data;
dataLogger *logger;

char str[64];
void calc_imu_data(void);

void setup()
{
	delay(2000);
	Serial.begin(9600);
	// pinMode(LED_BUILTIN, OUTPUT);
    // sd = SdCard::ConnectSdCard(0, 10);
	if (imu.begin() != INV_SUCCESS)
	{
		while (1)
		{
		  Serial.println("Unable to communicate with MPU-9250");
		  Serial.println("Check connections, and try again.");
		  Serial.println();
		  delay(5000);
		}
	}
	imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
	imu.setGyroFSR(250);
	imu.setAccelFSR(2);
	imu.setLPF(5);
	imu.setSampleRate(10);
	imu.setCompassSampleRate(10);  
	logger = new dataLogger();
}

void loop()
{
	int ret = imu.dataReady();
	if (ret)
	{
		imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
		calc_imu_data();
		logger->writeIMU(data);
		PRINT("gx: ");PRINT(data.gx);PRINT(", gy: ");PRINT(data.gy);PRINT(", gz: ");PRINTLN(data.gz);
	}
	delay(100);
}

void calc_imu_data(void)
{  
  data.ax = imu.calcAccel(imu.ax);
  data.ay = imu.calcAccel(imu.ay);
  data.az = imu.calcAccel(imu.az);
  data.gx = imu.calcGyro(imu.gx);
  data.gy = imu.calcGyro(imu.gy);
  data.gz = imu.calcGyro(imu.gz);
  data.mx = imu.calcMag(imu.mx);
  data.my = imu.calcMag(imu.my);
  data.mz = imu.calcMag(imu.mz);

}
