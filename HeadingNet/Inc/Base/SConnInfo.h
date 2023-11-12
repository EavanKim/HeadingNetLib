#pragma once

namespace Heading
{
	struct SConnInfo
	{
	public:
		SConnInfo();
		~SConnInfo();

		void SetDestInfo(SOCKADDR_IN& _in);
		void SetSessionInfo(sessionKey_t _session);

		const SOCKADDR_IN& GetDestInfo();
		const sessionKey_t GetSessionInfo();

		bool operator==(SOCKADDR_IN& _in);
		bool operator==(sessionKey_t _key);

	private:
		time_t		lastConnTime = ::time(NULL);
		SOCKADDR_IN destInfo;
		sessionKey_t sessionInfo;
	};
}