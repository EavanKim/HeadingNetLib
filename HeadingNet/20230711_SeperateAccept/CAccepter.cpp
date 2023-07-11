#include "pch.h"

Heading::CAccepter::CAccepter( uint64_t _size )
{

}

Heading::CAccepter::~CAccepter( )
{

}

void Heading::CAccepter::Do_Update( )
{

}

void Heading::CAccepter::Do_Select( )
{
	DWORD ret = WSAWaitForMultipleEvents( m_accepts.size( ), m_events, FALSE, 0, TRUE );

	switch(ret )
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
	for( INT seek = ret - WSA_WAIT_EVENT_0; WSA_MAXIMUM_WAIT_EVENTS > seek; ++seek )
	{
		// 대기하지 않고 이벤트 검사를 한 결과확인
		// 0번인 처음 seek는 무조건 set 상태지만 일단 검사해버린다.
		// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
		DWORD EventResult = WaitForSingleObject( m_events[ seek ], 0 );
		switch(EventResult)
		{
		case WAIT_ABANDONED:
		case WAIT_OBJECT_0:
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			continue;
		default:
		{
			AcceptSessionEventMap::iterator iter = m_accepts.find( m_events[ seek ] );
			if( m_accepts.end() != iter )
			{
				sockaddr_in info = {}; // 얻어질까 과연
				SOCKET newsock = iter->second->CreateConnect(info);
				if(INVALID_SOCKET != newsock )
					m_newSockets.push_back(newsock);
			}
		}
			break;
		}

		// WaitForSingleObject 결과 처리 대상이었던 신호가 들어온 이벤트라면!
		// 다 끝나면 이벤트 셋!
		WSAResetEvent( m_events[ seek ] );
	}
}

bool Heading::CAccepter::Get_NewSocket( OUT SocketList& _newSocket )
{
	m_newSockets.swap( _newSocket );
	if( 0 != _newSocket.size( ) )
	{
		return true;
	}

	return false;
}
