#include "pch.h"

namespace Heading
{
	CSelecter::CSelecter( )
	{
		InterlockedExchange64(&m_bSessionLive, 1);
	}

	CSelecter::~CSelecter( )
	{
	}

	bool CSelecter::Check_SessionCapacity()
	{
		return WSA_MAXIMUM_WAIT_EVENTS > m_size;
	}

	bool CSelecter::Set_NewSession( Heading::CClientSession* _newSocket )
	{
		if( Check_SessionCapacity() )
		{
			++m_size;
			m_pendingSession.push( _newSocket );
			return true;
		}

		return false;
	}

	int CSelecter::Do_Select( void* _ptr )
	{
		while( 1 == InterlockedCompareExchange64(&m_bSessionLive, 0, 0) )
		{
			// 일단 이쪽 정책을 잘 모르니 무슨 일이 있어도 1초 이후엔 처리 가능하게 지나갑니다.
			DWORD ret = WSAWaitForMultipleEvents( ( DWORD ) m_eventSize, m_events, FALSE, 1, TRUE );

			switch( ret )
			{
			case WSA_WAIT_FAILED:
			case WSA_WAIT_IO_COMPLETION:
				continue;
			case WSA_WAIT_TIMEOUT:
			{
				Do_FlushPendingQueue( );
			}
			continue;
			default:
				break;
			}

			// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents
			// https://www.joinc.co.kr/w/man/4100/WASWaitForMultipleEvents 
			ChatSessionEventMap::iterator iter = m_sessions.find( m_events[ ret - WSA_WAIT_EVENT_0 ] );
			if( m_sessions.end( ) != iter )
			{
				iter->second->Update( );				
			}

			WSAResetEvent( m_events[ ret - WSA_WAIT_EVENT_0 ] );

			Do_FlushPendingQueue( );
		}

		return 0;
	}

	void CSelecter::Do_FlushPendingQueue( )
	{

		while( !m_pendingSession.empty( ) )
		{
			Heading::CClientSession* session = nullptr;
			if( m_pendingSession.try_pop( session ) )
			{
				m_events[ m_eventSize ] = session->Get_Event( );
				m_sessions.insert( std::make_pair( m_events[ m_eventSize ], session ) );
				++m_eventSize;
			}
		}
	}

	void CSelecter::Stop( )
	{
		InterlockedExchange64(&m_bSessionLive, 0);
	}
}
