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
				return false;
			}
		}
		while( S_OK != returnValue );

		m_connectEvent = WSACreateEvent();
		WSAEventSelect( m_sock, m_connectEvent, FD_ACCEPT | FD_CLOSE );
		if( SOCKET_ERROR == listen( m_sock, 5 ) )
			return false;

		return true;
	}

	void CEventSelect::Do_Select()
	{
		DWORD index = WSAWaitForMultipleEvents( (DWORD)m_selectEvent.size(), m_selectEvent.data(), false, 0, false);
				
		bool result = WSAResetEvent(m_selectEvent[index - WSA_WAIT_EVENT_0]);

	}

	void CEventSelect::Update_Receive()
	{
		ActiveEventSessionMap::iterator iter = m_sessionMap.begin();
		for(;m_sessionMap.end() != iter; ++iter)
		{
			iter->second->RecvData();
		}
	}

	void CEventSelect::Accept_NewSession( long _NetworkEventType )
	{
		CEventSession* newSession = nullptr;
		if( m_freeList.size() )
		{
			newSession = m_freeList.back();
			m_freeList.pop_back();
		}
		else
		{
			newSession = new CEventSession();
		}

		newSession->Accept(_NetworkEventType);
	}

	void CEventSelect::Update_Send()
	{
		ActiveEventSessionMap::iterator iter = m_sessionMap.begin();
		for(;m_sessionMap.end() != iter; ++iter)
		{
			iter->second->SendData();
		}
	}
}