#include "pch.h"
#include "CChatSession.h"

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
		char* buffer = nullptr;
		int length = 0;
		m_buffer.get_buffer( &buffer, &length );

		m_buffer.commit( ::recv( m_sock, buffer, length, 0 ) );

		packetBuff datas = {};
		m_buffer.get_data( &datas );
		for( Header* debugsession : datas )
		{
			SendStruct<1, 1>* sendSt = (SendStruct<1, 1>*)debugsession;

			printf( "|DEBUG|SHOW|CHATBUFFER| %s", sendSt->buffer );
			m_sendBuff.push_back( debugsession );
		}

		datas.clear();
	}

	void CChatSession::GetChatData( packetBuff& _datas )
	{
		m_sendBuff.swap( _datas );
	}

	void CChatSession::SetChatData( CChatBuffer& _datas )
	{
		
	}
}