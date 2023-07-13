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
		ChatSessionEventMap m_clients = {};
	};
}

