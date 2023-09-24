#pragma once

namespace Heading
{
	struct AcceptThreadInfo_v2
	{
		uint16_t port = 0;
		void ( *onAccept )( SOCKET ) = nullptr;
		void ( *onNewSession )( ) = nullptr;
		bool ( *liveChecker )( ) = nullptr;
	};

	class CAcceptThread_v2 : public std::thread
	{
	public:
		CAcceptThread_v2(AcceptThreadInfo_v2* _ptr);

		static int functionAccept(void* _ptr);

	private:
		bool m_isReceivedAccessData = false;
	};
}

