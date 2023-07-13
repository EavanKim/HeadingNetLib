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
	}
}