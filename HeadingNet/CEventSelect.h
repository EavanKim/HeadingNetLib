#pragma once
//==================================================================================================================================================================
// 
// Event는 Multiple 대기이지만, microthread 처리하듯이 한 번에 하나만 처리됩니다.
// 이벤트가 많이 누적되어서 lost 되는 경우에 대비한 예방책을 준비하세요
// Ex) 10개의 소켓이 있을 때
// 0~5번만 오지게 처리하고
// 6~9번은 인덱스 문제로 아무것도 처리되지 않는 문제가 발생하지 않도록 처리 할 필요가 있습니다.
// 완전하게 막으려면 GameThread 형식의 마이크로스레드가 아니라, 개별 이벤트당 스레드 처리가 맞겠지만
// 머리를 써보시오...
// 설마 그러겠어 하고 해맸던 것은 삽질이었다는 엔딩...
// 
//==================================================================================================================================================================

namespace Heading
{
	class CEventSelect
	{
	public:
		CEventSelect( uint16_t _port );
		~CEventSelect();

		void SetupAddrInfo();
		bool bind();
		void Do_Select();

	private:
		void Update_Receive();
		void Accept_NewSession( long _NetworkEventType );
		void Update_Send();

		uint16_t				m_port				= 0;
		SOCKET					m_sock				= INVALID_SOCKET;
		SOCKADDR_IN				m_info				= {};
		DWORD					m_eventCount		= {};
		DWORD					m_bytesTransferred	= {};
		WSAEVENT				m_connectEvent		= {};
		WSAOVERLAPPED			m_acceptOverlapped	= {};
		std::vector<WSAEVENT>	m_selectEvent		= {};
		ActiveEventSessionMap	m_sessionMap		= {};
		FreeEventSessionList	m_freeList			= {};
	};
}

