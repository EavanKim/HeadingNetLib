#include "pch.h"

namespace Heading
{
	CAccepter::CAccepter( uint16_t _listenPort )
		: CEventBaseSession( INVALID_SOCKET )
		, m_listenPort( _listenPort )
	{
		// The socket address to be passed to bind
		m_info.sin_family = AF_INET;
		m_info.sin_addr.s_addr = htonl( INADDR_ANY );
		m_info.sin_port = htons( m_listenPort );
	}

	CAccepter::~CAccepter( )
	{
		// 부모 클래스에서 소켓과 이벤트 삭제
	}

	bool CAccepter::Bind( )
	{
		int returnValue = 0;
		int loopCounter = 0;

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

		//m_event = WSACreateEvent();
		//WSAEventSelect( m_sock, m_event, FD_ACCEPT | FD_CLOSE );
		CreateAndSetEvent( FD_ACCEPT | FD_CLOSE );
		if( SOCKET_ERROR == listen( m_sock, 5 ) )
			return false;

		return true;
	}

	SOCKET CAccepter::CreateConnect( sockaddr_in& _info )
	{
		int length = sizeof( _info );
		return ::accept( m_sock, ( struct sockaddr* )&_info, &length );
	}

	uint16_t CAccepter::Get_Port( )
	{
		return m_listenPort;
	}
}
