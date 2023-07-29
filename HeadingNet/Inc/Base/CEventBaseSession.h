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

		int RecvData( packetBuff& _datas );
		int SendData( );

		void enqueueSend( Header* _data );

	protected:
		Buffer		m_buffer	= {};
		SOCKADDR_IN	m_info		= {};
		SOCKET		m_sock		= INVALID_SOCKET;
		WSAEVENT	m_event		= INVALID_HANDLE_VALUE;

		packetBuff	m_sendBuff	= {};
	};
}

