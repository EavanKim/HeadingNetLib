#pragma once

namespace Heading
{
	class CEventBaseSession
	{
	public:
		CEventBaseSession( SOCKET _sock );
		virtual ~CEventBaseSession( );
		void CreateAndSetEvent( long _eventFlag );

		void Release( );

		SOCKET		Get_Sock();
		WSAEVENT	Get_Event();

		int RecvData();
		int SendData();

	protected:
		Buffer		m_buffer	= {};
		SOCKADDR_IN	m_info		= {};
		SOCKET		m_sock		= INVALID_SOCKET;
		WSAEVENT	m_event		= INVALID_HANDLE_VALUE;
		packetBuff	m_recvBuff	= {};
		packetBuff	m_sendBuff	= {};
	};
}

