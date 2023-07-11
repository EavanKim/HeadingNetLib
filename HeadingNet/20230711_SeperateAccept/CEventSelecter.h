#pragma once
// Select 처리해주는 Component

namespace Heading
{
	class CEventSelecter
	{
	public:
		CEventSelecter( );
		~CEventSelecter( );

		bool Set_NewEvents( WSAEVENT _newEventHandle );
		void Set_RemoveEvents( WSAEVENT _newEventHandle );

		void Do_Update( );

	private:
		uint8_t	 m_size								= 0;
		WSAEVENT m_events[WSA_MAXIMUM_WAIT_EVENTS]	= {};
	};
}

