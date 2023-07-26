#pragma once

namespace Heading
{
	struct AcceptThreadInfo
	{
		uint16_t port = 0;
		void ( *onAccept )( SOCKET ) = nullptr;
		bool ( *liveChecker )( ) = nullptr;
	};

	class CAcceptThread : public std::thread
	{
	public:
		CAcceptThread( AcceptThreadInfo* _ptr );

		static int functionAccept(void* _ptr);
	};
}

