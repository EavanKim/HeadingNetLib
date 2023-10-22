#include "HeadingNetHeader.h"

namespace Heading
{
	CHeadingObject::CHeadingObject(bool _isNeedCheackLifeCycleIsSameThread/* = false*/)
		: m_needThreadCheck(_isNeedCheackLifeCycleIsSameThread)
	{
		pthread_rwlock_init(&m_rwlock, NULL);
#if _DEBUG
		// 디버깅일때 해당 값이 사용된 객체는 Thread Check
		if ( m_needThreadCheck )
		{
			// 이거 리눅스에서 호환 될 방법 찾아보기.
			m_threadID = GetCurrentThreadId();
		}
#endif
	}

	CHeadingObject::~CHeadingObject()
	{
#if _DEBUG
		// 디버깅일때 해당 값이 사용된 객체는 Thread Check
		if ( m_needThreadCheck )
		{
			if ( m_threadID != GetCurrentThreadId() )
			{
				throw std::exception("CHeadingObject Check LifeCyle ThreadID Failure occured");
			}
		}
#endif
		pthread_rwlock_destroy(&m_rwlock);
	}

	void CHeadingObject::Lock_Write()
	{
		pthread_rwlock_wrlock(&m_rwlock);
	}

	void CHeadingObject::Lock_Read()
	{
		pthread_rwlock_rdlock(&m_rwlock);
	}

	void CHeadingObject::TryLock_Write()
	{
		pthread_rwlock_trywrlock(&m_rwlock);
	}

	void CHeadingObject::TryLock_Read()
	{
		pthread_rwlock_tryrdlock(&m_rwlock);
	}

	void CHeadingObject::Unlock()
	{
		pthread_rwlock_unlock(&m_rwlock);
	}
}
