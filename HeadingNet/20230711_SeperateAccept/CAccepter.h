#pragma once
namespace Heading
{
	class CAccepter
	{
	public:
		CAccepter(uint64_t _size);
		~CAccepter();

		void Do_Update();

	private:
		void Do_Select();
		bool Get_NewSocket( OUT SocketList& _newSocket );

		uint64_t				m_acceptsSize						= 0;
		AcceptSessionEventMap	m_accepts							= {};
		SocketList				m_newSockets						= {};
		WSAEVENT				m_events[WSA_MAXIMUM_WAIT_EVENTS]	= {};
	};
}

