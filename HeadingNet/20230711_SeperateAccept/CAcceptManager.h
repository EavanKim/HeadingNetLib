#pragma once
namespace Heading
{
	class CAccept_Mgr
	{
	public:
		CAccept_Mgr( );
		~CAccept_Mgr( );

		// WSA_MAXIMUM_WAIT_EVENTS �� �Ѿ�ų� bind �����ϸ� false
		bool Set_NewAcceptPort( uint16_t _port );
		bool Set_CloseAcceptPort( uint16_t _port );

		void Do_Select( );

		bool Get_NewSocket( OUT NewSocketList& _newSocket );

	private:
		uint8_t					m_size								= 0;
		// Accepter�� Select �����θ� ����ϰ� Events ���� ��ü���� ����ϴ°� ������ ���...
		// ��� ������ Accept�� MAXIMUM�� �ѵ��� ó������ �����״� �ϴ� ����� �����մϴ�.
		// Client�� MAX���� �ʰ� ó���ϵ��� Chat �Ŵ��������� �ܺ��� EventMap�� �޾Ƽ� Select
		// �ȱ׷��� Ŭ�� 64�� �Ѿ�� �����ϱ� ����...
		AcceptSessionEventMap	m_accepts							= {};
		NewSocketList			m_newSockets						= {};
		WSAEVENT				m_events[WSA_MAXIMUM_WAIT_EVENTS]	= {};
	};
}

