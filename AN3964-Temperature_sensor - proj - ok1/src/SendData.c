#include "SendData.h"


unsigned int m_nDataCount=0;
//定义 要发送的数据队列
//需要改进
PipeData m_DataList[DataListCount];

int i;



void Send()
{
	
}
//改！
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
		m_sErrorMsg = "Data exists in currect DataFrame!";
		return FALSE;
	}

	if(m_nDataCount>=m_pGateSpec->GetMeasurementCount()*
		m_pGateSpec->GetObjectCount())
	{
		m_sErrorMsg = "Too many data in currect DataFrame!";
		return FALSE;
	}

	CBasicData *pBasicData=new CBasicData;
	if(NULL==pBasicData)
	{
		m_sErrorMsg = "Failed to create BasicData object!";
		return FALSE;
	}

	pBasicData->m_nDataType = PIPE_DATA_BASIC;
	pBasicData->m_nMeasurementID = nMeasurementID;
	pBasicData->m_nObjectID = nObjectID;
	pBasicData->m_sData = sData;

	m_DataList[m_nDataCount++]=pBasicData;

	return TRUE;
}







