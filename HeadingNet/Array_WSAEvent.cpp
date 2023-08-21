#include "pch.h"

namespace Heading
{
	bool Array_WSAEvent::add( WSAEVENT _newEvent )
	{
		if( WSA_MAXIMUM_WAIT_EVENTS > m_size )
		{
			m_events[m_size] = _newEvent;
			++m_size;
			return true;
		}

		return false;
	}

	void Array_WSAEvent::remove( WSAEVENT _delete )
	{
		for( uint8_t seek = 0; m_size > seek; ++seek )
		{
			WSAEVENT CurrentEvent = m_events[ seek ];
			if( CurrentEvent == _delete )
			{
				if( seek != ( m_size - 1 ) )
				{
					m_events[ seek ] = m_events[ m_size - 1 ];
				}

				WSACloseEvent( CurrentEvent );
				--m_size;
				return;
			}
		}
	}

	WSAEVENT Array_WSAEvent::swap(int _index, WSAEVENT _newTarget)
	{
		WSAEVENT result = m_events[_index];
		m_events[_index] = _newTarget;

		return result;
	}

	void Array_WSAEvent::clear( )
	{
		ZeroMemory(m_events, (sizeof(WSAEVENT) * WSA_MAXIMUM_WAIT_EVENTS));
	}

	uint8_t Array_WSAEvent::size()
	{
		return m_size;
	}

	WSAEVENT* Array_WSAEvent::operator*( )
	{
		return m_events;
	}

	WSAEVENT Array_WSAEvent::operator[]( DWORD _index )
	{
		return m_events[_index];
	}
}
