#include "pch.h"

namespace Heading
{
	CEventBaseSession::CEventBaseSession( SOCKET _sock )
		: m_sock ( _sock )
	{ 

		const int on = 1;
		setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, (char*) & on, sizeof( on ) );
		
		uint64_t optVal;
		int optLen = sizeof(int);

		if (getsockopt(m_sock, 
						SOL_SOCKET, 
						SO_SNDBUF, 
						(char*)&optVal, 
						&optLen) != SOCKET_ERROR)
			printf("SockOpt Value: %lld\n", optVal);
		if (getsockopt(m_sock, 
						SOL_SOCKET, 
						SO_RCVBUF, 
						(char*)&optVal, 
						&optLen) != SOCKET_ERROR)
			printf("SockOpt Value: %lld\n", optVal);

		ioctlsocket(m_sock, FIONBIO, 0);
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

	int CEventBaseSession::RecvData( packetBuff& _datas )
	{
		int result = 0;

		char* buffer = nullptr;
		int length = 0;
		if( m_buffer.get_buffer( &buffer, &length ) )
		{
			result = ::recv( m_sock, buffer, length, 0 );
			if( -1 == result )
			{
				if(	WSAEWOULDBLOCK == WSAGetLastError() ) 
					return 0;

				if( INVALID_SOCKET != m_sock )
					closesocket( m_sock );

				m_sock = INVALID_SOCKET;

				return 0;
			}

			m_buffer.commit( result );
			m_buffer.get_data( &_datas );
		}

		return result;
	}

	void CEventBaseSession::onEventSend( )
	{
		m_isCanSend = true;
	}

	//https://gpgstudy.com/forum/viewtopic.php?t=24552
	int CEventBaseSession::SendData( )
	{
		int count = 0;
		int result = 0;

		// Event 유효기간 - Accept -> WouldBlock
		if( m_isCanSend && !m_sendBuff.empty( ) )
		{
			Header* packet = m_sendBuff.front( );
			if( nullptr != packet )
			{
				int sendresult = ::send( m_sock, ( char* ) packet, packet->length, 0 );
				if( SOCKET_ERROR == sendresult )
				{
					if( WSAEWOULDBLOCK == WSAGetLastError( ) )
						m_isCanSend = false;

					return result;
				}
				else if( 0 == sendresult )
				{
					// Close 상태
					m_isCanSend = false;
					return result;
				}
				else if( packet->length > sendresult )
				{
					return result;
				}
				else if( packet->length < sendresult )
				{
					return result;
				}
				else// if( packet->length == sendresult )
				{
					result += packet->length;
					delete packet;
					++count;
					printf( "[%i] sendCount \n", count );

					m_sendBuff.pop( );
				}
			}
			else
			{
				throw "Null Buffer Crash";
			}
		}

		return result;
	}

	void CEventBaseSession::enqueueSend( Header* _data )
	{
		m_sendBuff.push(_data);
	}
}