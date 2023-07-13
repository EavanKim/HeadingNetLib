#include "pch.h"

namespace Heading
{
	CBroadCastSession::CBroadCastSession( SOCKET _sock )
		: CClientSession( _sock )
	{
	}

	CBroadCastSession::~CBroadCastSession( )
	{
	}

	void CBroadCastSession::Update( )
	{
	}
}
