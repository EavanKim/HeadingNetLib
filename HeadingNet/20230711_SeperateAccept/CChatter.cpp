#include "pch.h"

namespace Heading
{
	CChatter::CChatter( )
	{

	}

	CChatter::~CChatter( )
	{

	}

	void CChatter::Set_NewSession( NewSocketList& _newSocket )
	{
		for( CreatedSocketInfo& info : _newSocket )
		{
			CClientSession* newSession = new CChatSession( info.Sock );
			newSession->CreateAndSetEvent( (long)(FD_READ | FD_WRITE | FD_CLOSE) );
			m_clients.insert( std::make_pair( newSession->Get_Event( ), newSession ) );
		}
	}

	void CChatter::Do_Select( )
	{

	}
}