#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#include "stm32l1xx_usart.h"
#include "main.h"
#include "string.h"

#include "service_startup.h"

//暂时需要自己写
#define DataListCount 1 // 1*1


extern char m_sErrorMsg[100];

typedef enum _PIPE_DATA_TYPE
{
	PIPE_DATA_UNKNOWN = -1,
	PIPE_DATA_BASIC = 0,
	PIPE_DATA_EXTENDED = 1
}PIPE_DATA_TYPE;

typedef struct _PipeData
{
		PIPE_DATA_TYPE m_nDataType;
		unsigned int m_nMeasurementID;
		unsigned int m_nObjectID;
}PipeData;



void Send();
//发送 测量值
_Bool FindData(unsigned int nMeasurementID,unsigned int nObjectID);
_Bool NewData(unsigned int nMeasurementID,unsigned int nObjectID, const char *sData);

