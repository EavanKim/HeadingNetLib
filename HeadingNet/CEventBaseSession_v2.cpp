#include "pch.h"

namespace Heading
{
	CEventBaseSession_v2::CEventBaseSession_v2( CSimpleSocket* _sock )
		: m_sock( _sock )
	{
	}

	CEventBaseSession_v2::~CEventBaseSession_v2( )
	{
		clear( );
	}

	void CEventBaseSession_v2::operator()( CSimpleSocket* _sock )
	{
		m_sock = _sock;
	}

	void CEventBaseSession_v2::StateCheck( )
	{
		if( m_state.checkState( HEBS_ERROR ) )
		{
			// 에러가 났다면 다 날려버립니다.
			if( INVALID_HANDLE_VALUE != m_event )
			{
				WSACloseEvent( m_event );
				m_event = INVALID_HANDLE_VALUE;
			}

			// 소켓쪽 삭제 관련 유무는 소켓 상태로 갈릴 것으로 보고 일단 지웁니다.
			if( nullptr != m_sock )
			{
				delete m_sock;
				m_sock = nullptr;
			}
		}
	}

	WSAEVENT CEventBaseSession_v2::getEvent( )
	{
		return m_event;
	}

	CSimpleSocket* CEventBaseSession_v2::getSocket( )
	{
		return m_sock;
	}

	void CEventBaseSession_v2::clear( )
	{
		// Event 자체는 소켓 상태와 무관하게 삭제 가능한 것으로 판정합니다.
		if( INVALID_HANDLE_VALUE != m_event )
		{
			WSACloseEvent( m_event );
			m_event = INVALID_HANDLE_VALUE;
		}

		if( nullptr != m_sock )
		{
			delete m_sock;
			m_sock = nullptr;
		}
	}
}