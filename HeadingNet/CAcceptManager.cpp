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
		if( WSA_MAXIMUM_WAIT_EVENTS <= m_accepts.size( ) )
			return false;

		if( 1024 >= _port )
		{
			return false;
		}

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

	bool CAccept_Mgr::Do_Select( )
	{
		bool result = false;
		DWORD ret = WSAWaitForMultipleEvents( (DWORD)m_accepts.size( ), m_events, FALSE, 0, TRUE );

		switch( ret )
		{
		case WSA_WAIT_IO_COMPLETION:
		case WSA_WAIT_TIMEOUT:
		case -1:
			if( WSAEWOULDBLOCK == ret )
				result = true;

			return result;
		default:
			break;
		}

		// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents
		// https://www.joinc.co.kr/w/man/4100/WASWaitForMultipleEvents
		AcceptSessionEventMap::iterator iter = m_accepts.find( m_events[ ret - WSA_WAIT_EVENT_0 ] );
		if( m_accepts.end( ) != iter )
		{
			sockaddr_in info = {};
			SOCKET newsock = iter->second->CreateConnect( info );
			if( INVALID_SOCKET != newsock )
			{
				CreatedSocketInfo NewInfo;
				NewInfo.AcceptPort = iter->second->Get_Port();
				NewInfo.Sock = newsock;
				m_newSockets.push_back( NewInfo );
				result = true;
			}
		}
		WSAResetEvent( m_events[ ret - WSA_WAIT_EVENT_0 ] );

		return result;
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

	void CAccept_Mgr::Dispose( )
	{
		for( AcceptSessionEventMap::iterator iter = m_accepts.begin( ); m_accepts.end( ) != iter; ++iter )
		{
			iter->second->Release();
		}

		m_accepts.clear();

		for( CreatedSocketInfo info : m_newSockets )
		{
			closesocket(info.Sock);
		}

		m_newSockets.clear();

		// m_accepts에서 비워졌을걸로 기대합니다.
		ZeroMemory(m_events, WSA_MAXIMUM_WAIT_EVENTS * sizeof(WSAEVENT));
	}

}
