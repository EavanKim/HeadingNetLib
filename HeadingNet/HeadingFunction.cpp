#include "pch.h"

namespace Heading
{
	std::string PrintMem( char* _ptr, uint64_t _length )
	{
		return std::string();
	}
	void start( WSAData& _data )
	{
		WSAStartup( MAKEWORD( 0, 0 ), &_data );
	}

	void end()
	{
		WSACleanup();
	}

	//=====================================================================================================================

	bool createInfo( connectionInfo& _connInfo )
	{
		addrinfo createData = {};
		SecureZeroMemory( ( PVOID )&createData, sizeof( addrinfo ) );
		createData.ai_family = AF_UNSPEC;
		createData.ai_socktype = SOCK_STREAM;
		createData.ai_protocol = IPPROTO_TCP;

		int result = getaddrinfo( _connInfo.ip.c_str(), _connInfo.port.c_str(), &createData, &_connInfo.info );
		if( S_OK == result )
		{
			return true;
		}

		_connInfo.err.errWSA = WSAGetLastError();
		_connInfo.err.errOS = GetLastError();

		return false;
	}

	bool connect( connectionInfo& _connInfo )
	{

		_connInfo.sock = socket( _connInfo.info->ai_family, _connInfo.info->ai_socktype, _connInfo.info->ai_protocol );
		if( INVALID_SOCKET == _connInfo.sock )
		{
			_connInfo.err.errWSA = WSAGetLastError();
			_connInfo.err.errOS = GetLastError();
			return false;
		}

		int result = connect( _connInfo.sock, _connInfo.info->ai_addr, ( int )_connInfo.info->ai_addrlen );
		if( SOCKET_ERROR == result )
		{
			_connInfo.err.errWSA = WSAGetLastError();
			_connInfo.err.errOS = GetLastError();
			int err = 0;
			if( WSAECONNREFUSED == ( err = WSAGetLastError() ) )
			{
				closesocket( _connInfo.sock );
				_connInfo.sock = INVALID_SOCKET;
			}
			_connInfo.err.LastError = formatf( "connect failed with error: %d\n", err );
			freeaddrinfo( _connInfo.info );
			return false;
		}

		return false;
	}

	bool disconnect( connectionInfo& _connInfo )
	{
		if( INVALID_SOCKET != _connInfo.sock )
		{
			shutdown( _connInfo.sock, SD_BOTH );
			closesocket( _connInfo.sock );
			_connInfo.sock = INVALID_SOCKET;
		}
		return false;
	}

	int send( _Inout_ connectionInfo& _connInfo )
	{
		int totalSend = 0;

		for( Header* packet : _connInfo.sendBuff )
		{
			::send( _connInfo.sock, ( char* )packet, packet->length, 0 );
			totalSend += packet->length;
		}

		return totalSend;
	}

	int recv( connectionInfo& _connInfo )
	{
		int			Receive		= 0;
		uint64_t	length		= 0;
		char*		bufferStart	= nullptr;

		if( _connInfo.recvBuff.get_buffer(&bufferStart, &length ) )
		{
			Receive = ::recv( _connInfo.sock, bufferStart, length, 0 );
			if( 0 > Receive )
				_connInfo.recvBuff.commit( Receive );
		}

		return Receive;
	}

	int Recorver( connectionInfo& _connInfo )
	{
		return 0;
	}

	//=====================================================================================================================

	bool createInfo( bindingInfo& _connInfo )
	{
		return false;
	}

	bool bind( bindingInfo& _connInfo )
	{
		return false;
	}

	int aSelect( bindingInfo& _connInfo )
	{
		return 0;
	}

	bool release( bindingInfo& _connInfo )
	{
		return false;
	}

	bool accept( bindingInfo& _connInfo )
	{
		return false;
	}

	int send( bindingInfo& _connInfo )
	{
		return 0;
	}

	int recv( bindingInfo& _connInfo )
	{
		return 0;
	}

	int Recorver( bindingInfo& _connInfo )
	{
		return 0;
	}

	//=====================================================================================================================

	Header* Parsing( Buffer& _buffer )
	{
		return nullptr;
	}
	void Parsing( packetBuff& _pullParsing )
	{
	}
}
