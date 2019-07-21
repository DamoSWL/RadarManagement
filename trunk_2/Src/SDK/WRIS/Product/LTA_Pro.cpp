﻿
#include "../../../include/WRIS/product/lta_pro.h"

#define LEN 5

LTAPro::LTAPro(void)
{


}

LTAPro::~LTAPro(void)
{

}

/*! 打开数据文件
	\param szPath 文件的绝对路径
    \param bPhase 是否解析文件 
	\return
	  \arg 1 文件名为空
	  \arg 2 文件打开错误，文件不存在或路径不正确
	  \arg 3 文件读取错误
 */
int LTAPro::Open(string szPath)
{
	RVWPro::Open( szPath );
	//////////////////////////////////////////////////////////////////////////

	if(szPath == "")
		return 1;
	////////////////////////
	memcpy( &m_LTAHeader, m_pData, sizeof(__LTA_HEADER) );

	int len = m_LTAHeader.LayCellCou;
	int len2 = len * len;

	int iDataCnt = len2 * 3  * 4;

	float *pData = (float *)(m_pData + sizeof(__LTA_HEADER));

	m_pDataL = pData;
	m_pDataM = m_pDataL + len2 * 4;
	m_pDataH = m_pDataM + len2 * 4;

	return 0;
}

int LTAPro::OpenBuff( char *pBuff,int iLen)
{
	RVWPro::OpenBuff( pBuff, iLen);
	//////////////////////////////////////////////////////////////////////////

	memcpy( &m_LTAHeader, m_pData, sizeof(__LTA_HEADER) );

	int len = m_LTAHeader.LayCellCou;
	int len2 = len * len;

	int iDataCnt = len2 * 3  * 4;

	float *pData = (float *)(m_pData + sizeof(__LTA_HEADER));

	m_pDataL = pData;
	m_pDataM = m_pDataL + len2 * 4;
	m_pDataH = m_pDataM + len2 * 4;

	//"20100914_172743.00.026.004_0-0-0-0"
	char cName[256] = {0};
	tagRealObserSec &ob = m_Header.obserSec;

	sprintf( cName,
		"%04d%02d%02d_%02d%02d%02d.%02d.%03d.%03d_%d-%d-%d-%d",
		ob.iObsStartTimeYear,
		ob.iObsStartTimeMonth,
		ob.iObsStartTimeDay,
		ob.iObsStartTimeHour,
		ob.iObsStartTimeMinute,
		ob.iObsStartTimeSecond,
		0,
		ob.iProductNumber,
		ob.batch.scanmode,
		(int)m_LTAHeader.LenOfWin,
		(int)m_LTAHeader.HeightL,
		(int)m_LTAHeader.HeightM,
		(int)m_LTAHeader.HeightH
		);

	m_szPath = string(cName);
	return 0;
}



