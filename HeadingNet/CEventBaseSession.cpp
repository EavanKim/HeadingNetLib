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

	//https://gpgstudy.com/forum/viewtopic.php?t=24552
	int CEventBaseSession::SendData( )
	{
		Buffer buff = {};
		int Count = 0;
		int result = 0;

		while( !m_sendBuff.empty( ) )
		{
			Header* packet = m_sendBuff.front();
			if( nullptr != packet )
			{
				if( !buff.set_data( ( char* ) packet, packet->length ) )
				{
					char* sendrawdata;
					int sendlength;
					buff.get_send_data(&sendrawdata, &sendlength);
					int sendresult = ::send( m_sock, ( char* )sendrawdata, sendlength, 0 );
					if( SOCKET_ERROR == sendresult )
					{
						return result;
					}
					else if( 0 == sendresult )
					{
						return result;
					}
					continue;
				}
				else
				{
					result += packet->length;
					delete packet;
					++Count;
					printf( "[%i] sendCount \n", Count );
				}
			}

			// NULL 이 들어있더라도 front에 잘못된 값이 저장된 것이니 비웁니다.
			m_sendBuff.pop();
		}

		if( !buff.isEmpty( ) )
		{
			char* sendrawdata;
			int sendlength;
			buff.get_send_data(&sendrawdata, &sendlength);
			int sendresult = ::send( m_sock, ( char* )sendrawdata, sendlength, 0 );
			if( SOCKET_ERROR == sendresult )
			{
				return result;
			}
			else if( 0 == sendresult )
			{
				return result;
			}
		}

		return result;
	}

	void CEventBaseSession::enqueueSend( Header* _data )
	{
		m_sendBuff.push(_data);
	}
}