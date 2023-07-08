#pragma once

namespace Heading
{
	class CSelect
	{
	public:
		CSelect( uint16_t _port );
		~CSelect();

		void SetupAddrInfo();
		bool bind();
		void setupSelect( bool _read = false, bool _write = false );
		void Do_Select();

	private:
		void Update_Receive();
		void Accept_NewSession();
		void Update_Send();

		bool		readSelect		= false;
		bool		writeSelect		= false;

		uint16_t	m_port			= 0;
		SOCKET		m_sock			= INVALID_SOCKET;
		SOCKADDR_IN m_info			= {};
		fd_set		m_socketSet		= {};
		fd_set		m_readSet		= {};
		fd_set		m_writeSet		= {};
		SessionMap	m_sessionMap	= {};
	};
}

