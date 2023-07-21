#include "pch.h"

namespace Heading
{
	namespace SimpleServerKit
	{
		Manager* Manager::m_instance = nullptr;

		void Manager::Init( E_LOG_LEVEL _logLevel, uint64_t _selectThreadCount )
		{
			if( nullptr == m_instance )
				m_instance = new Manager( _logLevel, _selectThreadCount );
		}

		Manager* Manager::Get( )
		{
			// 프로그램 시작과 끝에만 수정하고,
			// 나머지는 항시 유지되므로 별다른 잠금 없이 획득합니다.
			return m_instance;
		}

		void Manager::Dispose( )
		{
			if( nullptr != m_instance )
			{
				delete m_instance;
				m_instance = nullptr;
			}
		}

		void Manager::Start( uint16_t _port )
		{
			m_login.Set_NewAcceptPort( _port );
		}

		void Manager::End( uint16_t _port )
		{
			m_login.Set_CloseAcceptPort( _port );
		}

		void Manager::Update( )
		{
			while( 1 == InterlockedCompareExchange64( &m_managerWork, 0, 0 ) || !m_recvQueue.empty( ) )
			{
				server_log_flush();

				//// thread로 옮기기
				//if( m_login.Do_Select( ) )
				//{
				//	server_log( E_LOG_LEVEL_DEBUG, "Accept Someting" );
				//	NewSocketList list = {};
				//	m_login.Get_NewSocket( list );

				//	// 이 대상을 SelectThread에서 처리해야하니
				//	// 별도의 동기화 절차 하에서 socket을 받아서 selecter 내부에서 만드는 것으로 빼기.
				//	for( CreatedSocketInfo info : list )
				//	{
				//		server_log( E_LOG_LEVEL_DEBUG, "newSocket" );
				//		CEventBaseSession* session = new CEventBaseSession( info.Sock );
				//		session->CreateAndSetEvent( FD_READ | FD_WRITE );
				//		m_sessions.insert( std::make_pair( session->Get_Event( ), session ) );
				//	}
				//}

				// WaitForMultipleObject중인 Selecter Thread에 신호주기
				for( selectMap::iterator iter = m_zone.begin( ); m_zone.end( ) != iter; ++iter )
				{
					iter->second->Do_Select( nullptr );
				}

				// 무언가 Selecter Thread에서 진행되었다면
				// 아래에서 내용 처리

				while( !m_recvQueue.empty( ) )
				{
					server_log( E_LOG_LEVEL_DEBUG, "someting received" );
					SessionData* header = nullptr;
					if( m_recvQueue.try_pop( header ) )
					{
						switch( header->m_message->type )
						{
							for( CSubProcess* proc : m_process )
							{
								proc->Do_Process( header );
							}
						}
						delete header;
					}
				}

				while( !m_sendQueue.empty( ) )
				{
					server_log( E_LOG_LEVEL_DEBUG, "someting sending" );
					SessionData* header = nullptr;
					if( m_sendQueue.try_pop( header ) )
					{
						sessionMap::iterator iter = m_sessions.find(header->m_sessionKey);
						if( m_sessions.end( ) != iter )
						{
							iter->second->enqueueSend( header->m_message );
						}

						delete header;
					}
				}
			}
		}

		int Manager::Accept( void* _ptr )
		{
			// Thread Context입니다.
			// Thread Safe하지 않다면 돌리지 않는게 상책입니다.
			Manager* mgr = static_cast<Manager*>( _ptr );

			CAccept_Mgr login;

			while( 1 )
			{
				if( login.Do_Select( ) )
				{
					// log는 자료구조가 ThreadSafe 하므로 사용합니다.
					mgr->server_log( E_LOG_LEVEL_DEBUG, "Accept Someting" );
					NewSocketList list = {};

					// 굳이 리스트를 만들어서 빼는게 아니라,
					// 애당초 새로운 소켓 큐에 뺄 수 있도록 조치하기.
					login.Get_NewSocket( list );

					// 이 대상을 SelectThread에서 처리해야하니
					// 별도의 동기화 절차 하에서 socket을 받아서 selecter 내부에서 만드는 것으로 빼기.
					for( CreatedSocketInfo info : list )
					{
						// log는 자료구조가 ThreadSafe 하므로 사용합니다.
						mgr->server_log( E_LOG_LEVEL_DEBUG, "newSocket" );

						bool result = mgr->try_set_new_session( info );

						mgr->server_log(E_LOG_LEVEL::E_LOG_LEVEL_DEBUG, result ? "Accept new Socket" : "Accept Socket Closed" );
					}
				}
			}

			return 0;
		}

		void Manager::server_log( E_LOG_LEVEL _level, std::string _log )
		{
			if( m_logLevel < _level )
			{
				__time64_t currtime = time( NULL );
				tm formatTime = {};
				localtime_s( &formatTime, &currtime );
				// 이것저것 바꿔보는 포멧팅입니다.
				std::string formatString = formatf	(
														"[ %04i.%02i.%02i-%02i:%02i:%02i ] : %s"
														, formatTime.tm_year + 1900
														, formatTime.tm_mon + 1
														, formatTime.tm_wday
														, formatTime.tm_hour
														, formatTime.tm_min
														, formatTime.tm_sec
														, _log.c_str( )
													);

				m_logQueue.push( formatString );
			}
		}

		void Manager::server_log_flush( )
		{
			while( !m_logQueue.empty( ) )
			{
				std::string popResult;
				if( m_logQueue.try_pop( popResult ) )
				{
					printf(popResult.c_str());
				}
			}
		}

		bool Manager::try_set_new_session( CreatedSocketInfo& _info )
		{
			// 당분간 감소하는 위치가 없어서
			// 제한갯수만큼 연결하고나면 
			// 다시는 연결되지 않을 코드
			if( m_state_count.m_currentSession < m_state_count.m_maximumSession )
			{
				m_newSocketQueue.push( _info );
				++m_state_count.m_currentSession;
				
				return true;
			}
			else
			{
				closesocket( _info.Sock );
				return false;
			}
		}

		Manager::Manager( E_LOG_LEVEL _logLevel, uint64_t _selectThreadCount )
			: m_logLevel( _logLevel )
		{
			std::string str;
			WSAErrorString( WSAStartup( MAKEWORD( 2, 2 ), &m_data ), str );
			server_log( E_LOG_LEVEL::E_LOG_LEVEL_DEBUG, formatf( "Heading::string Error %s \n", str.c_str( ) ) );

			// 거의 고정값
			m_state_count.m_acceptThread = 1;
			m_state_count.m_selectThread = _selectThreadCount;
			// EventSelect는 각 Select당 64개만 커버가능
			// 나중에 Select 교체하면 해당 Select 특색에 맞게 값 변경 필요
			m_state_count.m_maximumSession = (m_state_count.m_selectThread * 64);

			InterlockedExchange64( &m_managerWork, 1 );
		}

		Manager::~Manager( )
		{
			// abort 되는 thread 만들지 않기.
			for( std::thread* thread : m_threads )
			{
				if( thread->joinable() )
					thread->join();

				// 해제되는 순간 joinable 하다면 abort 될 수 있고, 
				// abort 되면 thread 내부에서 처리하던 작업이 올바르게 끝나는 것을 보증하지 않으므로(transactional하지 않게 되므로)
				// server 에서는 무조건 join 시킨 다음 끝내기.
				// DB Fail의 경우 메모리 크래시만 아니면 SQLite를 사용해서 Schema만 유지된 임의의 파일 DB에 데이터를 뽑고
				// 기동 시 해당 파일이 있는 경우 복구하는 구조로 FailOver 구현하기.
				// SQLite 자체 메모리 할당기를 쓰면 메모리 크래시 극복되는지 테스트 해보기.
				delete thread;
			}

			for( std::pair<uint64_t, CSelecter*> iter : m_zone )
			{
				delete iter.second;
			}

			m_zone.clear( );

			for( CSubProcess* proc : m_process )
			{
				delete proc;
			}

			m_process.clear( );

			WSACleanup( );
		}
	}
}
