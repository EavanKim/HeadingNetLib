#pragma once

namespace Heading
{
	// 기본 define 정수는 int32 타입이므로 4byte 플래그를 기본으로 합니다
	// 앞 2byte는 살아있는 상태 관련으로
	// 뒷 2byte는 죽을 수 있거나 죽은 상태 관련으로 씁니다.
	// 그래서 flag && 0xFFFF 했을 때 뭔가 있으면 조건 체크가 필요합니다.
#define HS_OK			0x00000000 // Heading Socket Idle
#define HS_ERROR		0x00000001 // 에러상황
#define HS_SOCKETDOWN	0x00008000 // 소켓이 죽어서 새로 할당이 필요한 상황
#define HS_WOULDBLOCK	0x00010000 // 욷으블럭
#define HS_CLOSETROUGH	0x00100000 // 일부러 소켓을 닫지 않음
#define HS_CONNECT		0x08000000 // 가끔 최상단 비트는, 특히 마지막 2개 비트는 signed 유무로 고장날 때가 있어서 무시합니다.

	class CSimpleSocket;
	enum E_SOCKET_CALLBACK_TYPE
	{
		E_SOCKET_CALLBACK_ERROR,
		E_SOCKET_CALLBACK_RECEIVE,
		E_SOCKET_CALLBACK_SEND,
		E_SOCKET_CALLBACK_MAX
	};

	// 연결을 관리하는 객체
	class CSimpleSocket : public CHeadingObject
	{
	public:
		CSimpleSocket		( SOCKET _sock );
		~CSimpleSocket		( );

		void setNoDelay		( bool _onoff = false );
		void setLINGLE		( bool _state = false
							, u_short _timeout = 0 );

		// 콜백을 지울 땐 타입만 넣으면 됩니다,
		void setCallback	( E_SOCKET_CALLBACK_TYPE _type
							, fnCallback _fn = nullptr
							, void* _reserveParam = nullptr );

		// 6만 byte 전송은 선 넘는 일이니까 16으로 충분하리라 생각합니다.
		void send			( char* _data
							, uint16_t _length);
		void recv			( Buffer& _buffer);

	public:
		CSimpleState	m_state		= {};

		// 소켓을 꺼낼 필요가 생겼는데 소캣 래퍼에서 소켓을 꺼낼 거면
		// 어차피 private는 의미 없는 것 같아서 protected화
	protected:
		CSimpleCallback	onError;
		// 비동기 위치에서 리시브 하게 되면 데이터가 완성 된 순간의 콜백을 받도록 합니다.
		CSimpleCallback	onReceive;
		CSimpleCallback	onSend;

		Buffer			m_recvBuffer;

		SOCKET			m_sock		= INVALID_SOCKET;
	};
}

