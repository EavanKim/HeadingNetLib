#include "pch.h"

namespace Heading
{
	CSelecter::CSelecter( )
	{
	}

	CSelecter::~CSelecter( )
	{
	}

	void CSelecter::Do_Select( )
	{
		DWORD ret = WSAWaitForMultipleEvents( ( DWORD ) m_sessions.size( ), m_events, FALSE, 0, TRUE );

		switch( ret )
		{
		case WSA_WAIT_IO_COMPLETION:
		case WSA_WAIT_TIMEOUT:
		case -1:
			return;
		default:
			break;
		}

		// 리턴된 값이 가장 작은 인덱스임을 보장한다고 MSDN에 적혀있으므로
		// Wait 처리로 신호를 구분합니다.
		// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents
		// https://www.joinc.co.kr/w/man/4100/WASWaitForMultipleEvents 예제에 WaitForMultipleEvents의 리턴값이 에러가 아니라면 
		// 해당 리턴값에서 WSA_WAIT_EVENT_0을 뺀 값이 대상 인덱스
		ChatSessionEventMap::iterator iter = m_sessions.find( m_events[ ret - WSA_WAIT_EVENT_0 ] );
		if( m_sessions.end( ) != iter )
		{
			iter->second->Update( );
		}
		WSAResetEvent( m_events[ ret - WSA_WAIT_EVENT_0 ] );
	}
}
