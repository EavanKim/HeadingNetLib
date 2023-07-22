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

				for( selectMap::iterator iter = m_zone.begin( ); m_zone.end( ) != iter; ++iter )
				{
					iter->second->Do_Select( nullptr );
				}

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
			Manager* mgr = static_cast<Manager*>( _ptr );

			CAccept_Mgr login;

			while( 1 )
			{
				if( login.Do_Select( ) )
				{
					mgr->server_log( E_LOG_LEVEL_DEBUG, "Accept Someting" );
					NewSocketList list = {};

					login.Get_NewSocket( list );

					for( CreatedSocketInfo info : list )
					{
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

			m_state_count.m_acceptThread = 1;
			m_state_count.m_selectThread = _selectThreadCount;

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
