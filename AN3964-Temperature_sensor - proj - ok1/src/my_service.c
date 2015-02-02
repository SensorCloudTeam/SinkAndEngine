#include "my_service.h"



/* 1. Service Startup 启动服务-----------------------------------------------------------*/

//FlagStatus RX_status;
_Bool m_bOpened = FALSE;
_Bool m_bRegistered = FALSE;


typedef struct _GateParam {
		char m_sServerIP[20];
		WORD m_nServerPort;
		char m_sServiceName[20];
		char m_sGateKey[20];
		char m_sGateType[10];
		char m_sGateFilter[256];
		char m_sUserName[10];
		char m_sPassword[10];	
}GateParam;

char m_sMeasurementList[MAX_MEASUREMENT_COUNT][40];
char m_sTypeList[MAX_MEASUREMENT_COUNT][20];
char m_sObjectList[MAX_OBJECT_COUNT][20];
char Request[1000];
int overtime = 0;
int sendRegC = 1;
char tempS[10];

unsigned int m_nMeasurementCount=0;
unsigned int m_nObjectCount=0;

int buf_C = 0;

// 函数要用到的局部变量初始化
// gate_Param_Spec()
GateParam m_pGateParam;

// ComposeRegisterRequest()
char gateSpec[1000];

// InsertTimeStamp()
char sTime[40];

// InsertTimeStamp()
/*
time_t timer;
struct tm *tblock;
*/

// GetGateSpec()
char strM[100];
char strO[100];
unsigned int m;
unsigned int n;


/* 2. Parse Received Registered Data 分析注册后接收的数据--------------------------------*/

// 函数要用到的局部变量初始化
// ParseRegisterRespone()
char sResponse[256];
char pHead[256];
char pTail[50];
char sCommand[100];
char sResult[100];
char sErrorMsg[100];

// GetNodeValue()
char strHead[20];
char strTail[21];
int nHead;
int nTail;
int nStart;

// substring()
int i;


/* 3. Send Data 发送数据-----------------------------------------------------------------*/

#define DataListCount 5 // 5*1

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
		char m_sData[100];
}PipeData;

unsigned int m_nDataCount=0;
// 定义 要发送的数据队列
// 需要改进
PipeData m_DataList[DataListCount];

// 函数要用到的局部变量初始化
// NewData
PipeData pData;

// Send
char sValue[10];


// Write
char lpDataFrame[256];
//unsigned int nDataFrameSize;

// ComposeDataFrame
//char m_lpDataFrame[256];
char sBasicData[2000];

// ComposeBasicData
PipeData c_pData;
//char sMeasurementID[5][10]={"PH 值  1","PH 值  2","光照强度","温    度","土壤湿度"};
//char sObjectID[1][20]={"数学馆302"};
char sMeasurementID[50];
char sObjectID[50];

char sCycle[10];


// 临时发送数据
int tempData = 0;
int dataA[2];

// send_Re
char Res[20];

/* 主要程序-----------------------------------------------------------------*/

void my_proj()
{
		/*原来的代码
		//开启服务
		service_startup();
		
		do
		{
			// buf 缓冲区清空
			end = 0;
			count = 0;
			
			
			//发送注册信息
			send_Registered_data();
			//发送完成
			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
					{}
			
			Delay(5000);//delay 15s
			//LCD_GLASS_ScrollSentence(buf,1,SCROLL_SPEED_L);
			
						
			//当 接收完成 或者 超时6*10 = 60s 跳出while
			while(!(strstr(buf,"</GateProtocol>")||(overtime>=10)))
			{
				Delay(6000);//delay 6s
				overtime++;//开始计时
			}
			if(overtime>=10)
			{
				m_bRegistered = FALSE;
				overtime = 0;//计时清零
			}
			else
			{
				buf_C = strlen(buf);
				m_bRegistered = ParseRegisterRespone(buf);
				// Display startup message
				LCD_GLASS_ScrollSentence((unsigned char*)m_sErrorMsg,1,SCROLL_SPEED_L);
				overtime = 0;//计时清零
			}
			Delay(10000);//delay 10s 防止返回数据还在发送
		}while(!m_bRegistered);
		
		//关闭 USART3 接收中断
		//USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		// buf 缓冲区清空
		end = 0;
		count = 0;
		Delay(5000);//delay 5s
		
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString(" RE OK");
		Delay(10000);//delay 10s
		
		
		
		overtime = 1;
		while(TRUE)
		{
			
			if(overtime == 11)
			{
				//发送注册信息
				send_Registered_data();
				//发送完成
				while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
						{}
				overtime = 1;
				//Delay(5000);//delay 5s
			}
			
			
			//发送数据
			Send();	
			overtime++;
			//Delay(50000);//delay 15s
		}
		*/
		while(TRUE){
			//发送数据
			Send();	
			Delay(1000);//delay 1s
		}
}




/* 1. Service Startup 启动服务-----------------------------------------------------------*/

_Bool service_startup(void){
		// Display startup message
		LCD_GLASS_Clear();
		LCD_GLASS_ScrollSentence("  ** Service start now **  ",1,SCROLL_SPEED_L);
		// setting outGate info
		gate_Param_Spec("127.0.0.1",4040,"ECNU Measurement","ECNU Output","test","1234");

		m_bOpened = TRUE;

		// 改！！ 数组已经溢出！！ 分段
	  // - - 增加数组长度 解决
		ComposeRegisterRequest(Request);

		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString(" Cm OK");
		Delay(5000);//delay 5s

		/*
		USART_ClearFlag(USART3,USART_FLAG_TC);
		SerialPutString(Request);
		Delay(15000);//delay 15s
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString(" Sen 2");

		USART_ClearFlag(USART3,USART_FLAG_TC);
		SerialPutString(Request);
		Delay(15000);//delay 15s
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString(" Sen 3");
		*/

		return TRUE;
}

void send_Registered_data(void)
{
		USART_ClearFlag(USART3,USART_FLAG_TC);
		SerialPutString(Request);
		sprintf(tempS," Sen %d",sendRegC++);
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString(tempS);
		Delay(5000);//delay 5s
}

void gate_Param_Spec(char *sServerIP, WORD nServerPort,
							 char *sServiceName, char *sGateKey,
							 char *sUserName, char *sPassword){
		//在此处添加 Measurement 和 object
		AddMeasurement("PH Value 1","Number");
		AddMeasurement("PH Value 2","Number");
		AddMeasurement("Light intensity Value","Number");
		AddMeasurement("Temperature Value","Number");
		AddMeasurement("Humidity Value","Number");
		AddObject("Location One");

		strcpy(m_pGateParam.m_sServerIP, sServerIP);
		m_pGateParam.m_nServerPort = nServerPort;
		strcpy(m_pGateParam.m_sServiceName, sServiceName);
		strcpy(m_pGateParam.m_sGateKey, sGateKey);
		strcpy(m_pGateParam.m_sGateType, "out");
		strcpy(m_pGateParam.m_sGateFilter, "");
		strcpy(m_pGateParam.m_sUserName, sUserName);
		strcpy(m_pGateParam.m_sPassword, sPassword);
		
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString("  GaOK");
		Delay(5000);//delay 5s
}

_Bool AddMeasurement(char *sName, char *sType){
		if(m_nMeasurementCount<MAX_MEASUREMENT_COUNT)
		{
			strcpy(m_sMeasurementList[m_nMeasurementCount], sName);
			strcpy(m_sTypeList[m_nMeasurementCount++], sType);
			return TRUE;
		}
		return FALSE;
}

_Bool AddObject(char *sName){
		if(m_nObjectCount<MAX_OBJECT_COUNT)
		{
			strcpy(m_sObjectList[m_nObjectCount++], sName);
			return TRUE;
		}
		return FALSE;
}

void ComposeRegisterRequest(char sRequest[]){
		strcpy(sRequest,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
		strcat(sRequest,"<GateProtocol version=\"1.0\">\r\n");
		strcat(sRequest,"\t<cmd>GateRegisterRequest</cmd>\r\n");
		InsertTimeStamp(sRequest);

		AddNodeValue(sRequest,"ServiceName",m_pGateParam.m_sServiceName);
		AddNodeValue(sRequest,"GateKey",m_pGateParam.m_sGateKey);
		AddNodeValue(sRequest,"GateType",m_pGateParam.m_sGateType);
		GetGateSpec(gateSpec);
		strcat(sRequest,gateSpec);
		AddNodeValue(sRequest,"UserName",m_pGateParam.m_sUserName);
		AddNodeValue(sRequest,"Password",m_pGateParam.m_sPassword);

		strcat(sRequest,"</GateProtocol>\r\n");
}

void InsertTimeStamp(char sMessage[]){
		/*
		timer = time(NULL);
		tblock = localtime(&timer);

		sprintf(sTime,"%4d%02d%02d%02d%02d%02d",tblock->tm_year,tblock->tm_mon,tblock->tm_mday,
			tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
		*/
		sprintf(sTime,"%4d%02d%02d%02d%02d%02d",2013,04,8,
			8,8,8);

		strcat(sMessage,"\t<timestamp>");
		strcat(sMessage,sTime);
		strcat(sMessage,"</timestamp>\r\n");
}

void AddNodeValue(char *sRequest, char *sNode, char *sValue){
		strcat(sRequest,"\t<");
		strcat(sRequest,sNode);
		strcat(sRequest,">");
		strcat(sRequest,sValue);
		strcat(sRequest,"</");
		strcat(sRequest,sNode);
		strcat(sRequest,">\r\n");
}

void GetGateSpec(char *gSpec){
	
		strcpy(gSpec,"\t<GateSpec>\r\n");

		if(m_nMeasurementCount>0)
		{
			sprintf(strM,"\t\t<Measurements count=\"%d\">\r\n",m_nMeasurementCount);
			strcat(gSpec,strM);
			for(m = 0; m<m_nMeasurementCount; m++)
			{
				sprintf(strM,"\t\t\t<Measurement id=\"%d\" type=\"%s\">%s</Measurement>\r\n",
					m+1,m_sTypeList[m],m_sMeasurementList[m]);
				strcat(gSpec,strM);
			}
			strcat(gSpec,"\t\t</Measurements>\r\n");
		}
		
		if(m_nObjectCount>0)
		{
			sprintf(strO,"\t\t<Objects count=\"%d\">\r\n",m_nObjectCount);
			strcat(gSpec,strO);
			for(n = 0; n<m_nObjectCount; n++)
			{
				sprintf(strO,"\t\t\t<Object id=\"%d\">%s</Object>\r\n",n+1,m_sObjectList[n]);
				strcat(gSpec,strO);
			}
			strcat(gSpec,"\t\t</Objects>\r\n");
		}

		strcat(gSpec,"\t</GateSpec>\r\n");
		
}


/* 2. Parse Received Registered Data 分析注册后接收的数据--------------------------------*/

//分析接收到的数据
_Bool ParseRegisterRespone(char *lpPackage){
		strcpy(pHead, strstr(lpPackage,"<GateProtocol version=\"1.0\">"));
		if(NULL==pHead)
		{
			strcpy(m_sErrorMsg, "Invalid gate register response!");
			return FALSE;
		}

		strcpy(pTail, strstr(lpPackage,"</GateProtocol>"));
		if(NULL==pTail)
		{
			strcpy(m_sErrorMsg, "Invalid gate register response!");
			return FALSE;
		}

		if(pHead>pTail)
		{
			strcpy(m_sErrorMsg, "Invalid gate register response!");
			return FALSE;
		}

		strncpy(sResponse,pHead, pTail+15-pHead);

		if(!GetNodeValue(sResponse,"cmd",sCommand))
		{
			strcpy(m_sErrorMsg, "No command in gate message!");
			return FALSE;
		}
		if(strcmp("GateRegisterResponse",sCommand)!=0)
		{
			strcpy(m_sErrorMsg, "Invalid GateRegisterResponse command in gate message!");
			return FALSE;
		}
		if(!GetNodeValue(sResponse,"success",sResult))
		{
			strcpy(m_sErrorMsg, "No result in gate register response message!");
			return FALSE;
		}

		if(strcmp("true",sResult)==0)
		{
			strcpy(m_sErrorMsg, "Gate registered successfully.");
			return TRUE;
		}
		else
		{
			if(!GetNodeValue(sResponse,"errorMsg",sErrorMsg))
			{
				strcpy(m_sErrorMsg, "No errorMsg in gate message!");
				return FALSE;
			}
			if(strcmp("Gate ECNU Output of service ECNU Measurement is already online!",sErrorMsg)==0)
			{
				strcpy(m_sErrorMsg, "Gate registered successfully.");
				return TRUE;
			}
			GetNodeValue(sResponse,"errorMsg",m_sErrorMsg);
			return FALSE;
		}
}

_Bool GetNodeValue(char *sMessage, char *sNodeName, char *sNodeValue){
	
		sprintf(strHead,"<%s>",sNodeName);
		sprintf(strTail,"</%s>",sNodeName);

		nHead=pos_sub(sMessage,strHead);
		nTail=pos_sub(sMessage,strTail);

		if((-1==nHead)||(-1==nTail))
		{
			return FALSE;
		}

		nStart = nHead+strlen(strHead);
		if(nStart>nTail)
		{
			return FALSE;
		}
		substring(sNodeValue,sMessage,nStart,nTail);
		return TRUE;
}

//c		subString
void substring(char *dest,char *src,int start,int end)  
{  
    i=start;  
    if(start>strlen(src))return;  
    if(end>strlen(src))  
        end=strlen(src);  
    while(i<end)  
    {     
        dest[i-start]=src[i];  
        i++;  
    }  
    dest[i-start]='\0';
    return;  
}

//c		substring first position
int pos_sub(char *dest,char *sub){
		if(strstr(dest,sub)!=NULL)
		{
			return (strlen(dest) - strlen(strstr(dest,sub)));
		}
		return -1;
}


/* 3. Send Data 发送数据-----------------------------------------------------------------*/

void Send(void)
{
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString(" Send");
		Delay(5000);//delay 5s
	
		//dataA[0]=tempData;
		//dataA[1]=tempData*2;
		//tempData++;
		//if(tempData>49)
		//	tempData = 0;
		for(m = 0; m<m_nMeasurementCount; m++)
		{
			for(n = 0; n<m_nObjectCount; n++)
			{
				//sprintf(sValue,"%d",SensorDatas[m][n][0]);
				sprintf(sValue,"%f",SensorDatas[m][ d_first[m] ]);
				//if(tempData>49)
				//	tempData = 0;
				while(!NewData(m+1,n+1,sValue));
				//LCD_GLASS_Clear();
				//LCD_GLASS_DisplayString(" NewOK");
				//Delay(5000);//delay 5s
			}
		}
		Write();
		//LCD_GLASS_Clear();
		//LCD_GLASS_DisplayString(" W OK");
		//Delay(5000);//delay 5s
}

_Bool FindData(unsigned int nMeasurementID,unsigned int nObjectID)
{
		for(i=0; i<m_nDataCount; i++)
		{
			PipeData pData=m_DataList[i];
			if((pData.m_nMeasurementID==nMeasurementID)&&
				(pData.m_nObjectID==nObjectID))
			{
				return TRUE;
			}
		}
		return FALSE;
}

_Bool NewData(unsigned int nMeasurementID,unsigned int nObjectID, const char *sData)
{
		if(FindData(nMeasurementID,nObjectID))
		{
			strcpy(m_sErrorMsg,"Data exists in currect DataFrame!");
			return FALSE;
		}

		if(m_nDataCount>=m_nMeasurementCount*m_nObjectCount)
		{
			strcpy(m_sErrorMsg,"Too many data in currect DataFrame!");
			return FALSE;
		}

		if(NULL==&pData)
		{
			strcpy(m_sErrorMsg,"Failed to create BasicData object!");
			return FALSE;
		}
		// create a new pData
		pData.m_nDataType = PIPE_DATA_BASIC;
		pData.m_nMeasurementID = nMeasurementID;
		pData.m_nObjectID = nObjectID;
		strcpy(pData.m_sData,sData);

		// struct 赋值
		for(i=0;i<4;i++)
		{
			m_DataList[m_nDataCount].m_nDataType = pData.m_nDataType;
			m_DataList[m_nDataCount].m_nMeasurementID = pData.m_nMeasurementID;
			m_DataList[m_nDataCount].m_nObjectID = pData.m_nObjectID;
			strcpy(m_DataList[m_nDataCount].m_sData,pData.m_sData);
		}
		m_nDataCount++;

		return TRUE;
}

_Bool Write(void)
{
	/*
		if(!m_bOpened)
		{
			strcpy(m_sErrorMsg,"Gate is not opened!");
			return FALSE;
		}

		if(!m_bRegistered)
		{
			strcpy(m_sErrorMsg,"Gate is not registered!");
			return FALSE;
		}
*/
		if(!ComposeDataFrame(lpDataFrame))
		{
			strcpy(m_sErrorMsg,"Failed to allocate memory for DataFrame!");
		}
		else
		{
			USART_ClearFlag(USART3,USART_FLAG_TC);
			SerialPutString(lpDataFrame);
		}
		EmptyData();

		return TRUE;
}

_Bool ComposeDataFrame(char *lpDataFrame)
{
		ComposeBasicData(sBasicData);
		//nDataFrameSize = strlen(sBasicData);
		//memcpy(m_lpDataFrame,sBasicData,nDataFrameSize);
		strcpy(lpDataFrame,sBasicData);
		return TRUE;
}

void ComposeBasicData(char *sBasicData)
{
		strcpy(sBasicData,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
		strcat(sBasicData,"<DataFrame>\r\n");
		InsertTimeStamp(sBasicData);
		//InsertSeqNo(sBasicData);
		strcat(sBasicData,"\t<Values>\r\n");

		for(i=0; i<m_nDataCount; i++)
		{
			//c_pData=m_DataList[i];
			c_pData.m_nDataType = m_DataList[i].m_nDataType;
			c_pData.m_nMeasurementID = m_DataList[i].m_nMeasurementID;
			c_pData.m_nObjectID = m_DataList[i].m_nObjectID;
			strcpy(c_pData.m_sData,m_DataList[i].m_sData);			
			
			if(NULL==&c_pData)
			{
				continue;
			}

			strcat(sBasicData,"\t\t<Value type=\"");
			strcat(sBasicData,m_sTypeList[c_pData.m_nMeasurementID-1]);
			strcat(sBasicData,"\" mid=\"");
			sprintf(sMeasurementID,"%d",c_pData.m_nMeasurementID);
			//strcat(sBasicData,sMeasurementID[c_pData.m_nMeasurementID-1]);
			strcat(sBasicData,sMeasurementID);
			strcat(sBasicData,"\" oid=\"");
			sprintf(sObjectID,"%d",c_pData.m_nObjectID);
			//strcat(sBasicData,sObjectID[c_pData.m_nObjectID-1]);
			strcat(sBasicData,sObjectID);
			strcat(sBasicData,"\">");

			if(PIPE_DATA_BASIC==c_pData.m_nDataType)
			{
				strcat(sBasicData,(c_pData.m_sData));
			}
			else
			{
				continue;
			}
			strcat(sBasicData,"</Value>\r\n");
		}
		strcat(sBasicData,"\t</Values>\r\n");
		strcat(sBasicData,"</DataFrame>\r\n");
}
/*
void findType(char *type, int id)
{
		switch(id)
		{
				case 0:
					strcpy(type,"PH");
				break;
				case 1:
					strcpy(type,"光照强度");
				break;
				case 2:
					strcpy(type,"温度");
				break;
				case 3:
					strcpy(type,"土壤湿度");
				break;
				case 4:
					strcpy(type,"下雨监测");
				break;
		}
}
*/
void EmptyData(void)
{
		m_nDataCount = 0;
}

void get_Re(char *r)
{
	/*
	if((15>SensorDatas[Tem][ d_first[Tem] ])||(SensorDatas[Tem][ d_first[Tem] ]>25))
			strcpy(r,"wendu is bad");
		else if((65>SensorDatas[Light][ d_first[Light] ])||(SensorDatas[Light][ d_first[Light] ]>75))
			strcpy(r,"light is bad");
		else if((20>SensorDatas[Hum][ d_first[Hum] ])||(SensorDatas[Hum][ d_first[Hum] ]>35))
			strcpy(r,"shidu is bad");
		else
			strcpy(r,"every is ok");
		*/
		if((5>SensorDatas[m_Light][ d_first[m_Light] ])||(SensorDatas[m_Light][ d_first[m_Light] ]>7))
			strcpy(r,"light is bad");
		else if((5>SensorDatas[m_PH1][ d_first[m_PH1] ])||(SensorDatas[m_PH1][ d_first[m_PH1] ]>8))
			strcpy(r,"PH is bad");
		else
			strcpy(r,"everything is ok");
}

void send_Re(void)
{
		get_Re(Res);
		
		SerialPutString(Res);
}


/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void SerialPutChar(char c)
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
void SerialPutString(char *s)
{
	while (*s != '\0')
	{
		SerialPutChar(*s);
		s ++;
	}
}

