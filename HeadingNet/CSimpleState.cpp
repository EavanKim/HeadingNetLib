#include "pch.h"

namespace Heading
{
	CSimpleState::CSimpleState( )
	{
	}

	CSimpleState::~CSimpleState( )
	{
	}

	void CSimpleState::setState( bool _onoff, int _flag )
	{
		if( _onoff )
		{
			m_state |= _flag;
		}
		else
		{
			m_state &= ~_flag;
		}
	}

	bool CSimpleState::checkState( int _flag )
	{
		return _flag == ( m_state & _flag );
	}

	void CSimpleState::initialize( int _flag )
	{
		m_state = _flag;
	}
}