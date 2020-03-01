
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
		imu.computeEulerAngles();
		calc_imu_data();
		logger->writeIMU(data);
		// PRINT("SELF TEST:"); PRINTLN(imu.selfTest());
		PRINT("gx: ");PRINT(imu.yaw);PRINT(", gy: ");PRINT(imu.pitch);PRINT(", gz: ");PRINTLN(imu.roll);
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
