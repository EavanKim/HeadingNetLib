#pragma once

//========================================================================
// 
// simplelock의 핵심은 interlocked 동작으로 값이 바뀌는 순간에만 캐시잠금이 발생한다는 점.
// 반드시 interlocked 결과로만 비교해야합니다.
// 
//========================================================================

namespace Heading
{
	class CSimpleLock_Win
	{
	public:
		void lock();

		bool tryLock();

		void unlock();

	private:
		LONG64 m_lock = 0;
	};
}

