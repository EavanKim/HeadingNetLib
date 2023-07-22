#include "pch.h"

//=============================================================================================================================
// 
// 다시 시작하는 주석잔치
// 
// 이해할 수 없는 부분들 기록하기
// 1. 왜 굳이 파이프라인 통신 하나만 연결하고 언제 송수신하는가의 타이밍으로 고통받는듯이 하나만 해야하는 것일까...?
//		- 아마도 비용 문제일까...?
// 2. 송신 타이밍은 서버와 클라 쌍방이 아무때나 보낼텐데 중첩해도 상관이 없을까...?
//		- 아마 드라이버 단에서 송 수신 자체가 분리되어있고 (Rx / Tx) 자체적인 버퍼에 쓰는 동작 자체의 차이이지 않을까
// 
//=============================================================================================================================

namespace Heading
{
	CSelecter::CSelecter( )
	{
		InterlockedExchange64(&m_bSessionLive, 1);
	}

	CSelecter::~CSelecter( )
	{
	}

	int CSelecter::Do_Select( void* _ptr )
	{
		while( 1 == InterlockedCompareExchange64(&m_bSessionLive, 0, 0) )
		{
			DWORD ret = WSAWaitForMultipleEvents( ( DWORD ) m_sessions.size( ), m_events, FALSE, 0, TRUE );

			switch( ret )
			{
			case WSA_WAIT_FAILED:
			case WSA_WAIT_IO_COMPLETION:
			case WSA_WAIT_TIMEOUT:
			{
				// would be need blocked but...we need process
				// 읽고있는 송신측을 100% 신뢰하고 블럭을 비울것이기 때문에
				// 내가 송신 시도 대기시키는 버퍼가 넘칠 수 있어도 지나가는 코드.
				// 만약 송신 시도 대기가 가득차도록 동작을 안하면 연결 끊어버리기가 필요하지만...
				// 지금은 그냥 넘기기만 합니다.
				if(	WSAEWOULDBLOCK == WSAGetLastError() ) 
					continue;
			}
			continue;
			default:
				break;
			}

			// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents
			// https://www.joinc.co.kr/w/man/4100/WASWaitForMultipleEvents 
			ChatSessionEventMap::iterator iter = m_sessions.find( m_events[ ret - WSA_WAIT_EVENT_0 ] );
			if( m_sessions.end( ) != iter )
			{
				iter->second->Update( );				
			}

			WSAResetEvent( m_events[ ret - WSA_WAIT_EVENT_0 ] );
		}

		return 0;
	}
	void CSelecter::Stop( )
	{
		InterlockedExchange64(&m_bSessionLive, 0);
	}
}
