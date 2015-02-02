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
extern char buf[];
extern int count;
extern int end;

// �õ�����������
extern float SensorDatas[5][10];
extern int d_current[],d_first[],d_end[];
extern float sensorCycle[5];

typedef unsigned short	WORD;

enum m_id{m_PH1,m_PH2,m_Light,m_Tem,m_Hum};


/*--------����--------*/

void my_proj(void);

/* 1. Service Startup ��������-----------------------------------------------------------*/

_Bool service_startup(void);
void send_Registered_data(void);

void gate_Param_Spec(char *sServerIP, WORD nServerPort,
							 char *sServiceName, char *sGateKey,
							 char *sUserName, char *sPassword);//���÷���
_Bool AddMeasurement(char *sName, char *sType);
_Bool AddObject(char *sName);
void ComposeRegisterRequest(char *sRequest);
void InsertTimeStamp(char *sMessage);
void AddNodeValue(char *sRequest, char *sNode, char *sValue);
void GetGateSpec(char *gSpec);


/* 2. Parse Received Registered Data ����ע�����յ�����--------------------------------*/

_Bool GetNodeValue(char *sMessage, char *sNodeName, char *sNodeValue);//�������ܵ�������
_Bool ParseRegisterRespone(char *lpPackage);

void substring(char *dest,char *src,int start,int end);
int pos_sub(char *dest,char *sub);


/* 3. Send Data ��������-----------------------------------------------------------------*/

//��������
void Send(void);
//���� ����ֵ
_Bool FindData(unsigned int nMeasurementID,unsigned int nObjectID);
_Bool NewData(unsigned int nMeasurementID,unsigned int nObjectID, const char *sData);
_Bool Write(void);
_Bool ComposeDataFrame(char *lpDataFrame);
void ComposeBasicData(char *sBasicData);

void EmptyData(void);

void get_Re(char *r);
void send_Re(void);


/* 4.�����ã� ���ڷ�������----------------------------------------------------------------*/
void SerialPutChar(char c);
void SerialPutString(char *s);
//uint16_t SerialRecieveChar(char c);


