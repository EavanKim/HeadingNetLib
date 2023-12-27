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

	class CSocket_Stream : public ISocket
	{
	public:
		explicit CSocket_Stream(connectionInfo& _info);
		explicit CSocket_Stream(SOCKET _sock);
		virtual ~CSocket_Stream();

		// Stream(TCP) 소켓용 설정
		void setNoDelay(bool _onoff = false);
		void setLINGLE(bool _state = false, u_short _timeout = 0);

		// ISocket을(를) 통해 상속됨
		const SOCKADDR_IN* GetInfo() override;
		bool Send(IMessage* _send) override;
		void Recv(IMessage* _recv) override;

	public:
		CSimpleState	m_state = {};

	private:

		Buffer			m_recvBuffer;

		SOCKET			m_sock = INVALID_SOCKET;
	};
}

