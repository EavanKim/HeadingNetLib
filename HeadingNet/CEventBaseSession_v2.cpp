#include "pch.h"

namespace Heading
{
	CEventBaseSession_v2::CEventBaseSession_v2( CSimpleSocket* _sock )
		: m_sock( _sock )
	{
	}
	
	CEventBaseSession_v2::~CEventBaseSession_v2( )
	{
	}
	
	void CEventBaseSession_v2::operator()( CSimpleSocket* _sock )
	{
		m_sock = _sock;
	}
}