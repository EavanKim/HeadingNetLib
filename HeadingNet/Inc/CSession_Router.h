#pragma once

namespace Heading
{
	class CSession_Router
	{
	public:
		CSession_Router(ISessionFectory* _factory);
		~CSession_Router();

		sessionKey_t	AcceptClient		( ISocket*		_sock );
		ISession*		findSession			( ISocket*		_sock );
		ISession*		findSession			( sessionKey_t	_key );
		void			DisconnectClient	( ISocket*		_sock );

	private:
		ISessionFectory* m_sessionFactory = nullptr;

		uint64_t m_maximumSession = UINT64_MAX;
		sessionKey_t m_lastMaxKey = 0;
		std::queue<sessionKey_t> m_freeKey;
		std::unordered_map<ISocket*, sessionKey_t> m_sessionRount;
		std::unordered_map<sessionKey_t, ISession*> m_sessionMap;
	};
}