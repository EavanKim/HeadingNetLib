#pragma once

#ifdef _WIN32 || _WIN64

namespace Heading
{
	class CSelect_Win : public ISelect
	{
	public:
		CSelect_Win();
		virtual ~CSelect_Win();

		// ISelect을(를) 통해 상속됨
		void Update() override;

	private:
		std::vector<WSAEVENT> m_WSALists = {};
		CRouter_Session* m_router = nullptr;
		fd_set m_read;
		fd_set m_send;
	};
}

#endif