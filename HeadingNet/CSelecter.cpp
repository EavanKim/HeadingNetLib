#include "pch.h"

//=============================================================================================================================
// 
// �ٽ� �����ϴ� �ּ���ġ
// 
// ������ �� ���� �κе� ����ϱ�
// 1. �� ���� ���������� ��� �ϳ��� �����ϰ� ���� �ۼ����ϴ°��� Ÿ�̹����� ����޴µ��� �ϳ��� �ؾ��ϴ� ���ϱ�...?
//		- �Ƹ��� ��� �����ϱ�...?
// 2. �۽� Ÿ�̹��� ������ Ŭ�� �ֹ��� �ƹ����� �����ٵ� ��ø�ص� ����� ������...?
//		- �Ƹ� ����̹� �ܿ��� �� ���� ��ü�� �и��Ǿ��ְ� (Rx / Tx) ��ü���� ���ۿ� ���� ���� ��ü�� �������� ������
// 
//=============================================================================================================================

namespace Heading
{
	CSelecter::CSelecter( )
	{
		InterlockedExchange64(&m_bSessionLive, 1);
	}

	CSelecter::~CSelecter( )
	{
	}

	int CSelecter::Do_Select( void* _ptr )
	{
		while( 1 == InterlockedCompareExchange64(&m_bSessionLive, 0, 0) )
		{
			DWORD ret = WSAWaitForMultipleEvents( ( DWORD ) m_sessions.size( ), m_events, FALSE, 0, TRUE );

			switch( ret )
			{
			case WSA_WAIT_FAILED:
			case WSA_WAIT_IO_COMPLETION:
			case WSA_WAIT_TIMEOUT:
			{
				// would be need blocked but...we need process
				// �а��ִ� �۽����� 100% �ŷ��ϰ� ���� �����̱� ������
				// ���� �۽� �õ� ����Ű�� ���۰� ��ĥ �� �־ �������� �ڵ�.
				// ���� �۽� �õ� ��Ⱑ ���������� ������ ���ϸ� ���� ��������Ⱑ �ʿ�������...
				// ������ �׳� �ѱ�⸸ �մϴ�.
				if(	WSAEWOULDBLOCK == WSAGetLastError() ) 
					continue;
			}
			continue;
			default:
				break;
			}

			// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents
			// https://www.joinc.co.kr/w/man/4100/WASWaitForMultipleEvents 
			ChatSessionEventMap::iterator iter = m_sessions.find( m_events[ ret - WSA_WAIT_EVENT_0 ] );
			if( m_sessions.end( ) != iter )
			{
				iter->second->Update( );				
			}

			WSAResetEvent( m_events[ ret - WSA_WAIT_EVENT_0 ] );
		}

		return 0;
	}
	void CSelecter::Stop( )
	{
		InterlockedExchange64(&m_bSessionLive, 0);
	}
}
