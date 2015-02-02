#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#include "stm32l1xx_usart.h"
#include "string.h"
#include "stdio.h"
#include "main.h"


extern char m_sErrorMsg[100];


//分析接受到的数据
_Bool GetNodeValue(char *sMessage, char *sNodeName, char *sNodeValue);
_Bool ParseRegisterRespone(char *lpPackage, int nSize);

void substring(char *dest,char *src,int start,int end);
int pos_sub(char *dest,char *sub);




void SerialPutChar(char c);
void SerialPutString(char *s);
