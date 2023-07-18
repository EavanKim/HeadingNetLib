#pragma once

namespace Heading
{
	namespace SimpleServerKit
	{
		typedef std::vector<WSAEVENT> eventArray;

		class Manager
		{
		public:

		private:
			Manager();
			~Manager();

			WSADATA m_data = {};
			std::unordered_map<WSAEVENT, CEventBaseSession> m_sessions;
			std::unordered_map<uint64_t, eventArray> m_eventGroup;
			std::unordered_map<uint64_t, CSelecter> m_selecters; // 이 안의 정적 배열로만 관리할지 이벤트 어레이를 쓸 지 고민
		};
	}
}

