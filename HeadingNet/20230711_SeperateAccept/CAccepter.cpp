#include "pch.h"

namespace Heading
{


	CAccepter::CAccepter( )
	{
	}

	CAccepter::~CAccepter( )
	{

	}

	bool CAccepter::Set_NewAcceptPort( uint16_t _port )
	{
		// 리턴 에러처리를 생각하면 이 Events 갯수를 넘어갈 수 없음.
		if( WSA_MAXIMUM_WAIT_EVENTS <= m_accepts.size( ) )
			return false;

		if( 1024 >= _port )
		{
			// 예약된 포트를 쓰려하면 밴
			return false;
		}
		// 최대값은 uint16_t 형으로 방어하므로 체크가 없습니다.

		CAcceptSession* session = new CAcceptSession( _port );
		if( session->Bind( ) )
		{
			m_accepts.insert( std::make_pair( session->Get_Event( ), session ) );
			m_events[ m_size ] = session->Get_Event( );
			++m_size;
			return true;
		}
		else
		{
			delete session;
			session = nullptr;
		}

		return false;
	}

	bool CAccepter::Set_CloseAcceptPort( uint16_t _port )
	{
		WSAEVENT Target = INVALID_HANDLE_VALUE;
		for( AcceptSessionEventMap::iterator iter = m_accepts.begin( ); m_accepts.end( ) != iter; ++iter )
		{
			if( iter->second->Get_Port( ) == _port )
			{
				iter->second->Release( );
				Target = iter->first;
				break;
			}
		}

		if( INVALID_HANDLE_VALUE != Target )
		{
			for( uint8_t seek = 0; WSA_MAXIMUM_WAIT_EVENTS > seek; ++seek )
			{
				if( m_events[ seek ] == Target )
				{
					--m_size;
					m_events[ seek ] = m_events[ m_size ];
				}
			}

			m_accepts.erase( Target );
		}

		return false;
	}

	void CAccepter::Do_Select( )
	{
		DWORD ret = WSAWaitForMultipleEvents( m_accepts.size( ), m_events, FALSE, 0, TRUE );

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
		for( INT seek = ret - WSA_WAIT_EVENT_0; m_size > seek; ++seek )
		{
			// 대기하지 않고 이벤트 검사를 한 결과확인
			// 0번인 처음 seek는 무조건 set 상태지만 일단 검사해버린다.
			// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
			DWORD EventResult = WaitForSingleObject( m_events[ seek ], 0 );
			switch( EventResult )
			{
			case WAIT_ABANDONED:
			case WAIT_OBJECT_0:
			case WAIT_TIMEOUT:
			case WAIT_FAILED:
				continue;
			default:
			{
				AcceptSessionEventMap::iterator iter = m_accepts.find( m_events[ seek ] );
				if( m_accepts.end( ) != iter )
				{
					sockaddr_in info = {}; // 얻어질까 과연
					SOCKET newsock = iter->second->CreateConnect( info );
					if( INVALID_SOCKET != newsock )
						m_newSockets.push_back( newsock );
				}
			}
			break;
			}

			// WaitForSingleObject 결과 처리 대상이었던 신호가 들어온 이벤트라면!
			// 다 끝나면 이벤트 셋!
			WSAResetEvent( m_events[ seek ] );
		}
	}

	bool CAccepter::Get_NewSocket( OUT SocketList& _newSocket )
	{
		m_newSockets.swap( _newSocket );
		if( 0 != _newSocket.size( ) )
		{
			return true;
		}

		return false;
	}

}
