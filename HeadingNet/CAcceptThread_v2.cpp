#include "pch.h"

namespace Heading
{
	CAcceptThread_v2::CAcceptThread_v2(AcceptThreadInfo_v2* _ptr)
		: std::thread(functionAccept, _ptr)
	{

	}

	int CAcceptThread_v2::functionAccept(void* _ptr)
	{
		AcceptThreadInfo_v2* info = static_cast< AcceptThreadInfo_v2* >( _ptr );

		if( (nullptr == info->liveChecker) || (nullptr == info->onAccept) || (nullptr == info->onNewSession) )
		{
			throw formatf( "Accept Thread Crash! function pointer is Null [liveChecker : %llX] [onAccept : %llX] [ onNewSession : %llX]", info->liveChecker, info->onAccept, info->onNewSession );
		}
		
		CAccepter* accepter = new CAccepter( info->port );
		accepter->Bind();
		CLoginWaiter Waiter( accepter );

		while( info->liveChecker() )
		{
			DWORD EventCount = 0;
			WSAEVENT* SelectTarget = nullptr;

			Waiter.preSelect(&SelectTarget, &EventCount);

			if ( 0 == EventCount )
			{
				// Accept까지 죽은채로 복구되지 않고 다음 루프에 왔습니다.
				// Thread에서 탈출합니다.
				break;
			}

			DWORD result = WSAWaitForMultipleEvents(EventCount, SelectTarget, FALSE, INFINITE, FALSE);
			switch( WaitObjectCheck( result ) )
			{
				case E_Wait_Delayed:
					break;
				case E_Wait_MEM_FULL:
					throw "Memory FULL !!!";
				case E_Wait_Reset_WSA:
					return -1;
				case E_Wait_Reset_SOCK:
				case E_Wait_Reset_EVENTS_ARRAY:
					{
						delete accepter;
						Waiter.updateAccepter( new CAccepter( info->port ) );
					}
					break;
				case E_Wait_OK:
					{
						if ( 0 == result )
						{
							sockaddr_in sockinfo = {};
							SOCKET newSock = accepter->CreateConnect( sockinfo );
							if ( INVALID_SOCKET != newSock )
								Waiter.addNewSocket(newSock);
						}
						else
						{
							SOCKET sock = Waiter.removeSocket(result);
							info->onAccept(sock);
						}
					}
					break;
				default:
					break;
			}

			WSAResetEvent( SelectTarget[result] );
		}

		delete accepter;
		delete _ptr;

		return 0;
	}
}
