#pragma once

namespace Heading
{
	class CEventBaseSession : public CHeadingObject
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
		int InternalSendData( packetHeader_t* _data );

		bool CheckLive();

		void enqueueSend( packetHeader_t* _data );

		void trySend( packetHeader_t* _data );

	protected:
		std::atomic<bool> m_sending		= false;
		uint64_t	m_sendBufferSize	= 0;
		Buffer		m_buffer			= {};
		SOCKADDR_IN	m_info				= {};
		SOCKET		m_sock				= INVALID_SOCKET;
		WSAEVENT	m_event				= INVALID_HANDLE_VALUE;

		bool		m_isConnected		= true;

		packetBuff	m_sendBuff			= {};
	};
}

