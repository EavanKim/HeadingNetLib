#pragma once

namespace Heading
{
	struct ISocket
	{
		virtual ~ISocket() {}

		virtual void 			Send( char*				_send
									, unsigned short	_size ) = 0;
		virtual unsigned int	Recv( char*				_recv ) = 0;
	};
}