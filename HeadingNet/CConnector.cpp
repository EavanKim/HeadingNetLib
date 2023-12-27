#include "pch.h"

namespace Heading
{
	CConnector::CConnector( )
	{

	}

	CConnector::~CConnector( )
	{

	}

	void CConnector::newConnect( connectionInfo& _info )
	{
		addrinfo info = {};
		ZeroMemory( &info, sizeof( addrinfo ) );
		info.ai_family = AF_INET;
		info.ai_socktype = SOCK_STREAM;
		info.ai_protocol = IPPROTO_TCP;

		int fnResult = getaddrinfo( _info.ip.c_str( ), _info.port.c_str( ), &info, &_info.info );
		if( S_OK != fnResult )
		{
			_info.sock = INVALID_SOCKET;
			return;
		}

		_info.sock = socket( _info.info->ai_family, _info.info->ai_socktype, _info.info->ai_protocol );
	}

	void CConnector::onPing( )
	{

	}

}
