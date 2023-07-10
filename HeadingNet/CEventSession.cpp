#include "pch.h"

namespace Heading
{
	CEventSession::CEventSession( )
	{

	}

	CEventSession::~CEventSession( )
	{
		ReleaseEvent( );
	}

	void CEventSession::BindEvent( SOCKET _sock, long _NetworkEventType )
	{
		if( INVALID_SOCKET != _sock )
		{
			m_session = _sock;
			m_event = WSACreateEvent( );
			int result = WSAEventSelect( m_session, m_event, _NetworkEventType );
		}
	}

	void CEventSession::ReleaseEvent( )
	{
		InterlockedExchange64( &m_threadAlive, 0 );

		if( SetEvent( m_event ) )
		{
			if( INVALID_HANDLE_VALUE != m_event )
			{
				CloseHandle( m_event );
				m_event = INVALID_HANDLE_VALUE;
			}

			if( INVALID_SOCKET != m_session )
			{
				closesocket( m_session );
				m_session = INVALID_SOCKET;
			}
		}
	}

	void CEventSession::SetUpSessionType( bool _read, bool _write, bool _accept )
	{
		if( _read )
			m_sessionTypeFlag |= FD_READ;
		if( _write )
			m_sessionTypeFlag |= FD_WRITE;
		if( _accept )
			m_sessionTypeFlag |= FD_ACCEPT;

		m_sessionTypeFlag |= FD_CLOSE;
	}

	CEventSession* CEventSession::Accept( long _NetworkEventType )
	{
		CEventSession* result = new CEventSession( );
		INT length = 0;
		BindEvent( WSAAccept( m_session, ( sockaddr* ) &m_info, &length, NULL, NULL ), _NetworkEventType );

		if( INVALID_SOCKET == result->m_session )
		{
			delete result;
			result = nullptr;
		}

		return result;
	}

	void CEventSession::EnumNetworkEvents( )
	{
		int EventsResult = WSAEnumNetworkEvents( m_session, m_event, &m_sessionEvents );
		if( SOCKET_ERROR == EventsResult )
		{
			// error
			return;
		}

		if( m_sessionEvents.lNetworkEvents & FD_ACCEPT )
		{
			Accept( m_sessionTypeFlag );
		}

		if( m_sessionEvents.lNetworkEvents & FD_READ )
		{
			RecvData( );
		}

		if( m_sessionEvents.lNetworkEvents & FD_WRITE )
		{
			SendData( );
		}
	}

	void CEventSession::RecvData( )
	{
		char* buffer = nullptr;
		uint64_t length = 0;
		m_socketBuffer.get_buffer( &buffer, &length );
		int result = ::recv( m_session, buffer, length, 0 );
		m_socketBuffer.commit( result );

		m_socketBuffer.get_data( &m_recvPackets );
	}

	void CEventSession::SendData( )
	{
		// TODO : 이걸 하나로 묶어서 보내는게 더 경제적일 것 같기는 한 데 방법이 있을지 고민 해 보기
		for( Header* packet : m_sendPackets )
		{
			::send( m_session, ( char* ) packet, packet->length, 0 );
			delete packet;
		}

		m_sendPackets.clear( );
	}
}
