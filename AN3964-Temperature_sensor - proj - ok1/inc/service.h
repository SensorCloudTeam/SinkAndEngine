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


// ���ͷ������� �����������
extern char m_sErrorMsg[100];
extern char buf[]; //���ܵ�����
extern int count;
extern int end;

// �õ�����������
extern float SensorDatas[5][10];
extern int d_current[],d_first[],d_end[];
extern float sensorCycle[5];

typedef unsigned short	WORD;

enum m_id{m_PH1,m_PH2,m_Light,m_Tem,m_Hum};

//�������ݸ�ʽ
typedef struct _SensorData{
	char sensor_id[5];
	char *sample_rate;
	char *value;
} SensorData;

/*--------����--------*/

void entry(void);
//�༭request����
void composeRequest(void);
//����request����
void sendRequest(void);
//��������
_Bool decompose(char *s);
//��װ����
void compose(/*SensorData**/);
//��������
void send(void);



/* 4.�����ã� ���ڷ�������----------------------------------------------------------------*/
void serialPutChar(char c);
void serialPutString(char *s);
//uint16_t SerialRecieveChar(char c);


