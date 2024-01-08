#include "pch.h"

namespace Heading
{
    CCache_Line::CCache_Line()
    {
    }

    CCache_Line::~CCache_Line()
    {
    }

    CLock_UserData* CCache_Line::GetCacheData(char*& _result, uint64_t& _size)
    {
        // 정적 공간이므로 포인터 획득은 널 문제가 발생할 수 없어서
        // 접근 전에 잠금만 확보하면 안전합니다.
        _result = m_buffer;

        // 현재 사이즈로 기록된 내용을 돌려줍니다.
        _size = m_size;

        return &m_lock;
    }

    CLock_UserData* CCache_Line::SetCacheData(char*& _result, uint64_t& _size)
    {
        // 정적 공간이므로 포인터 획득은 널 문제가 발생할 수 없어서
        // 접근 전에 잠금만 확보하면 안전합니다.
        _result = m_buffer;

        // 쓰기 위한 정보는 최대 사이즈로 강제 반환합니다.
        _size = MAX_CACHE_SIZE;
        m_size = MAX_CACHE_SIZE;

        return &m_lock;
    }

    void CCache_Line::Commit(uint64_t _size)
    {
        if (MAX_CACHE_SIZE > _size)
        {
            m_size = _size;
        }
    }
    uint64_t CCache_Line::GetMaximum()
    {
        return MAX_CACHE_SIZE;
    }
}
