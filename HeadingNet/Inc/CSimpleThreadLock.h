#pragma once

//========================================================================
// 
// 같은 thread내라면 굳이 잠금체크가 필요없는 동작에서만 쓰는 잠금.
// 
// 해제 처리를 위해 카운터 도입 필수
// 
//========================================================================

namespace Heading
{
	class CSimpleThreadLock
	{
	public:
		void lock();

		bool tryLock();

		void unlock();

	private:
		LONG64 m_counter = 0;
		LONG64 m_thread = 0;
	};
}

