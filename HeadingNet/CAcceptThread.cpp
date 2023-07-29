#include "pch.h"

namespace Heading
{
	CAcceptThread::CAcceptThread( AcceptThreadInfo* _ptr )
		: std::thread(CAcceptThread::functionAccept, _ptr)
	{

	}

	int CAcceptThread::functionAccept( void* _ptr )
	{
		AcceptThreadInfo* info = static_cast< AcceptThreadInfo* >( _ptr );

		if( (nullptr == info->liveChecker) || (nullptr == info->onAccept) )
		{
			throw formatf( "Accept Thread Crash! function pointer is Null [liveChecker : %llX] [onAccept : %llX]", info->liveChecker, info->onAccept );
		}

		CAccepter* accepter = new CAccepter( info->port );
		accepter->Bind();

		WSAEVENT events[ 1 ] = { accepter->Get_Event() };

		while( info->liveChecker() )
		{
			DWORD result = WSAWaitForMultipleEvents(1, events, FALSE, INFINITE, FALSE);
			switch( WaitObjectCheck( result ) )
			{
			case E_Wait_Delayed:
				break;
			case E_Wait_MEM_FULL: // 크래시 어떻게 낼까 고민해보기
				throw "Memory FULL !!!"; // 이거 뜨면 막장이니 Throw 시켜버리기
			case E_Wait_Reset_WSA: // 이거 뜨면 답이 없다...
				return -1;
			case E_Wait_Reset_SOCK: // 단 하나뿐인 accepter socket이 맛이 간 것 같으니 삭제 후 재할당 진행
			case E_Wait_Reset_EVENTS_ARRAY: // 단 하나뿐인 Event가 고장났으니 accepter 수리 필요. // 삭제 후 재할당 진행
			{
				delete accepter;
				accepter = new CAccepter( info->port );
				events[ 0 ] = { accepter->Get_Event() };
			}
				break;
			case E_Wait_OK: // 여기서 Accept 처리
			{
				sockaddr_in sockinfo = {};
				SOCKET newSock = accepter->CreateConnect( sockinfo );
				if( INVALID_SOCKET != newSock )
					info->onAccept( newSock );
			}
				break;
			default:
				break;
			}

			WSAResetEvent(events[result] );
		}

		return 0;
	}
}
