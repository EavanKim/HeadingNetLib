#pragma once

namespace Heading
{
	class ISessionFectory
	{
	public:
		virtual ISession* CreateSession(sessionKey_t _key, ISocket* _sock) = 0;
	private:
	};
}