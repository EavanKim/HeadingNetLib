#include "pch.h"

namespace Heading
{
	CBroadCaster::CBroadCaster( )
	{

	}

	CBroadCaster::~CBroadCaster( )
	{

	}

	void CBroadCaster::Set_NewSession( NewSocketList& _newSocket )
	{
		for( CreatedSocketInfo& info : _newSocket )
		{
			switch( info.AcceptPort )
			{
			case 51000:
				{
					CClientSession* newSession = new CBroadCastSession( info.Sock );
					newSession->CreateAndSetEvent( ( long ) ( FD_WRITE | FD_CLOSE ) );
					m_events[ m_size ] = newSession->Get_Event( );
					m_sessions.insert( std::make_pair( newSession->Get_Event( ), newSession ) );
					++m_size;
				}
				break;
			}
		}
	}
}
