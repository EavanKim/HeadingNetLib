#pragma once

namespace Heading
{
	class CEventSession
	{
	public:
		CEventSession( );
		~CEventSession( );

		// 여기에 넘긴 소켓은 그 순간부터 Session에서 관리권을 취득 한 것으로 합니다.
		void			BindEvent			( SOCKET				_sock
											, long					_NetworkEventType			);
		void			ReleaseEvent		(													);
		void			SetUpSessionType	( bool					_read				= false
											, bool					_write				= false
											, bool					_accept				= false );

		CEventSession*	Accept				( long					_NetworkEventType			);

		void			EnumNetworkEvents	(													);		
		void			RecvData			(													);
		void			SendData			(													);

		// 기본 기능
		volatile	LONG64				m_threadAlive		= 0;
					long				m_sessionTypeFlag	= 0;
					WSAEVENT			m_event				= INVALID_HANDLE_VALUE;
					SOCKET				m_session			= INVALID_SOCKET;
					WSANETWORKEVENTS	m_sessionEvents		= {};
					SOCKADDR_IN			m_info				= {};

					// 기존 내용
					Buffer				m_socketBuffer		= {};
					packetBuff			m_recvPackets		= {};
					packetBuff			m_sendPackets		= {};
	};

	typedef std::unordered_map<WSAEVENT, CEventSession*>	ActiveEventSessionMap;
	typedef std::vector<CEventSession*>						FreeEventSessionList;
}

