#include "pch.h"

namespace Heading
{
	bool Array_WSAEvent::Add( WSAEVENT _newEvent )
	{
		if( WSA_MAXIMUM_WAIT_EVENTS > m_size )
		{
			m_events[m_size] = _newEvent;
			++m_size;
			return true;
		}

		return false;
	}

	void Array_WSAEvent::Remove( WSAEVENT _delete )
	{
		// 단순하고 무식하게 써리 원
		// Event를 키로 하여 검색해서 꺼내쓰기 때문에 순서가 관계 없어서 가능한 단순처리
		for( uint8_t seek = 0; m_size > seek; ++seek )
		{
			WSAEVENT CurrentEvent = m_events[ seek ];
			// 일치하면 지우고 끝장을 본 뒤 탈출
			if( CurrentEvent == _delete )
			{
				// 기본적으로 seek가 마지막이 아니라면
				if( seek != ( m_size - 1 ) )
				{
					// 마지막 값을 위치를 옮겨 덮어버립니다.
					m_events[ seek ] = m_events[ m_size - 1 ];
				}
				// seek가 마지막 인덱스인 size - 1이라면, 사이즈를 감소시켜서 다시 못 쓰게 만들고 덮어쓰기를 안합니다.

				WSACloseEvent( CurrentEvent );
				// 지우면 사이즈가 하나 감소
				--m_size;
				return;
			}
		}
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
