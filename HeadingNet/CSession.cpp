#include "pch.h"
#include "CSession.h"

namespace Heading
{
	CSession::CSession()
	{

	}

	CSession::~CSession()
	{

	}

	void CSession::SetNewSession( uint64_t _key, SOCKET _sock )
	{
		m_key = _key;
		m_sock = _sock;

		int length = sizeof( m_info );
		int result = getsockname( m_sock, ( struct sockaddr* )&m_info, &length );
	}

	int CSession::RecvData()
	{
		int result = 0;

		char* buffer = nullptr;
		uint64_t length = 0;
		m_recvBuff.get_buffer( &buffer, &length );


		result = ::recv( m_sock, buffer, length, 0 );
		if( -1 == result )
		{
			if( INVALID_SOCKET != m_sock )
				closesocket( m_sock );

			m_sock = INVALID_SOCKET;

			return 0;
		}

		m_recvBuff.commit( result );
		m_recvBuff.get_data( &m_recvPacketBuff );

		return result;
	}

	int CSession::SendData()
	{
		int result = 0;

		for( Header* packet : m_sendPacketBuff )
		{
			::send( m_sock, ( char* )packet, packet->length, 0 );
			result += packet->length;
			delete packet;
		}

		m_sendPacketBuff.clear();

		return result;
	}
}
