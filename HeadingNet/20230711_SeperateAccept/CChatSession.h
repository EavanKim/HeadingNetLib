#pragma once

namespace Heading
{
	class CChatSession : public CClientSession
	{
	public:
		CChatSession( SOCKET _sock );
		virtual ~CChatSession();
		
		virtual void Update( ) override;
		void GetChatData( packetBuff& _datas );
		void SetChatData( CChatBuffer& _datas );
	};
}

