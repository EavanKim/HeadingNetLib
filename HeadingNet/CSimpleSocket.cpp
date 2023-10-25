#include "pch.h"

namespace Heading
{
	CSimpleSocket::CSimpleSocket( SOCKET _sock )
		: CHeadingObject(true)
		, m_sock( _sock )
	{

	}

	CSimpleSocket::~CSimpleSocket( )
	{
		if( INVALID_SOCKET != m_sock )
		{
			// 에러 상태면 socket 삭제 패스
			// 혹은 일부러 삭제를 피하겠다고 하면 패스
			if( !m_state.haveState( HS_ERROR | HS_CLOSETROUGH ) )
				closesocket( m_sock );
			m_sock = INVALID_SOCKET;
		}
	}

	void CSimpleSocket::setNoDelay( bool _onoff )
	{
		int on = 0;

		if( _onoff )
		{
			on = 1;
		}

		setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof( on ) );
	}

	void CSimpleSocket::setLINGLE( bool _state, u_short _timeout )
	{
		// 종료 관련 버퍼 처리 - LINGLE
		LINGER  ling = {0,};
		if( _state )
			ling.l_onoff = 1;
		else
			ling.l_onoff = 0;

		ling.l_linger = _timeout;

		setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (CHAR*)&ling, sizeof(ling));
	}

	void CSimpleSocket::setCallback( E_SOCKET_CALLBACK_TYPE _type, fnCallback _fn, void* _reserveParam /*= nullptr*/ )
	{
		switch( _type )
		{
			case E_SOCKET_CALLBACK_TYPE::E_SOCKET_CALLBACK_ERROR:
				onError(_fn, _reserveParam);
				break;
			case E_SOCKET_CALLBACK_TYPE::E_SOCKET_CALLBACK_RECEIVE:
				onReceive(_fn, _reserveParam);
				break;
			case E_SOCKET_CALLBACK_TYPE::E_SOCKET_CALLBACK_SEND:
				onSend(_fn, _reserveParam );
				break;
			default:
				break;
		}
	}

	void CSimpleSocket::send( char* _data, uint16_t _length )
	{
		int sendResult = ::send( m_sock, _data, _length, 0 );

		if( SOCKET_ERROR == sendResult )
		{
			m_state.setState( true, HS_ERROR );
			if( WSAEWOULDBLOCK == WSAGetLastError( ) )
				m_state.setState( true, HS_WOULDBLOCK );

			onError( this );

			return;
		}

		if( 0 == sendResult )
		{
			// 발송 실패 == terminate
			m_state.setState(true, HS_SOCKETDOWN);
			onError( this );

			return;
		}

		onSend( this );
	}

	void CSimpleSocket::recv( Buffer& _buffer )
	{
		char* dataPtr = nullptr;
		int length = 0;
		if( _buffer.get_buffer( &dataPtr, &length ) )
		{
			int recvResult = ::recv( m_sock, dataPtr, length, 0 );

			if( SOCKET_ERROR == recvResult )
			{
				// Error Flag 설정하기
				m_state.setState(true, HS_ERROR);
				onError( this );

				return;
			}

			if( 0 == recvResult )
			{
				// Error Flag 설정하기
				// size 0을 연결 확인으로 사용하는 경우도 있다고 하시니 넘겨봅니다.
				return;
			}

			onReceive( this );
		}
	}
}
