#pragma once
namespace Heading
{
	class CAccept_Mgr
	{
	public:
		CAccept_Mgr( );
		~CAccept_Mgr( );

		// WSA_MAXIMUM_WAIT_EVENTS 를 넘어가거나 bind 실패하면 false
		bool Set_NewAcceptPort( uint16_t _port );
		bool Set_CloseAcceptPort( uint16_t _port );

		void Do_Select( );

		bool Get_NewSocket( OUT NewSocketList& _newSocket );

	private:
		uint8_t					m_size								= 0;
		// Accepter를 Select 용으로만 사용하고 Events 맵은 전체에서 사용하는게 맞을지 고민...
		// 사실 어차피 Accept를 MAXIMUM이 넘도록 처리하진 않을테니 일단 여기다 선언합니다.
		// Client만 MAX넘지 않게 처리하도록 Chat 매니저에서는 외부의 EventMap을 받아서 Select
		// 안그러면 클라 64개 넘어가면 터지니까 문제...
		AcceptSessionEventMap	m_accepts							= {};
		NewSocketList			m_newSockets						= {};
		WSAEVENT				m_events[WSA_MAXIMUM_WAIT_EVENTS]	= {};
	};
}

