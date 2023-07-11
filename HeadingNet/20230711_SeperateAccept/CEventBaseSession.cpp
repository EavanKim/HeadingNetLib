#include "pch.h"

namespace Heading
{
	CEventBaseSession::~CEventBaseSession( )
	{
		Release( );
	}

	void CEventBaseSession::Release( )
	{

	}

	SOCKET CEventBaseSession::Get_Sock( )
	{
		return m_sock;
	}
	WSAEVENT CEventBaseSession::Get_Event( )
	{
		return m_event;
	}
}