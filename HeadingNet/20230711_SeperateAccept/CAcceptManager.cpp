#include "pch.h"

namespace Heading
{


	CAccept_Mgr::CAccept_Mgr( )
	{
	}

	CAccept_Mgr::~CAccept_Mgr( )
	{

	}

	bool CAccept_Mgr::Set_NewAcceptPort( uint16_t _port )
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

		CAccepter* session = new CAccepter( _port );
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

	bool CAccept_Mgr::Set_CloseAcceptPort( uint16_t _port )
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

	void CAccept_Mgr::Do_Select( )
	{
		DWORD ret = WSAWaitForMultipleEvents( (DWORD)m_accepts.size( ), m_events, FALSE, 0, TRUE );

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
		AcceptSessionEventMap::iterator iter = m_accepts.find( m_events[ ret - WSA_WAIT_EVENT_0 ] );
		if( m_accepts.end( ) != iter )
		{
			sockaddr_in info = {}; // 얻어질까 과연
			SOCKET newsock = iter->second->CreateConnect( info );
			if( INVALID_SOCKET != newsock )
			{
				CreatedSocketInfo NewInfo;
				NewInfo.AcceptPort = iter->second->Get_Port();
				NewInfo.Sock = newsock;
				m_newSockets.push_back( NewInfo );
			}
		}
		WSAResetEvent( m_events[ ret - WSA_WAIT_EVENT_0 ] );
	}

	bool CAccept_Mgr::Get_NewSocket( OUT NewSocketList& _newSocket )
	{
		m_newSockets.swap( _newSocket );
		if( 0 != _newSocket.size( ) )
		{
			return true;
		}

		return false;
	}

}
