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
			case E_Wait_MEM_FULL:
				throw "Memory FULL !!!";
			case E_Wait_Reset_WSA:
				return -1;
			case E_Wait_Reset_SOCK:
			case E_Wait_Reset_EVENTS_ARRAY:
			{
				delete accepter;
				accepter = new CAccepter( info->port );
				events[ 0 ] = { accepter->Get_Event() };
			}
				break;
			case E_Wait_OK:
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
