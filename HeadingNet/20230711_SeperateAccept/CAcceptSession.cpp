#include "pch.h"

namespace Heading
{
	CAcceptSession::CAcceptSession( uint16_t _listenPort )
		: m_listenPort( _listenPort )
	{
		// The socket address to be passed to bind
		m_info.sin_family = AF_INET;
		m_info.sin_addr.s_addr = htonl( INADDR_ANY );
		m_info.sin_port = htons( m_listenPort );
	}

	CAcceptSession::~CAcceptSession( )
	{
	}

	bool CAcceptSession::Bind( )
	{
		int returnValue = 0;
		int loopCounter = 0;

		// 새로 바인딩하면 초기화해버리기
		if( INVALID_SOCKET != m_sock )
		{
			closesocket( m_sock );
			m_sock = INVALID_SOCKET;
			return false;
		}

		m_sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if( INVALID_SOCKET == m_sock )
		{
			return false;
		}

		returnValue = ::bind( m_sock, ( SOCKADDR* ) &m_info, sizeof( m_info ) );
		if( returnValue == SOCKET_ERROR )
		{
			int err = 0;
			if( WSAECONNREFUSED == ( err = WSAGetLastError( ) ) )
			{
				closesocket( m_sock );
				m_sock = INVALID_SOCKET;

				return false;
			}
			printf( "connect failed with error: %d\n", err );
			return false;
		}

		m_event = WSACreateEvent();
		WSAEventSelect( m_sock, m_event, FD_ACCEPT | FD_CLOSE ); // 여기가 아마 Event와 묶이는 부분
		if( SOCKET_ERROR == listen( m_sock, 5 ) )
			return false;

		return true;
	}

	SOCKET CAcceptSession::CreateConnect( sockaddr_in& _info )
	{
		int length = sizeof( _info );
		return ::accept( m_sock, ( struct sockaddr* )&_info, &length );
	}

	uint16_t CAcceptSession::Get_Port( )
	{
		return m_listenPort;
	}
}
