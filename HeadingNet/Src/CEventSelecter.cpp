#include "pch.h"

namespace Heading
{
	CEventSelecter::CEventSelecter( )
	{
	}

	CEventSelecter::~CEventSelecter( )
	{
	}

	bool CEventSelecter::Set_NewEvents( WSAEVENT _newEventHandle )
	{
		if( WSA_MAXIMUM_WAIT_EVENTS != m_size )
		{
			m_events[m_size] = _newEventHandle;
			++m_size;
			return true;
		}

		return false;
	}

	void CEventSelecter::Set_RemoveEvents( WSAEVENT _newEventHandle )
	{
		for( uint8_t seek = 0; WSA_MAXIMUM_WAIT_EVENTS > seek; ++seek )
		{
			if( m_events[ seek ] == _newEventHandle )
			{
				--m_size;
				m_events[ seek ] = m_events[ m_size ];
				return;
			}
		}
	}

	void CEventSelecter::Do_Update( ChatSessionEventMap& _sessionMap )
	{
		DWORD ret = WSAWaitForMultipleEvents( m_size, m_events, FALSE, 0, TRUE );

		switch( ret )
		{
		case WSA_WAIT_IO_COMPLETION:
		case WSA_WAIT_TIMEOUT:
		case -1:
			return;
		default:
			break;
		}

		// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents
		// https://www.joinc.co.kr/w/man/4100/WASWaitForMultipleEvents 
		for( INT seek = ret - WSA_WAIT_EVENT_0; WSA_MAXIMUM_WAIT_EVENTS > seek; ++seek )
		{
			// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
			DWORD EventResult = WaitForSingleObject( m_events[ seek ], 0 );
			switch( EventResult )
			{
			case WAIT_ABANDONED:
			case WAIT_OBJECT_0:
			case WAIT_TIMEOUT:
			case WAIT_FAILED:
				continue;
			default:
				{
					ChatSessionEventMap::iterator iter = _sessionMap.find( m_events[ seek ] );
					if( _sessionMap.end() != iter )
					{
						iter->second->Update();
					}
				}
				break;
			}

			// WaitForSingleObject ??? ??? ???????? ????? ???? ???????!
			// ?? ?????? ???? ??!
			WSAResetEvent( m_events[ seek ] );
		}
	}
}
