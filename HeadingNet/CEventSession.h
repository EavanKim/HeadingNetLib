#pragma once
namespace Heading
{
	class CEventSession
	{
	public:
		CEventSession( );
		~CEventSession( );

		// 여기에 넘긴 소켓은 그 순간부터 Session에서 관리권을 취득 한 것으로 합니다.
		void BindEvent( SOCKET _sock );
		void ReleaseEvent( );

		// 단순 선형 업데이트
		void Update( LPWSAOVERLAPPED _overraped );

		// Event 단위로 관리하는 단일 Thread 할당된 Update일 경우
		int Update( void* _ptr );

	private:
		static void WSARecvCallback( IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags);
		static void WSASendCallback( IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags);
		void WSARecvData( LPWSAOVERLAPPED _overraped );
		void WSASendData( LPWSAOVERLAPPED _overraped );
		void RecvData();
		void SendData();


		volatile	LONG64				m_threadAlive	= 0;
					WSAEVENT			m_event			= INVALID_HANDLE_VALUE;
					SOCKET				m_sock			= INVALID_SOCKET;
					SOCKADDR_IN			m_info			= {};
					WSANETWORKEVENTS	m_eventStruct	= {};
					Buffer				m_socketBuffer	= {};
					packetBuff			m_recvPackets	= {};
					packetBuff			m_sendPackets	= {};
	};

	typedef std::unordered_map<SOCKET, CEventSession*> ActiveEventSessionMap;
	typedef std::vector<CEventSession*> FreeEventSessionList;
}

