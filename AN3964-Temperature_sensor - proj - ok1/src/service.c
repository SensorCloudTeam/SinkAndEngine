#include "service.h"

char basicData[3000];
char requestData[100];
/*����Ӧ������*/
char sResponse[256];
char pHead[200];
char pTail[20];
char sCommand[20];
char sResult[100];
char sErrorMsg[100];
//SensorData data1 = {"00001","60","20"};
//SensorData data2 = {"00002","60","30"};
char* sinkId = "81273981998234814341";
SensorData datas[2];
int len; 
int i;
char id[10]; //װ�ش�����id��int���ͣ�
char value[10];  //װ�ش��������ݣ�float���ͣ�
char rate[10]; //װ�ش�����Ƶ�ʣ�float���ͣ�
int overtime = 0;
int sendRegC = 1; //��������Ĵ���
char tempS[10];
int buf_C = 0;  //buf�ĳ���
_Bool m_bOpened = FALSE;
_Bool m_bRegistered = FALSE;
int s = 0;

void entry(){
		
		LCD_GLASS_Clear();
		LCD_GLASS_ScrollSentence("  ** Service start now **  ",1,SCROLL_SPEED_L);
		composeRequest();
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString(" Cm OK");
		Delay(5000);//delay 5s

		do{
			// buf ���������
			end = 0;
			count = 0;
			//����ע����Ϣ
			sendRequest();
			//�������
			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){}
			Delay(5000);//delay 5s
				
			//�� ������� ���� ��ʱ6*10 = 60s ����while
			while(!(strstr(buf,"}}")||overtime>=10))
			{
				Delay(6000);//delay 6s
				overtime++;//��ʼ��ʱ
			}
			LCD_GLASS_Clear();
			sprintf(tempS,"  OV %d",overtime);
			LCD_GLASS_DisplayString(tempS);
			Delay(5000);//delay 5s
			if(overtime>=10)
			{
				m_bRegistered = FALSE;
				overtime = 0;//��ʱ����
			}
			else
			{
				buf_C = strlen(buf);
				m_bRegistered = decompose(buf);
				// Display startup message
				LCD_GLASS_ScrollSentence((unsigned char*)m_sErrorMsg,1,SCROLL_SPEED_L);
				overtime = 0;//��ʱ����
			}
			Delay(10000);//delay 10s ��ֹ�������ݻ��ڷ���
		}while(!m_bRegistered);
	
		while(TRUE){
			send();
			Delay(1000);
		};
};

void send(){
	
	/*
	datas[0] = data1;
	datas[1] =data2;
	compose(datas);
	*/
	compose();
	LCD_GLASS_Clear();
	LCD_GLASS_DisplayString("Send");
	Delay(1000);

	serialPutString(basicData);
	
	
};
void composeRequest(){
	strcat(requestData,"{\"Request\":{\"sinkid\":\"");
	strcat(requestData,sinkId);
	strcat(requestData,"\"}}");
}
void sendRequest(){
	USART_ClearFlag(USART3,USART_FLAG_TC);
	serialPutString(requestData);
	sprintf(tempS," Sen %d",sendRegC++);
	LCD_GLASS_Clear();
	LCD_GLASS_DisplayString(tempS);
	Delay(5000);//delay 5s

}
_Bool decompose(char *request){
		strcpy(pHead, strstr(request,"{\"Answer\":{"));
		if(NULL==pHead)
		{
			strcpy(m_sErrorMsg, "Invalid  register response!");
			return FALSE;
		}

		strcpy(pTail, strstr(request,"}}"));
		if(NULL==pTail)
		{
			strcpy(m_sErrorMsg, "Invalid  register response!");
			return FALSE;
		}
		
		
		/*
		if(pHead > pTail)
		{
			strcpy(m_sErrorMsg, "Invalid  register response!");
			return FALSE;
		}*/

		memcpy(sResponse,pHead, strlen(pHead)-strlen(pTail+2)); //2ָ����"}}"�ĳ���
		
		
		strcpy(pHead,strstr(sResponse,"\"command\":"));
		strcpy(pTail,strstr(sResponse,"}}"));
		s = strlen(pHead+10)-strlen(pTail);
		memcpy(sCommand,pHead+10,s); //10ָ���ǡ� "command": ���ĳ���
		strcpy(&sCommand[s],"\0");
	
		if(strcmp(sCommand,"\"ok\"")==0){
					return TRUE;
		}
		return FALSE;
		
}
void compose(/*SensorData *datas*/){
	//strcpy(basicData,"{");
	strcat(basicData,"{\"Data\":{");
	strcat(basicData,"\"sinkid\":");
	strcat(basicData,"\"");
	strcat(basicData,sinkId);
	strcat(basicData,"\"");
	strcat(basicData,",");
	strcat(basicData,"\"values\":[");
	//len = sizeof(datas)/sizeof(SensorData);
	len = 5;
	
	for(i =0;i<len;i++){
		strcat(basicData,"{");
		strcat(basicData,"\"sensorid\":");
		strcat(basicData,"\"");
		//strcat(basicData,datas[i].sensor_id);
		sprintf(id,"%d",i);
		strcat(basicData,id);
		strcat(basicData,"\"");
		strcat(basicData,",");
		
		strcat(basicData,"\"samplerate\":");
		//strcat(basicData,datas[i].sample_rate);
		sprintf(rate,"%f",sensorCycle[i]);
		strcat(basicData,rate);
		strcat(basicData,",");
		
		strcat(basicData,"\"value\":");
		strcat(basicData,"\"");
		//strcat(basicData,datas[i].value);
		sprintf(value,"%f", SensorDatas[i][ d_first[i] ]);
		strcat(basicData,value);
		strcat(basicData,"\"");
		strcat(basicData,"}");
		if(i<len-1){
			strcat(basicData,",");
		}
	}
	strcat(basicData,"]}}");
};



/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void serialPutChar(char c)
{
    USART_SendData(USART3,c);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			{}
}

/*******************************************************************************
* Function Name  : SerialPutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void serialPutString(char *s)
{
	while (*s != '\0')
	{
		serialPutChar(*s);
		s ++;
	}
}

