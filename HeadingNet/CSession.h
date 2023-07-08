#pragma once

namespace Heading
{
	class CSession
	{
	public:
		CSession();
		~CSession();

		void SetNewSession( uint64_t _key, SOCKET _sock );
		int RecvData();
		int SendData();

	private:
		uint64_t		m_key				= 0;
		SOCKET			m_sock				= INVALID_SOCKET;
		SOCKADDR_IN		m_info				= {};
		Buffer			m_recvBuff			= {};
		packetBuff		m_recvPacketBuff	= {};
		packetBuff		m_sendPacketBuff	= {};
	};

	typedef std::unordered_map<SOCKET, CSession> CSessionMap;
}
