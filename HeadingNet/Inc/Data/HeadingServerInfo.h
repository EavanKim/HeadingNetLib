#pragma once

namespace Heading
{
	struct ServerSetupInfo
	{
		WSADATA wsa;
		SOCKADDR_IN sock;
	};
}