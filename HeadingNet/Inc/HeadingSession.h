#pragma once

namespace Heading
{
	enum ENUM_SOCKET_ERROR
	{
		WSA_E_CONN_ABORTED,
		WSA_E_CONN_RESET,
		WSA_E_IN_PROGRESS,
		WSA_E_INVAL,
		WSA_E_NET_DOWN,
		WSA_E_NOT_CONN,
		WSA_E_NOT_SOCK,
		WSA_NOT_INITIALISED,
		WSA_E_MAX
	};

	struct errorInfo
	{
		int				errWSA		= S_OK;
		int				errOS		= S_OK;
		std::string		LastError	= "";
	};

	struct sessionInfo
	{
		uint64_t		key			= 0;
		SOCKET			sock		= INVALID_SOCKET;
		SOCKADDR_IN		info		= {};
		Buffer			recvBuff	= {};
		packetBuff		sendBuff	= {};
	};

	typedef std::unordered_map<SOCKET, sessionInfo> SessionMap;
	typedef std::vector<sessionInfo>				FreeSession;

	struct connectionInfo
	{

		uint64_t		Key			= 0;
		std::string		ip			= "";
		std::string		port		= "";
		addrinfo*		info		= nullptr;
		SOCKET			sock		= INVALID_SOCKET;
		Buffer			recvBuff	= {};
		packetBuff		sendBuff	= {};
		errorInfo		err			= {};
	};

	struct bindingInfo
	{
		uint16_t		port		= 0;
		SOCKET			sock		= INVALID_SOCKET;
		SOCKADDR_IN		info		= {};
		fd_set			selectSet	= {};
		fd_set			readSet		= {};
		fd_set			writeSet	= {};
		SessionMap		sessionMap	= {};
		FreeSession		freeList	= {};
		errorInfo		err			= {};
	};
}