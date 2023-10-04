#include "pch.h"

namespace Heading
{
	CSessionStorage::CSessionStorage()
	{
		m_dataStorage = ( char* ) malloc(MAXIMUM_CUSTOM_STORAGE_SIZE);
		memset(m_dataStorage, 0x00, MAXIMUM_CUSTOM_STORAGE_SIZE);

		PrevIndex(MemStart) = NULL;
		NextIndex(MemStart) = NULL;
		MemState(MemStart) = true;
		MemSize(MemStart) = MAXIMUM_CUSTOM_STORAGE_SIZE;
	}

	CSessionStorage::~CSessionStorage()
	{
		if ( nullptr != m_dataStorage )
		{
			delete m_dataStorage;
			m_dataStorage = nullptr;
		}
	}

	void CSessionStorage::split(index_t _addr, storageSize_t _size)
	{
		// 원하는 양 보다 크면 찢고
		if ( MemSize(_addr) >= (_size + SZ_MEMHEADER + SZ_MEMHEADER + 1) ) // 헤더 사이즈에 딱 맞으면 사이즈 0짜리니까 1을 더 크게 확인
		{
			index_t oldPrev = PrevIndex(_addr);
			index_t oldNext = NextIndex(_addr);
			storageSize_t oldSize = MemSize(_addr);

			index_t newAddr = _addr + _size + SZ_MEMHEADER;

			NextIndex(_addr) = newAddr;
			MemState(_addr) = false;
			MemSize(_addr) = _size;

			PrevIndex(newAddr) = _addr;
			NextIndex(newAddr) = oldNext;
			MemState(newAddr) = true;
			MemSize(newAddr) = oldSize - _size - SZ_MEMHEADER;
		}
		else // 딱맞으면 그냥 쓰기
		{
			MemState(_addr) = false;
		}
	}

	void CSessionStorage::merge(index_t _prev, index_t _current, index_t _next)
	{
		if ( NULL == _prev )
		{
			if ( NULL == _next )
			{
				MemState(_current) = true;
			}
			else if ( false == MemState(_next) )
			{
				MemState(_current) = true;
			}
			else if ( true == MemState(_next) )
			{
				MemState(_current) = true;
				MemSize(_current) = MemSize(_current) + MemSize(_next) + SZ_MEMHEADER;

				index_t temp = NextIndex(_next);
				PrevIndex(temp) = _current;
			}
		}
		else if ( NULL == _next )
		{
			if ( false == MemState(_prev) )
			{
				MemState(_current) = true;
			}
			else if ( true == MemState(_prev) )
			{
				MemSize(_prev) = MemSize(_prev) + MemSize(_current) + SZ_MEMHEADER;
				NextIndex(_prev) = NextIndex(_current);
			}
		}
		else if ( (false == MemState(_prev)) && (false == MemState(_next))  )
		{
			MemState(_current) = true;
		}
		else if ( ( true == MemState(_prev) ) && ( false == MemState(_next) ) )
		{
			MemState(_current) = true;
			MemSize(_current) = MemSize(_current) + MemSize(_next) + SZ_MEMHEADER;
			NextIndex(_current) = NextIndex(_next);

			index_t temp = NextIndex(_next);
			if ( NULL != temp )
			{
				PrevIndex(temp) = _current;
			}
		}
		else if ( ( false == MemState(_prev) ) && ( true == MemState(_next) ) )
		{
			MemSize(_prev) = MemSize(_prev) + MemSize(_current) + SZ_MEMHEADER;
			NextIndex(_prev) = NextIndex(_current);
			PrevIndex(_next) = _prev;
		}
		else if ( ( true == MemState(_prev) ) && ( true == MemState(_next) ) )
		{
			MemSize(_prev) = MemSize(_prev) + MemSize(_current) + SZ_MEMHEADER + MemSize(_next) + SZ_MEMHEADER;
			NextIndex(_prev) = NextIndex(_next);

			index_t temp = NextIndex(_next);
			if ( NULL != temp )
			{
				PrevIndex(temp) = _prev;
			}
		}
	}
}