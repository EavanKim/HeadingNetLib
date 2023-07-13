#include "pch.h"

namespace Heading
{
	CEventBaseSession::CEventBaseSession( SOCKET _sock )
		: m_sock ( _sock )
	{
	}

	CEventBaseSession::~CEventBaseSession( )
	{
		Release( );
	}

	void CEventBaseSession::CreateAndSetEvent( long _eventFlag )
	{
		if( INVALID_SOCKET != m_sock )
		{
			m_event = WSACreateEvent();
			WSAEventSelect( m_sock, m_event, _eventFlag );
		}
	}

	void CEventBaseSession::Release( )
	{
		if( INVALID_SOCKET == m_sock )
		{
			closesocket( m_sock );
			m_sock = INVALID_SOCKET;
		}

		if( INVALID_HANDLE_VALUE == m_event )
		{
			WSACloseEvent(m_event);
			m_event = INVALID_HANDLE_VALUE;
		}
	}

	SOCKET CEventBaseSession::Get_Sock( )
	{
		return m_sock;
	}

	WSAEVENT CEventBaseSession::Get_Event( )
	{
		return m_event;
	}

	int CEventBaseSession::RecvData( )
	{
		int result = 0;

		char* buffer = nullptr;
		int length = 0;
		m_buffer.get_buffer( &buffer, &length );


		result = ::recv( m_sock, buffer, length, 0 );
		if( -1 == result )
		{
			if( INVALID_SOCKET != m_sock )
				closesocket( m_sock );

			m_sock = INVALID_SOCKET;

			return 0;
		}

		m_buffer.commit( result );
		m_buffer.get_data( &m_recvBuff );

		return result;
	}

	int CEventBaseSession::SendData( )
	{
		int result = 0;

		for( Header* packet : m_sendBuff )
		{
			::send( m_sock, ( char* )packet, packet->length, 0 );
			result += packet->length;
			delete packet;
		}

		m_sendBuff.clear();

		return result;
	}
}