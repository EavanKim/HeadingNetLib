#pragma once

// Session 은 서버가 관리하는 클라이언트 정보

namespace Heading
{
	class CSessionFactory_Conn : public ISessionFectory
	{
	public:
		virtual ISession* CreateSession(sessionKey_t _key, ISocket* _sock) override;

	private:

	};
}

 