#include "pch.h"

namespace Heading
{
	const SOCKADDR_IN* CDefaultSession::GetInfo()
	{
		if (nullptr != m_sock)
		{
			return m_sock->GetInfo();
		}
		
		return nullptr;
	}

	void CDefaultSession::Set(ISocket* _sock)
	{
		if (nullptr != m_sock)
		{
			delete m_sock;
			m_sock = nullptr;
		}

		m_sock = _sock;
	}

	bool CDefaultSession::Set(IMessage* _message)
	{
		if (nullptr != m_sock) // 소켓 처리가 가능하다면 전송 가능한지 확인.
		{
			if (m_sock->Send(_message))
			{
				return true;
			}
		}

		m_sendQueue.push(_message);
	}

	void CDefaultSession::Get(IMessage*& _message)
	{
		if (nullptr != m_sock)
		{
			m_sock->Recv(_message);
		}
	}

	void CDefaultSession::Update()
	{
		if (NULL != m_sock)
		{
			for (int count = 0; !m_sendQueue.empty() && (5 > count); ++count)
			{
				if (m_sock->Send(m_sendQueue.front()))
				{
					m_sendQueue.pop();
				}
			}
		}
	}
}
