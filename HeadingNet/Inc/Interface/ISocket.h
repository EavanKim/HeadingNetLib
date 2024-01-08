#pragma once

namespace Heading
{
	struct ISocket
	{
		virtual ~ISocket() {}

		virtual const SOCKADDR_IN*	GetInfo	( )					= 0;
		virtual bool				Send	( CCache_Line* _dataCache )	= 0;
		virtual void				Recv	( CCache_Line* _dataCache )	= 0;
	};
}