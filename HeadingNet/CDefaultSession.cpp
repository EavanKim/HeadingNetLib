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
		m_sock = _sock;
	}

	void CDefaultSession::Set(IMessage& _message)
	{
		if (nullptr != m_sock)
		{
			m_sock->Send( _message );
		}
	}

	void CDefaultSession::Get(IMessage& _message)
	{

	}
}
