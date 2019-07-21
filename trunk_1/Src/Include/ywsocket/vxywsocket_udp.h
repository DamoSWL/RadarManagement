/* YWSocket_UDP.h */

#ifndef _HEADER_YWSOCKET_CLIENT_UDP_20081026
#define _HEADER_YWSOCKET_CLIENT_UDP_20081026

#ifdef WIN32
	#include <Winsock2.h>
	#include <mstcpip.h>

	#pragma comment(lib, "wsock32.lib")		// search for wsock32 lib at compile time
	#pragma comment(lib, "Ws2_32.lib")		// search for wsock32 lib at compile time
	#pragma comment(lib, "mpr.lib")			// search for mpr lib at compile time

	#define CloseSocket  closesocket
#else
	#include <Socket.h>
	#include <In.h>
	#include <ioLib.h>
	#include <sockLib.h>
	#include <inetLib.h>
	#include "vxWorks.h"
	#include "stdio.h"
	#include "taskLib.h"
	#include "sysLib.h"

	#define CloseSocket close

#endif

#include "ywsocketdefine.h"

#include <pthread.h>

#include <iostream>
#include <string>
#include <vector>
#include "math.h"

using namespace std;

#define DEF_BUFFERSIZE  6  // ȱʡ����������С
/////////////////////////////////////////////////////////////////////////



//! YWSocket_UDP Զ������Vxworks��������ͨ����
/**
YWSocket_UDP��Զ������ͨ���࣬���� Windows Socket������UDP/IPЭ�顣���������UDP����/�ͻ��ˡ�
���ú���CreateCli(...)��
\author lava_sdb
\ingroup group_CDYW
\date 2008 - 2010

\b ������ʷ:	
\arg \c lava_sdb, 20080408, ����
\arg \c lava_sdb, 20081120, ���ͻ��˺ͷ���˺ϲ�Ϊһ����;

\b ��Ȩ
CopyRight ? 2003-2010 ChengDu YuanWang Science & Technology.,LTD.
*/

class YWSocket_UDP
{
	int _recThrExitFlag;   // �����߳��˳����
	timeval _recThrTimeout;  // �����̳߳�ʱֵ

	unsigned char *m_pRecvBuf; // ���ݽ��ջ���

#ifdef WIN32
	static void *CB_CleanReceiveThr( void *pSock );
#else
	static void CB_CleanReceiveThr( void *pSock );
#endif

	int m_socket;
	struct sockaddr_in m_sockaddr;

	int m_port;         //!< �˿ں�

	bool m_bIsReceiving;	//! �Ƿ������ݽ���״̬

	bool m_bManul;	//! �Ƿ��û�������ֹ����

	void *m_pPar;

	bool m_bStartReceive; // �Ƿ��������ݽ����߳�
	bool m_bCreateOK; // �Ƿ�Socket��������
	int m_iVxLeavePackgeSize; //! VX�ְ���ʣ�����С
	int m_tidReceive; //! ���ݽ����̵߳�ID

public:
	string s_message;		//! Socket״̬������Ϣ

	const char * GetError();
	//Construction
	//! ���캯��
	YWSocket_UDP();
	virtual ~YWSocket_UDP();
	//Attributes
	//! �õ� Socket ��������
	int  GetSoket()			{ return m_socket; };

	//! �õ��˿ںţ����ڿͻ��ˣ��õ�Ҫ���ӵķ���˵Ķ˿ں�
	int  GetPort()			{return m_port;};

	//Operations
	//! ����Socket
	bool Socket(int nAddressFormat = AF_INET,int nSocketType=SOCK_STREAM,int nProtocolType = 0);

	//! �󶨵�ָ����IP
	bool Bind( unsigned int nSocketPort, const char *szIP= NULL);

	/*! ��ֹ����ˣ��ͻ��ˣ�
	\param bMan �Ƿ��û�������ֹ���ӡ�
	- true �����ж�
	- false �����նˣ��������쳣��ԭ���ж�����
	*/
	bool Close( bool bMan=true );

	/*! ��������
		\param lpBuf,
		\param nBufLen
		\param sendIP
		\param sendPort
	*/
	int Send(char * lpBuf, int nBufLen, const char *sendIP, unsigned short int sendPort );

private:
	pthread_t m_thrRec;		//!< �̱߳�ʶ
	//! ѡȡ���еĽ��ջ���

	void *RecData( );
	void *RecDataT( );

	unsigned int m_iBuffLen;     //!< ����������

public:

	inline bool IsWorking() { return m_bCreateOK; };  // �Ƿ������ݽ��չ�����
	inline bool IsReceiving() { return m_bIsReceiving; };

	//! ����VxWorks�ְ���ʣ����Ĵ�С
	inline void SetVXLeavePackgeSize( int iLeaveLen ) { m_iVxLeavePackgeSize = iLeaveLen; };
	inline int GetVXLeavePackgeSize() { return m_iVxLeavePackgeSize; };

	/*! ����UDP
	\param bStartRec true���������ݽ����̣߳�false�����������ݽ����߳�
	\param szBindIP �󶨵�IP
	\param nBindPort �󶨵Ķ˿ں�
	\param bufLen tcp/IP�ײ㻺��Ŀ���
	\param timeout select��ʱֵ����λus;

	\b �޸ģ�
	- lava_sdb 2003-11-20 Create
	\return Success return true, fail return false
	*/
	bool Create( bool bStartRec, const char* szBindIP, unsigned int nBindPort, unsigned int bufLen, unsigned int prio,char *taskname,long timeout = 1000000);

	//! �������ݽ����߳�
	void StartReceive(char *taskname , int prio);

	//! ֹͣ���ݽ����߳�
	void StopReceive();

	//! ��������������յ�ʱ��������
	virtual int OnReceiveData( const char *pData, unsigned int len ) { return 0; };
	virtual void OnEvent(int EventType, void * pData=NULL, unsigned int len=0) {};

private:

	static void *thrReceive( void * pParam );
};

#endif



