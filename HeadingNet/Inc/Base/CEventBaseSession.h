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
		void onEventSend();
		int SendData( );
		int InternalSendData( Header* _data );

		bool CheckLive();

		void enqueueSend( Header* _data );

		void trySend( Header* _data );

	protected:
		std::atomic<bool> m_sending		= false;
		std::atomic<bool> m_isCanSend	= true;
		uint64_t	m_sendBufferSize	= 0;
		Buffer		m_buffer			= {};
		SOCKADDR_IN	m_info				= {};
		SOCKET		m_sock				= INVALID_SOCKET;
		WSAEVENT	m_event				= INVALID_HANDLE_VALUE;

		bool		m_isConnected		= true;

		packetBuff	m_sendBuff			= {};
	};
}

