
#include <Arduino.h>
#include <SparkFunMPU9250-DMP.h>
#include <SdCard.h>

MPU9250_DMP imu;
SdCard sd;

float accelX = 0;
float accelY = 0;
float accelZ = 0;
float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
float magX = 0;
float magY = 0;
float magZ = 0;

void calcIMUData(void);
void setup()
{
    Serial.begin(9600);
	if (imu.begin() != INV_SUCCESS)
	{
		while (1)
		{
		  SerialPort.println("Unable to communicate with MPU-9250");
		  SerialPort.println("Check connections, and try again.");
		  SerialPort.println();
		  delay(5000);
		}
	}
	imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
	imu.setGyroFSR(2000);
	imu.setAccelFSR(2);
	imu.setLPF(5);
	imu.setSampleRate(10);
	imu.setCompassSampleRate(10);  
}

void loop()
{
	if ( imu.dataReady() )
	{
		imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
		printIMUData();
	}
	calcIMUData();
	writeImuData();
}

void writeImuData()
{
	sd.print("%d,"%milis());
	sd.printf("%.4f,%.4f,%.4f"% accelX, accelY,accelZ);
	sd.print("%.4f,%.4f,%.4f"% gyroX,gyroY,gyroZ);
	sd.print("%.4f,%.4f,%.4f"% magX,magY,magZ);
	sd.println();
}
void calcImuData(void)
{  
  accelX = imu.calcAccel(imu.ax);
  accelY = imu.calcAccel(imu.ay);
  accelZ = imu.calcAccel(imu.az);
  gyroX = imu.calcGyro(imu.gx);
  gyroY = imu.calcGyro(imu.gy);
  gyroZ = imu.calcGyro(imu.gz);
  magX = imu.calcMag(imu.mx);
  magY = imu.calcMag(imu.my);
  magZ = imu.calcMag(imu.mz);
}
