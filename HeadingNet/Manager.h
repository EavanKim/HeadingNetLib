#pragma once

namespace Heading
{
	namespace SimpleServerKit
	{
		typedef std::unordered_map<WSAEVENT, CEventBaseSession*> sessionMap;
		typedef std::unordered_map<uint64_t, CSelecter*> selectMap;

		class Manager
		{
		public:
			static void Init();
			static Manager* Get();
			void Dispose( );

			void Start( uint16_t _port );
			void End( uint16_t _port );

			void Update();

			// Thread에서 Accept되면 Manager로 빼면서 SelectMap에 배치하기.
			// SelectMap에 사이즈 관리 추가하고 연결이 끊기면 여분자리에 새 연결 받기
			// 모자라면 새 Select 추가.
			// Accept가 굳이 메니저에 있을 필요가 있을까...?
			// 그냥 Accept를 로컬에 선언해서 써버릴까...
			static int Accept( void* _ptr );

		private:
			Manager();
			~Manager();

			static Manager* m_instance;

			volatile LONG64 m_managerWork = 0;

			concurrency::concurrent_queue<SessionData*> m_recvQueue;
			concurrency::concurrent_queue<SessionData*> m_sendQueue;

			std::vector<CSubProcess*> m_process = {};

			WSADATA m_data = {};
			sessionMap m_sessions = {};
			CAccept_Mgr m_login = {};
			selectMap m_zone = {};
		};
	}
}

