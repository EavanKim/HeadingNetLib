#include "pch.h"

namespace Heading
{
	SConnInfo::SConnInfo()
	{
	}

	SConnInfo::~SConnInfo()
	{
	}

	void SConnInfo::SetDestInfo(SOCKADDR_IN& _in)
	{
		destInfo = _in;
	}

	void SConnInfo::SetSessionInfo(sessionKey_t _session)
	{
		sessionInfo = _session;
	}

	const SOCKADDR_IN& SConnInfo::GetDestInfo()
	{
		return destInfo;
	}

	const sessionKey_t SConnInfo::GetSessionInfo()
	{
		return sessionInfo;
	}

	bool SConnInfo::operator==(SOCKADDR_IN& _in)
	{
		// 아이피와, 접속 방식, 포트가 같다면 일단 대상으로 봅니다.
		if ( ( destInfo.sin_addr.S_un.S_addr == _in.sin_addr.S_un.S_addr ) && ( destInfo.sin_family == _in.sin_family ) && ( destInfo.sin_port == _in.sin_port ) )
			return true;

		return false;
	}

	bool SConnInfo::operator==(sessionKey_t _key)
	{
		if ( sessionInfo == _key )
			return true;

		return false;
	}
}
