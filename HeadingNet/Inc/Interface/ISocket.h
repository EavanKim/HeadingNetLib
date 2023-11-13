#pragma once

namespace Heading
{
	struct ISocket
	{
		virtual ~ISocket() {}

		virtual const SOCKADDR_IN*	GetInfo	( )					= 0;
		virtual bool				Send	( IMessage*	_send )	= 0;
		virtual void				Recv	( IMessage* _recv )	= 0;
	};
}