#include "pch.h"

namespace Heading
{
	void CLock_UserData::lock_read()
	{
		// 잠금대기 - 쓰기중에 읽기잠금이 통과되는 일 없도록 선획득
		while (0 == InterlockedCompareExchange64(&m_wLock, 1, 0)) {};

		InterlockedAddAcquire64(&m_rLock, 1);

		//해제
		InterlockedExchange64(&m_wLock, 0);
	}

	void CLock_UserData::lock_write()
	{
		// 잠금대기
		while (0 == InterlockedCompareExchange64(&m_wLock, 1, 0)) {};

		// 읽기 끝날 때 까진 쓰기 처리로 진입 하지 않고 대기
		while (0 == InterlockedCompareExchange64(&m_rLock, 0, 0)) {};
	}

	bool CLock_UserData::tryLock_read()
	{
		if(0 == InterlockedCompareExchange64(&m_wLock, 1, 0)) 
		{
			InterlockedAddAcquire64(&m_rLock, 1);

			//해제
			InterlockedExchange64(&m_wLock, 0);

			return true;
		};

		return false;
	}

	bool CLock_UserData::tryLock_write()
	{
		if (0 == InterlockedCompareExchange64(&m_wLock, 1, 0))
		{
			// 읽기 끝날 때 까진 쓰기 처리로 진입 하지 않고 대기
			while (0 == InterlockedCompareExchange64(&m_rLock, 0, 0)) {};

			return true;
		};

		return false;
	}

	void CLock_UserData::release_read()
	{
		if (0 > InterlockedAddAcquire64(&m_rLock, -1))
		{
			InterlockedExchange64(&m_rLock, 0);
			// 던질까 말까 던질까 말까 던던던던...
		}
	}

	void CLock_UserData::release_write()
	{
		InterlockedExchange64(&m_wLock, 0);
	}
}
