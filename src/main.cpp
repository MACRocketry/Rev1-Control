
#include <Arduino.h>
#include <SparkFunMPU9250-DMP.h>
#include <SdCard.h>

#define SerialPort Serial

MPU9250_DMP imu;
SdCard sd;

float accel_x = 0;
float accel_y = 0;
float accel_z = 0;
float gyro_x = 0;
float gyro_y = 0;
float gyro_z = 0;
float mag_x = 0;
float mag_y = 0;
float mag_z = 0;

void calc_imu_data(void);
void write_imu_data(void);

void setup()
{
    SerialPort.begin(9600);
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
		calc_imu_data();
		write_imu_data();
	}
}

void write_imu_data(void)
{
	sd.printf("%d,",millis());
	sd.printf("%.4f,%.4f,%.4f",accel_x, accel_y,accel_z);
	sd.printf("%.4f,%.4f,%.4f",gyro_x,gyro_y,gyro_z);
	sd.printf("%.4f,%.4f,%.4f\n",mag_x,mag_y,mag_z);
}
void calc_imu_data(void)
{  
  accel_x = imu.calcAccel(imu.ax);
  accel_y = imu.calcAccel(imu.ay);
  accel_z = imu.calcAccel(imu.az);
  gyro_x = imu.calcGyro(imu.gx);
  gyro_y = imu.calcGyro(imu.gy);
  gyro_z = imu.calcGyro(imu.gz);
  mag_x = imu.calcMag(imu.mx);
  mag_y = imu.calcMag(imu.my);
  mag_z = imu.calcMag(imu.mz);
}
