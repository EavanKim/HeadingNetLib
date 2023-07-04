#pragma once

namespace Heading
{
	struct binding
	{

	};

	struct Session
	{
		SOCKADDR_IN				info;
		uint64_t				key;
		SOCKET					sock;
		Buffer					buff;
		std::vector<Header*>	sendBuff;
	};
}