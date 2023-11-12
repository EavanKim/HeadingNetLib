#include "pch.h"

namespace Heading
{
	CSession_Router::CSession_Router(ISessionFectory* _factory)
		: m_sessionFactory(_factory)
	{

	}

	CSession_Router::~CSession_Router()
	{
		if (nullptr != m_sessionFactory)
		{
			delete m_sessionFactory;
			m_sessionFactory = nullptr;
		}
	}
	sessionKey_t CSession_Router::AcceptClient(ISocket* _sock)
	{
		sessionKey_t result = m_maximumSession;
		if (nullptr != m_sessionFactory)
		{
			std::unordered_map<ISocket*, sessionKey_t>::iterator findTraget = m_sessionRount.find(_sock);

			if (m_sessionRount.end() == findTraget)
			{
				if (m_maximumSession != m_lastMaxKey)
				{
					result = m_lastMaxKey;

					ISession* session = m_sessionFactory->CreateSession(result, _sock);

					m_sessionRount.insert(std::make_pair(_sock, result));
					m_sessionMap.insert(std::make_pair(result, session));

					++m_lastMaxKey;
				}
			}
			else
			{
				result = findTraget->second;
			}
		}

		return result;
	}
	
	ISession* CSession_Router::findSession(ISocket* _sock)
	{
		ISession* result = nullptr;
		if (nullptr != m_sessionFactory)
		{
			std::unordered_map<ISocket*, sessionKey_t>::iterator findTraget = m_sessionRount.find(_sock);

			if (m_sessionRount.end() != findTraget)
			{
				result = findSession(findTraget->second);
			}
		}

		return result;
	}

	ISession* CSession_Router::findSession(sessionKey_t _key)
	{
		ISession* result = nullptr;
		std::unordered_map<sessionKey_t, ISession*>::iterator findTarget = m_sessionMap.find(_key);
		if (m_sessionMap.end() != findTarget)
		{
			// 여기는 키를 잘못 넣을 가능성이 있으므로 있는 경우에만 돌려줍니다.
			result = findTarget->second;
		}

		return result;
	}

	void CSession_Router::DisconnectClient(ISocket* _sock)
	{
		if (nullptr != m_sessionFactory)
		{
			std::unordered_map<ISocket*, sessionKey_t>::iterator findTraget = m_sessionRount.find(_sock);

			if (m_sessionRount.end() != findTraget)
			{
				m_sessionRount.erase(findTraget);
			}
		}
	}
}
