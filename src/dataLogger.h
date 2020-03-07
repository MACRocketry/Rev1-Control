#ifndef __DATA_LOGGER_H_
#define __DATA_LOGGER_H_

typedef struct{
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
	float mx;
	float my;
	float mz;
	float temp;
	float pressure;
	float alt;
}imuData;

class dataLogger
{
public:
	int writeIMU(imuData data, float time = -1);
	int writeAccel(float x, float y, float z, bool accelOnly = true);
	int writeGyro(float x, float y, float z, bool gyroOnly = true);
	int writeMag(float x, float y, float z, bool magOnly = true);
	int writeBMP(float temp, float pressure, float alt, bool bmpOnly = true);
	dataLogger();
	~dataLogger();

private:
	int writeFormat();  
	
#define BUFFER_SIZE 512
	char str[BUFFER_SIZE] = "";
	SdCard *sd;
	int bytesUsed;
};

#endif