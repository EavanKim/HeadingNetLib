#include "pch.h"

namespace Heading
{
	CSimpleCallback::CSimpleCallback( )
	{

	}

	CSimpleCallback::~CSimpleCallback( )
	{

	}

	void CSimpleCallback::operator()( fnCallback _fn, void* _reserveParam )
	{
		m_fn = _fn;
		m_param = _reserveParam;
	}

	void CSimpleCallback::operator()( void* _callerParam )
	{
		if( nullptr != m_fn )
			m_fn(_callerParam, m_param);
	}
}