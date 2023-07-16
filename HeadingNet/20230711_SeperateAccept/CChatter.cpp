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
			switch( info.AcceptPort )
			{
			case 50000:
				{
					CClientSession* newSession = new CChatSession( info.Sock );
					newSession->CreateAndSetEvent( ( long ) ( FD_READ | FD_WRITE | FD_CLOSE ) );
					m_events[ m_size ] = newSession->Get_Event( );
					m_sessions.insert( std::make_pair( newSession->Get_Event( ), newSession ) );
					++m_size;
				}
				break;
			}
		}
	}


	void CChatter::Do_PostProcess()
	{
		ChatSessionEventMap::iterator iter = m_sessions.begin( );
		for( ; m_sessions.end( ) != iter; ++iter )
		{
			CChatSession* session = (CChatSession*)iter->second;
			packetBuff buffer;
			session->GetChatData(buffer);
			for( Header* head : buffer )
				m_chat.InsertDatas( (uint64_t)iter->first, head );
		}


		ChatSessionEventMap::iterator iter = m_sessions.begin( );
		for( ; m_sessions.end( ) != iter; ++iter )
		{
			CChatSession* session = (CChatSession*)iter->second;
			m_chat.GetDatas();
		}
	}
}
