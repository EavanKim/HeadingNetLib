#include "pch.h"

namespace Heading
{
	void CSimpleThreadLock::lock()
	{
		DWORD thread = GetCurrentThreadId();

		while ( thread != InterlockedCompareExchange64(&m_thread, thread, thread) ) // Thread값으로 덮어씌워졌음이 확인되지 않았다면
		{
			InterlockedCompareExchange64(&m_thread, thread, 0);  // Unlock이 되었을 때 자기자신으로 덮도록 대기

			// 다른 곳에서 덮혀져도 while 조건으로 탈출
		}

		InterlockedIncrement64(&m_counter);

		return;
	}

	bool CSimpleThreadLock::tryLock()
	{
		DWORD thread = GetCurrentThreadId();

		InterlockedCompareExchange64(&m_thread, thread, 0); // 일단 덮어쓰기 시도 해 보고

		if ( thread == InterlockedCompareExchange64(&m_thread, thread, thread) ) // 자기 자신의 값이 맞다면
		{
			InterlockedIncrement64(&m_counter); // 카운터 올리고
			return true; // 리턴
		}

		return false;
	}

	void CSimpleThreadLock::unlock()
	{
		InterlockedDecrement64(&m_counter);

		if( 0 == InterlockedCompareExchange64(&m_counter, 0, 0) )
			InterlockedExchange64(&m_thread, 0);
	}
}
