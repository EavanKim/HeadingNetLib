#pragma once

// 네트워크 관련 베이스 오브젝트
// 

namespace Heading
{
	class CHeadingObject
	{
	protected:
		CHeadingObject(bool _isNeedCheackLifeCycleIsSameThread = false);
		~CHeadingObject();

		// 데드락이 걸리지 않게 주의하면서 잘 쓰기.
		void Lock_Write();
		void Lock_Read();
		void TryLock_Write();
		void TryLock_Read();
		void Unlock();

	private:
		pthread_rwlock_t	m_rwlock;
		bool m_needThreadCheck = false;
		DWORD m_threadID = 0;
	};
}