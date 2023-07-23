#pragma once

namespace Heading
{
	class CSelecter
	{
	public:
		CSelecter();
		virtual ~CSelecter();

		bool Check_SessionCapacity();
		virtual bool Set_NewSession( Heading::CClientSession* _newSocket );
		int Do_Select( void* _ptr );
		void Do_FlushPendingQueue();
		void Stop();

	protected:
		volatile LONG64		m_bSessionLive						= 0;
		int					m_size								= 0;
		int					m_eventSize							= 0;
		concurrency::concurrent_queue<Heading::CClientSession*> m_pendingSession;
		ChatSessionEventMap m_sessions							= {};
		WSAEVENT			m_events[WSA_MAXIMUM_WAIT_EVENTS]	= {};
	};
}

