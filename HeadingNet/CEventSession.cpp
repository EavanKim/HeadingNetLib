#include "pch.h"

namespace Heading
{
	CEventSession::CEventSession( )
	{

	}

	CEventSession::~CEventSession( )
	{

	}

	void CEventSession::BindEvent( SOCKET _sock )
	{
		m_sock = _sock;
		m_event = WSACreateEvent( );
		int result = WSAEventSelect( m_sock, m_event, 0 );
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

			if( INVALID_SOCKET != m_sock )
			{
				closesocket( m_sock );
				m_sock = INVALID_SOCKET;
			}
		}
	}

	void CEventSession::Update( LPWSAOVERLAPPED _overraped )
	{
		WSARecvData( _overraped );
		WSASendData( _overraped );
	}

	int CEventSession::Update( void* _ptr )
	{
		while( 0 != InterlockedCompareExchange64( &m_threadAlive, 0, 0 ) )
		{
			WaitForSingleObject( m_event, INFINITE );

			// 혹시 이벤트에서 깼는데 Thread가 정리당하는 중이라면
			if( 0 == InterlockedCompareExchange64( &m_threadAlive, 0, 0 ) )
			{
				// 탈출
				break;
			}

			RecvData( );
			SendData( );
		}

		return 0;
	}

	void CEventSession::WSARecvCallback( IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags )
	{

	}

	void CEventSession::WSASendCallback( IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags )
	{

	}

	void CEventSession::WSARecvData( LPWSAOVERLAPPED _overraped )
	{
		LPWSABUF buf;
		m_WSAsocketBuffer.get_buffer( &buf );
		DWORD readResult;
		int result = WSARecv( m_sock, buf, 1, &readResult, 0, _overraped, CEventSession::WSARecvCallback );
		m_WSAsocketBuffer.commit( readResult );

		m_WSAsocketBuffer.get_data( &m_WSArecvPackets );
	}

	void CEventSession::WSASendData( LPWSAOVERLAPPED _overraped )
	{
		// TODO : 이걸 하나로 묶어서 보내는게 더 경제적일 것 같기는 한 데 방법이 있을지 고민 해 보기
		for( WSAHeader* packet : m_WSAsendPackets )
		{
			LPWSABUF buf = new WSABUF( ); // LPWSABUF 배열을 들고있는게 가장 효과적일 듯...
			buf->len = packet->length;
			buf->buf = ( char* ) packet;
			WSASend( m_sock, buf, 1, ( LPDWORD ) &packet->length, 0, _overraped, CEventSession::WSASendCallback );
			delete buf;
		}

		m_WSAsendPackets.clear( );
	}

	void CEventSession::RecvData( )
	{
		char* buffer = nullptr;
		uint64_t length = 0;
		m_socketBuffer.get_buffer( &buffer, &length );
		int result = ::recv( m_sock, buffer, length, 0 );
		m_socketBuffer.commit( result );

		m_socketBuffer.get_data( &m_recvPackets );
	}

	void CEventSession::SendData( )
	{
		// TODO : 이걸 하나로 묶어서 보내는게 더 경제적일 것 같기는 한 데 방법이 있을지 고민 해 보기
		for( Header* packet : m_sendPackets )
		{
			::send( m_sock, ( char* ) packet, packet->length, 0 );
			delete packet;
		}

		m_sendPackets.clear( );
	}
}
