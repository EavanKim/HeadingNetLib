#include "pch.h"

namespace Heading
{
	CEventSelecter::CEventSelecter( )
	{
	}

	CEventSelecter::~CEventSelecter( )
	{
	}

	bool CEventSelecter::Set_NewEvents( WSAEVENT _newEventHandle )
	{
		if( WSA_MAXIMUM_WAIT_EVENTS != m_size )
		{
			m_events[m_size] = _newEventHandle;
			++m_size;
			return true;
		}

		return false;
	}

	void CEventSelecter::Set_RemoveEvents( WSAEVENT _newEventHandle )
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

	void CEventSelecter::Do_Update( ChatSessionEventMap& _sessionMap )
	{
		DWORD ret = WSAWaitForMultipleEvents( m_size, m_events, FALSE, 0, TRUE );

		switch( ret )
		{
		case WSA_WAIT_IO_COMPLETION:
		case WSA_WAIT_TIMEOUT:
		case -1:
			return;
		default:
			break;
		}

		// 리턴된 값이 가장 작은 인덱스임을 보장한다고 MSDN에 적혀있으므로
		// Wait 처리로 신호를 구분합니다.
		// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents
		// https://www.joinc.co.kr/w/man/4100/WASWaitForMultipleEvents 예제에 WaitForMultipleEvents의 리턴값이 에러가 아니라면 
		// 해당 리턴값에서 WSA_WAIT_EVENT_0을 뺀 값이 대상 인덱스
		for( INT seek = ret - WSA_WAIT_EVENT_0; WSA_MAXIMUM_WAIT_EVENTS > seek; ++seek )
		{
			// 대기하지 않고 이벤트 검사를 한 결과확인
			// 0번인 처음 seek는 무조건 set 상태지만 일단 검사해버린다.
			// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
			DWORD EventResult = WaitForSingleObject( m_events[ seek ], 0 );
			switch( EventResult )
			{
			case WAIT_ABANDONED:
			case WAIT_OBJECT_0:
			case WAIT_TIMEOUT:
			case WAIT_FAILED:
				continue;
			default:
				{
					ChatSessionEventMap::iterator iter = _sessionMap.find( m_events[ seek ] );
					if( _sessionMap.end() != iter )
					{
						iter->second->Update();
					}
				}
				break;
			}

			// WaitForSingleObject 결과 처리 대상이었던 신호가 들어온 이벤트라면!
			// 다 끝나면 이벤트 셋!
			WSAResetEvent( m_events[ seek ] );
		}
	}
}
