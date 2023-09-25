#include "pch.h"

namespace Heading
{
	CEventBaseSession::CEventBaseSession( SOCKET _sock )
		: m_sock ( _sock )
	{ 

		const int on = 1;
		setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, (char*) & on, sizeof( on ) );

		LINGER  ling = {0,};
		ling.l_onoff = 1;
		ling.l_linger = 0;

		setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (CHAR*)&ling, sizeof(ling));
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

	}

	//https://gpgstudy.com/forum/viewtopic.php?t=24552
	int CEventBaseSession::SendData( )
	{
		int count = 0;
		int result = 0;
		
		if( !m_sendBuff.empty( ) )
		{
			Header* packet = m_sendBuff.front( );
			if( nullptr != packet )
			{
				result = InternalSendData(packet);
				if ( 0 < result )
				{
					m_sendBufferSize += result;

					if ( m_sendBufferSize >= packet->length )
					{
						delete packet;
						++count;
						printf( "[%i] sendCount \n", count );
						m_sendBuff.pop( );
					}
				}
				else
				{
					
				}
			}
			else
			{
				throw "Null Buffer Crash";
			}
		}

		return result;
	}

	// 0바이트 전송 시 타는 곳
	// tcp_sendmsg_fastopen
	// https://github.com/torvalds/linux/blob/3aba70aed91f2b283f7952be152ad76ec5c34975/net/ipv4/tcp.c
	// __inet_stream_connect
	// https://android.googlesource.com/kernel/msm/+/android-msm-bullhead-3.10-n-preview-5/net/ipv4/af_inet.c
	// 
	// https://cr.yp.to/docs/connect.html
	int CEventBaseSession::InternalSendData( Header* _data )
	{
		m_sending.store(true);
		int result = 0;

		int sendresult = ::send( m_sock, ( char* ) _data, _data->length, 0 );
		if( SOCKET_ERROR == sendresult )
		{
			if( WSAEWOULDBLOCK == WSAGetLastError( ) )
				return 0;

			m_sending.store(false);
			return result;
		}
		else if( 0 == sendresult )
		{
			// Close ����
			m_isConnected = false;
			m_sending.store(false);
			return result;
		}
		else if( _data->length > sendresult )
		{
			m_sending.store(false);
			return result;
		}
		else if( _data->length < sendresult )
		{
			m_sending.store(false);
			return result;
		}
		else// if( packet->length == sendresult )
		{
			result += _data->length;
		}

		m_sending.store(false);
		return result;
	}

	bool CEventBaseSession::CheckLive( )
	{
		return m_isConnected;
	}

	void CEventBaseSession::enqueueSend( Header* _data )
	{
		m_sendBuff.push(_data);
	}

	void CEventBaseSession::trySend(Header* _data)
	{
		if ( m_sending.load() )
		{
			enqueueSend(_data);
		}
		else
		{
			if ( !m_sendBuff.empty() )
			{
				// 앞에 저장된 데이터가 있다면 전송 순서 지키기
				enqueueSend(_data);
				SendData();
			}
			else
			{
				InternalSendData(_data);
			}
		}
	}
}