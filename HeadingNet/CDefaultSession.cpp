#include "pch.h"

namespace Heading
{
	void CSession_Base::Release()
	{
		Flush();
		Reset();
	}
	
	void CSession_Base::Reset()
	{
		if (nullptr != m_sock)
		{
			delete m_sock;
			m_sock = nullptr;
		}
	}

	const SOCKADDR_IN* CSession_Base::GetInfo()
	{
		if (nullptr != m_sock)
		{
			return m_sock->GetInfo();
		}
		
		return nullptr;
	}

	void CSession_Base::Set(ISocket* _sock)
	{
		if (nullptr != m_sock)
		{
			delete m_sock;
			m_sock = nullptr;
		}

		m_sock = _sock;
	}

	bool CSession_Base::Set(IMessage* _message)
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

	void CSession_Base::Get(IMessage*& _message)
	{
		if (nullptr != m_sock)
		{
			m_sock->Recv(_message);
		}
	}

	void CSession_Base::Flush()
	{
		if (NULL != m_sock)
		{
			for (int count = 0; !m_sendQueue.empty(); ++count)
			{
				if (m_sock->Send(m_sendQueue.front()))
				{
					m_sendQueue.pop();
				}
			}
		}
	}

	void CSession_Base::Update()
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
