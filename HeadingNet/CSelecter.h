#pragma once

namespace Heading
{
	class CSelecter
	{
	public:
		CSelecter();
		virtual ~CSelecter();

		virtual void Set_NewSession( NewSocketList& _newSocket ) = 0;
		void Do_Select( );

	protected:
		int					m_size								= 0;
		ChatSessionEventMap m_sessions							= {};
		WSAEVENT			m_events[WSA_MAXIMUM_WAIT_EVENTS]	= {};
	};
}

