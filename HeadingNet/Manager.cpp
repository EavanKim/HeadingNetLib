#include "pch.h"
#include "Manager.h"

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

		}

		Manager::Manager( )
		{
			WSAStartup( MAKEWORD(2,2), &m_data );
		}

		Manager::~Manager( )
		{
			WSACleanup();
		}
	}
}
