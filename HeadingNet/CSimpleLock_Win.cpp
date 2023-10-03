#include "pch.h"

namespace Heading
{
	void CSimpleLock_Win::lock()
	{
		while ( 0 != InterlockedCompareExchange64(&m_lock, 1, 0) ) { }

		return;
	}

	bool CSimpleLock_Win::tryLock()
	{
		return 0 == InterlockedCompareExchange64(&m_lock, 1, 0);
	}

	void CSimpleLock_Win::unlock()
	{
		InterlockedExchange64(&m_lock, 0);
	}
}
