#include "ParseRecieveData.h"


// ParseRegisterRespone()
char sResponse[256];
char pHead[256];
char pTail[256];
char sCommand[100];
char sResult[100];

// GetNodeValue()
char strHead[20];
char strTail[21];
int nHead;
int nTail;
int nStart;

// substring()
int i;


//分析接受到的数据
_Bool ParseRegisterRespone(char *lpPackage, int nSize){
	strcpy(pHead, strstr(lpPackage,"<GateProtocol version=\"1.0\">"));
	if((NULL==pHead)||(pHead>lpPackage+nSize))
	{
		strcpy(m_sErrorMsg, "Invalid gate register response!");
		return FALSE;
	}

	strcpy(pTail, strstr(lpPackage,"</GateProtocol>"));
	if((NULL==pTail)||(pTail>lpPackage+nSize))
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
