#pragma once

namespace Heading
{
	class CChatter
	{
	public:
		CChatter( );
		~CChatter( );

		void Set_NewSession( NewSocketList& _newSocket );

		void Do_Select( );

	private:
		int					m_size								= 0;
		ChatSessionEventMap m_clients							= {};
		ChatSessionEventMap m_broadCasts						= {};
		WSAEVENT			m_events[WSA_MAXIMUM_WAIT_EVENTS]	= {};
	};
}

