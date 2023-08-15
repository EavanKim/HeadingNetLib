#include "pch.h"

namespace Heading
{
	CSimpleSocket::CSimpleSocket( SOCKET _sock )
		: m_sock( _sock )
	{

	}

	CSimpleSocket::~CSimpleSocket( )
	{
		if( INVALID_SOCKET != m_sock )
		{
			// 에러 상태면 socket 삭제 패스
			if( !m_state.checkState( HS_ERROR ) )
				closesocket( m_sock );
			m_sock = INVALID_SOCKET;
		}
	}

	void CSimpleSocket::setLINGLE( bool _state )
	{
		// 종료 관련 버퍼 처리 - LINGLE
		_Unreferenced_parameter_( _state );
	}

	void CSimpleSocket::setCallback( E_SOCKET_CALLBACK_TYPE _type, fnCallback _fn, void* _reserveParam = nullptr )
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
				// 이건 어떻게 처리해야하나...
				// size 0을 연결 확인으로 사용하는 경우도 있다고 하시니 넘겨봅니다.
				return;
			}

			onReceive( this );
		}
	}
}
