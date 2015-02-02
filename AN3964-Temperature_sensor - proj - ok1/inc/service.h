#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#include "stm32l1xx_usart.h"
#include "stm32l1xx_it.h"
//#include "time.h"
#include "string.h"
#include "stdio.h"
#include "main.h"

#define MAX_MEASUREMENT_COUNT 16
#define MAX_OBJECT_COUNT 16


// 发送服务数据 处理接收数据
extern char m_sErrorMsg[100];
extern char buf[]; //接受的数据
extern int count;
extern int end;

// 得到传感器数据
extern float SensorDatas[5][10];
extern int d_current[],d_first[],d_end[];
extern float sensorCycle[5];

typedef unsigned short	WORD;

enum m_id{m_PH1,m_PH2,m_Light,m_Tem,m_Hum};

//定义数据格式
typedef struct _SensorData{
	char sensor_id[5];
	char *sample_rate;
	char *value;
} SensorData;

/*--------函数--------*/

void entry(void);
//编辑request数据
void composeRequest(void);
//发送request数据
void sendRequest(void);
//解析数据
_Bool decompose(char *s);
//组装数据
void compose(/*SensorData**/);
//发送数据
void send(void);



/* 4.（公用） 串口发送数据----------------------------------------------------------------*/
void serialPutChar(char c);
void serialPutString(char *s);
//uint16_t SerialRecieveChar(char c);


