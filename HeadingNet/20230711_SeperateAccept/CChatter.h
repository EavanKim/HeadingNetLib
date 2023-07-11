#pragma once

namespace Heading
{
	class CChatter
	{
	public:
		CChatter();
		~CChatter();

		void Do_Update();

	private:
		ChatSessionEventMap m_clients = {};
	};
}

