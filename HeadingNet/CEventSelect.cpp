#include "pch.h"


namespace Heading
{
	CEventSelect::CEventSelect( uint16_t _port )
		: m_port( _port )
	{

	}

	CEventSelect::~CEventSelect()
	{
	}

	void CEventSelect::SetupAddrInfo()
	{
		sockaddr_storage storage;
		memset( &storage, 0, sizeof storage );
		// The socket address to be passed to bind
		m_info.sin_family = AF_INET;
		m_info.sin_addr.s_addr = htonl( INADDR_ANY );
		m_info.sin_port = htons( m_port );
	}

	bool CEventSelect::bind()
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

		do
		{
			if( 5 < loopCounter )
			{
				int winerror = GetLastError();
				// exception 객체 생성되면 throw하면서 에러 정보 송신
				return false;
			}

			m_sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			if( INVALID_SOCKET == m_sock )
			{
				continue;
			}

			returnValue = ::bind( m_sock, ( SOCKADDR* )&m_info, sizeof( m_info ) );
			if( returnValue == SOCKET_ERROR )
			{
				int err = 0;
				if( WSAECONNREFUSED == ( err = WSAGetLastError() ) )
				{
					closesocket( m_sock );
					m_sock = INVALID_SOCKET;
					continue;
				}
				printf( "connect failed with error: %d\n", err );
				return INVALID_SOCKET;
			}
		}
		while( S_OK != returnValue );

		m_selectEvent = WSACreateEvent();
		WSAEventSelect( m_sock, m_selectEvent, FD_ACCEPT | FD_CLOSE );
		if( SOCKET_ERROR == listen( m_sock, 5 ) )
			return false;

		return true;
	}

	void CEventSelect::setupSelect( bool _read, bool _write )
	{
		readSelect = _read;
		writeSelect = _write;
	}

	void CEventSelect::Do_Select()
	{

	}

	void CEventSelect::Update_Receive()
	{
	}

	void CEventSelect::Accept_NewSession()
	{
	}

	void CEventSelect::Update_Send()
	{
	}
}