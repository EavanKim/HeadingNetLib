#include "pch.h"

Heading::CAccepter::CAccepter( uint64_t _size )
{

}

Heading::CAccepter::~CAccepter( )
{

}

void Heading::CAccepter::Do_Update( )
{

}

void Heading::CAccepter::Do_Select( )
{

}

bool Heading::CAccepter::Get_NewSocket( OUT SocketList& _newSocket )
{
	m_newSockets.swap(_newSocket);
	if(0 != _newSocket.size())
	{
		return true;
	}

	return false;
}
