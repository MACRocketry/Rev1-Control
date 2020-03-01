#include <Arduino.h>
#include <stdio.h>

#include "SdCard.h"
#include "dataLogger.h"

#define SerialPort Serial
#define DEBUG 1

#define PRINT(args...) 			\
do{								\
	if(DEBUG){					\
	SerialPort.print(args);		\
	}							\
} while (0);					\

#define PRINTLN(args...) 			\
do{								\
	if(DEBUG){					\
	SerialPort.println(args);	\
	}							\
} while (0);					\



dataLogger::dataLogger()
{
	sd = SdCard::ConnectSdCard(0, 10);
	bytesUsed = 0;	
	sd->openNextFile();
	writeFormat();


}

dataLogger::~dataLogger()
{}


int dataLogger::writeFormat()
{
	char temp[BUFFER_SIZE] = "";
	sd->write(temp, snprintf(temp, BUFFER_SIZE ,"time, ax, ay, az, gx, gy, gz, mx, my, mz\n"));
	return bytesUsed;
}

int dataLogger::writeIMU(imuData data, float time)	
{		
	bytesUsed = 0;
	if (time<0){
		bytesUsed += snprintf(str + bytesUsed, BUFFER_SIZE - bytesUsed, "%lu,", millis());
	}
	writeAccel(data.ax, data.ay, data.az,false);
	writeGyro(data.gx, data.gy, data.gz,false);
	writeMag(data.mx, data.my, data.mz, false);
	bytesUsed += snprintf(str + bytesUsed, BUFFER_SIZE - bytesUsed, "\n");
	PRINT(str);
	PRINTLN(sd->write(str, bytesUsed));
	return bytesUsed;
}

int dataLogger::writeAccel(float x, float y, float z, bool accelOnly)
{
	int numBytes = 0;
	if (accelOnly) bytesUsed = 0;
	numBytes = snprintf(str + bytesUsed , BUFFER_SIZE - bytesUsed, "%s,%s,%s",String(x).c_str(),String(y).c_str(),String(z).c_str());
	if (accelOnly) numBytes += snprintf(str + bytesUsed + numBytes, BUFFER_SIZE - bytesUsed - numBytes, "\n");
	bytesUsed+=numBytes;
	return numBytes;
}
int dataLogger::writeMag(float x, float y, float z, bool magOnly)
{
	int numBytes = 0;
	if (magOnly) bytesUsed = 0;
	numBytes += snprintf(str + bytesUsed,BUFFER_SIZE - bytesUsed,"%s,%s,%s",String(x).c_str(),String(y).c_str(),String(z).c_str());
	if (magOnly) numBytes += snprintf(str + bytesUsed + numBytes, BUFFER_SIZE - bytesUsed,"\n");
	bytesUsed += numBytes;
	return bytesUsed;
}
int dataLogger::writeGyro(float x, float y, float z, bool gyroOnly)
{
	int numBytes = 0;
	if (gyroOnly) bytesUsed = 0;
	numBytes += snprintf(str + bytesUsed,BUFFER_SIZE - bytesUsed, "%s,%s,%s",String(x).c_str(),String(y).c_str(),String(z).c_str());
	if (gyroOnly) numBytes += snprintf(str + bytesUsed + numBytes,BUFFER_SIZE - bytesUsed, "\n");
	bytesUsed += numBytes;
	return bytesUsed;
}