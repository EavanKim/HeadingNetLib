#pragma once

namespace Heading
{
	class CSessionMgr
	{
	public:
		CSessionMgr();
		~CSessionMgr();

		void AddSession();

		void Update();

	private:
		std::unordered_map<WSAEVENT, CEventBaseSession_v2*> m_sessions;
	};
}

