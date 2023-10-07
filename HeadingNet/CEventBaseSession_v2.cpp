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

	void CEventBaseSession_v2::trySend(packetHeader_t* _send)
	{
		// 어느것이 전송의 끝일까...?
		if ( NULL != _send )
		{
			// 쓰기 상태가 아니라면
			if ( !m_state.checkState( HEBS_WRITE ) )
			{
				m_state.setState(true, HEBS_WRITE);
				m_sock->send(( char* ) _send, _send->length);
				m_state.setState(false, HEBS_WRITE);
				return;
			}
			else
			{
				m_sendQueue.push(_send);
			}
		}
		else
		{
			if ( !m_sendQueue.empty() )
			{
				packetHeader_t* sending = m_sendQueue.front();
				m_state.setState(true, HEBS_WRITE);
				// 필요하면 실패한 경우 pop 제외 로직 추가
				m_sock->send(( char* ) sending, sending->length);
				m_state.setState(false, HEBS_WRITE);
				m_sendQueue.pop();
			}
		}
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