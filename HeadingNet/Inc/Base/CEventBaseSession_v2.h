#pragma once

namespace Heading
{
#define HEBS_OK		0x00000000
#define HEBS_READ	0x00000010
#define HEBS_WRITE	0x00001000
#define HEBS_ERROR	0x00100000

	enum E_SELECT_EVENT_TYPE
	{
		E_SELECT_EVENT_TYPE_READ,
		E_SELECT_EVENT_TYPE_WRITE,
		E_SELECT_EVENT_TYPE_CONNECT,
		E_SELECT_EVENT_TYPE_MAX
	};

	// Session은 그저 호출만 당할 뿐
	// 유저 상태를 관리하는 객체
	class CEventBaseSession_v2
	{
	public:
		CEventBaseSession_v2	( CSimpleSocket* _sock ); // Accept 된 대상으로 Session을 만드니까 socket 을 받아서 생성합니다.
		~CEventBaseSession_v2	( );

		void operator()( CSimpleSocket* _sock );

	private:
		CSimpleState	m_state			= {};
		WSAEVENT		m_event			= INVALID_HANDLE_VALUE;

		// 연결에 문제가 생기면 접속정보를 사용해서 다시 Accept 되길 기다리고
		// 다시 Accept되면 연속해서 살려줍니다
		// 혹시 Timeout 되면 Session도 날려버립니다.
		CSimpleSocket*	m_sock			= nullptr;

		uint64_t		m_sessionKey	= 0;
		uint64_t		m_userKey		= 0;
		uint64_t		m_connectKey	= 0;
	};
}

