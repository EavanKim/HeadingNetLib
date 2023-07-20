#include "pch.h"

namespace Heading
{
	namespace SimpleServerKit
	{
		Manager* Manager::m_instance = nullptr;

		void Manager::Init( )
		{
			if( nullptr == m_instance )
				m_instance = new Manager( );
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
				// thread로 빼기
				if( m_login.Do_Select( ) )
				{
					NewSocketList list = {};
					m_login.Get_NewSocket( list );

					for( CreatedSocketInfo info : list )
					{
						CEventBaseSession* session = new CEventBaseSession( info.Sock );
						session->CreateAndSetEvent( FD_READ | FD_WRITE );
						m_sessions.insert( std::make_pair( session->Get_Event( ), session ) );
					}
				}

				for( selectMap::iterator iter = m_zone.begin( ); m_zone.end( ) != iter; ++iter )
				{
					iter->second->Do_Select( nullptr );
				}

				while( !m_recvQueue.empty( ) )
				{
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
			return 0;
		}

		Manager::Manager( )
		{
			WSAStartup( MAKEWORD( 2, 2 ), &m_data );

			InterlockedExchange64(&m_managerWork, 1 );
		}

		Manager::~Manager( )
		{
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
