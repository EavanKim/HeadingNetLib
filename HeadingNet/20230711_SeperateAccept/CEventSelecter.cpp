#include "pch.h"
#include "CEventSelecter.h"

Heading::CEventSelecter::CEventSelecter( )
{
}

Heading::CEventSelecter::~CEventSelecter( )
{
}

bool Heading::CEventSelecter::Set_NewEvents( WSAEVENT _newEventHandle )
{
	if( WSA_MAXIMUM_WAIT_EVENTS != m_size )
	{
		m_events[m_size] = _newEventHandle;
		++m_size;
		return true;
	}

	return false;
}

void Heading::CEventSelecter::Set_RemoveEvents( WSAEVENT _newEventHandle )
{
	// 배열을 선형으로 유지하기 위해 FastPop 같은 방식으로 마지막 값을 써서 덮어버린 뒤 맥스값만 조절합니다.
	// 어차피 Event는 OS 관리 객체의 관리 번호만 받는것이므로 값만 잃지 않으면 어떠한 복사 리스크도 없습니다.

	for( uint8_t seek = 0; WSA_MAXIMUM_WAIT_EVENTS > seek; ++seek )
	{
		if( m_events[ seek ] == _newEventHandle )
		{
			--m_size;
			m_events[ seek ] = m_events[ m_size ];
			return;
		}
	}
}

void Heading::CEventSelecter::Do_Update( )
{

}
