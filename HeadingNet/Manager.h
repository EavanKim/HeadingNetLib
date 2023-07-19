#pragma once

namespace Heading
{
	namespace SimpleServerKit
	{
		typedef std::unordered_map<WSAEVENT, CEventBaseSession> sessionMap;
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

		private:
			Manager();
			~Manager();

			static Manager* m_instance;

			WSADATA m_data = {};
			sessionMap m_sessions;
			CAccept_Mgr m_login;
			selectMap m_zone;
		};
	}
}

