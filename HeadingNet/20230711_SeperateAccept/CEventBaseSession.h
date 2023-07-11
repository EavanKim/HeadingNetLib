#pragma once
// 일단 생성 복사는 기본 세팅을 사용합니다.

namespace Heading
{
	class CEventBaseSession
	{
	public:
		virtual ~CEventBaseSession( );

		void Release( );

		SOCKET		Get_Sock();
		WSAEVENT	Get_Event();

	protected:
		SOCKADDR_IN	m_info	= {};
		SOCKET		m_sock	= INVALID_SOCKET;
		WSAEVENT	m_event = INVALID_HANDLE_VALUE;
	};
}

