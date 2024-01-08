#pragma once

namespace Heading
{
	class CCache_Router
	{
	public:
		CCache_Router();
		~CCache_Router();

		void ReceiveRouting(ISocket* _sock);

	private:
		std::queue<CCache_Line*> m_available;
		std::list<CCache_Line*> m_wating;

		std::vector<CCache_Line> m_cacheLines;
	};
}

