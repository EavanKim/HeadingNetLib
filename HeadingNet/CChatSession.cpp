#include "pch.h"

namespace Heading
{
	CChatSession::CChatSession( SOCKET _sock )
		: CClientSession( _sock )
	{
	}

	CChatSession::~CChatSession( )
	{
	}

	void CChatSession::Update( )
	{
		WSANETWORKEVENTS NetworkEvents;
		int retval = WSAEnumNetworkEvents( m_sock, m_event, &NetworkEvents);
		if (retval == SOCKET_ERROR) 
			return;

		if( NetworkEvents.lNetworkEvents & FD_READ )
		{
			if (NetworkEvents.lNetworkEvents & FD_READ
				&& NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				return;
			}
			RecvData();
		}

		if( NetworkEvents.lNetworkEvents & FD_WRITE )
		{
			if (NetworkEvents.lNetworkEvents & FD_WRITE
				&& NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				return;
			}

			SendData();
		}

		if( NetworkEvents.lNetworkEvents & FD_CLOSE )
			return;
	}

	void CChatSession::GetChatData( packetBuff& _datas )
	{
		m_recvBuff.swap( _datas );
	}

	void CChatSession::SetChatData( packetBuff& _datas )
	{
		m_sendBuff.swap( _datas );
	}
}