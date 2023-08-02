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

		void enqueueSend( Header* _data );

	protected:
		std::atomic<bool> m_isCanSend = true; // 처음엔 일단 보낼 수 있고 발송 후에는 완료 이벤트를 받아야 초기화
		uint64_t	m_sendBufferSize	= 0;
		Buffer		m_buffer			= {};
		SOCKADDR_IN	m_info				= {};
		SOCKET		m_sock				= INVALID_SOCKET;
		WSAEVENT	m_event				= INVALID_HANDLE_VALUE;

		packetBuff	m_sendBuff			= {};
	};
}

