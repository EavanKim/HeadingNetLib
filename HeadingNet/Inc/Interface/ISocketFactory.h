#pragma once

namespace Heading
{
	class ISocketFactory
	{
	public:
		virtual ISocket* CreateSocket(connectionInfo& _info) = 0;
	};
}