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
		// �ܼ��ϰ� �����ϰ� �Ḯ ��
		// Event�� Ű�� �Ͽ� �˻��ؼ� �������� ������ ������ ���� ��� ������ �ܼ�ó��
		for( uint8_t seek = 0; m_size > seek; ++seek )
		{
			WSAEVENT CurrentEvent = m_events[ seek ];
			// ��ġ�ϸ� ����� ������ �� �� Ż��
			if( CurrentEvent == _delete )
			{
				// �⺻������ seek�� �������� �ƴ϶��
				if( seek != ( m_size - 1 ) )
				{
					// ������ ���� ��ġ�� �Ű� ��������ϴ�.
					m_events[ seek ] = m_events[ m_size - 1 ];
				}
				// seek�� ������ �ε����� size - 1�̶��, ����� ���ҽ��Ѽ� �ٽ� �� ���� ����� ����⸦ ���մϴ�.

				WSACloseEvent( CurrentEvent );
				// ����� ����� �ϳ� ����
				--m_size;
				return;
			}
		}
	}

	void Array_WSAEvent::clear( )
	{
		// Event ��ü�� �����ֱ�� m_sessions�� session�� �����ϹǷ� �ڵ鿡 ���� ��� �߰����굵 �����ϴ�.
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
