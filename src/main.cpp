
#include <SparkFunMPU9250-DMP.h>
#include <Adafruit_BMP280.h>
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
Adafruit_BMP280 bmp;
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
		  PRINTLN("Unable to communicate with MPU-9250");
		  PRINTLN("Check connections, and try again.");
		  delay(5000);
		}
	}
	if (!bmp.begin(0x76, BMP280_CHIPID)){
		while(1){
      		PRINTLN("Could not find a valid BMP280 sensor, check wiring!");
    	}
	}
	/* Default settings from datasheet. */
	bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

	imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS | INV_WXYZ_QUAT);
	imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT);
	imu.configureFifo(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS | INV_WXYZ_QUAT);
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
		// imu.dmpUpdateFifo();
		imu.computeEulerAngles();
		calc_imu_data();
		logger->writeIMU(data);
		// PRINT("SELF TEST:"); PRINTLN(imu.selfTest());
		// PRINT("qw: ");PRINT(imu.qw);PRINT(", qx: ");PRINT(imu.qx);PRINT(", qy: ");PRINT(imu.qy);PRINT(", qz: ");PRINTLN(imu.qz);
		// PRINT("yaw: ");PRINT(imu.yaw);PRINT(", pitch: ");PRINT(imu.pitch);PRINT(", roll: ");PRINTLN(imu.roll);

	}
	// delay(100);
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
  data.temp = bmp.readTemperature();
  data.pressure = bmp.readPressure();
  data.alt = bmp.readAltitude();
}
