#pragma once

namespace Heading
{
	template<typename ... Args>
	static std::string	formatf( _In_ const	std::string& _format
								, _In_	Args ..._args )
	{
		int length = std::snprintf( nullptr, 0, _format.c_str( ), _args ... ) + 1;

		if( length <= 0 )
		{
			return "";
		}

		std::string result = "";

		result.resize( length, '\0' );

		std::snprintf( ( char* ) result.c_str( ), length, ( char* ) _format.c_str( ), _args ... );

		return result;
	}

	static void	PrintMem( _In_	char* _ptr
								, _In_	uint64_t _length )
	{
		for (uint64_t seek = 0; _length > seek; ++seek)
			printf("%02x ", _ptr[seek]);
	}


	static void	WSAErrorString	( _In_	int	_code
								, _Out_	std::string& _string )
	{
		switch( _code )
		{
		case WSA_WAIT_FAILED: // WSA_INFINITE
			_string = std::string( "WSA_WAIT_FAILED" );
			break;
		case WSA_IO_PENDING:
			_string = std::string( "WSA_IO_PENDING" );
			break;
		case WSA_IO_INCOMPLETE:
			_string = std::string( "WSA_IO_INCOMPLETE" );
			break;
		case WSA_INVALID_HANDLE:
			_string = std::string( "WSA_INVALID_HANDLE" );
			break;
		case WSA_NOT_ENOUGH_MEMORY:
			_string = std::string( "WSA_NOT_ENOUGH_MEMORY" );
			break;
		case WSA_OPERATION_ABORTED:
			_string = std::string( "WSA_OPERATION_ABORTED" );
			break;
		case WSA_MAXIMUM_WAIT_EVENTS:
			_string = std::string( "WSA_MAXIMUM_WAIT_EVENTS" );
			break;
		case WSA_INVALID_PARAMETER:
			_string = std::string( "WSA_INVALID_PARAMETER" );
			break;
		case WSA_WAIT_EVENT_0:
			_string = std::string( "WSA_WAIT_EVENT_0" );
			break;
		case WSA_WAIT_IO_COMPLETION:
			_string = std::string( "WSA_WAIT_IO_COMPLETION" );
			break;
		case WSA_WAIT_TIMEOUT:
			_string = std::string( "WSA_WAIT_TIMEOUT" );
			break;
		case WSANOTINITIALISED:
			_string = std::string( "WSANOTINITIALISED" );
			break;
		case WSASYSNOTREADY:
			_string = std::string( "WSASYSNOTREADY" );
			break;
		case WSAVERNOTSUPPORTED:
			_string = std::string( "WSAVERNOTSUPPORTED" );
			break;
		case WSAEINPROGRESS:
			_string = std::string( "WSASYSNOTREADY" );
			break;
		case WSAEPROCLIM:
			_string = std::string( "WSAEPROCLIM" );
			break;
		case WSAEFAULT:
			_string = std::string( "WSAEFAULT" );
			break;
		default:
			_string = std::string( "default" );
			break;
		}
	}

	static void start( _In_	WSAData& _data )
	{
		std::string str;
		WSAErrorString( WSAStartup( MAKEWORD( 2, 2 ), &_data ), str );
		printf( "Heading::string Error %s \n", str.c_str( ) );
	}

	static void end( )
	{
		WSACleanup( );
	}

	//=====================================================================================================================

	static bool createInfo( _Inout_ connectionInfo& _connInfo )
	{
		addrinfo createData = {};

		SecureZeroMemory( ( PVOID ) &createData, sizeof( addrinfo ) );

		createData.ai_family = AF_UNSPEC;
		createData.ai_socktype = SOCK_STREAM;
		createData.ai_protocol = IPPROTO_TCP;

		int result = getaddrinfo( _connInfo.ip.c_str( ), _connInfo.port.c_str( ), &createData, &_connInfo.info );
		if( S_OK == result )
		{
			return true;
		}

		std::string errString;
		WSAErrorString( WSAGetLastError( ), errString );
		printf( "createInfo Fail : %s \n", errString.c_str( ) );
		int winerror = GetLastError( );

		return false;
	}

	static bool	connect( _Inout_ connectionInfo& _connInfo )
	{

		_connInfo.sock = socket( _connInfo.info->ai_family, _connInfo.info->ai_socktype, _connInfo.info->ai_protocol );
		if( INVALID_SOCKET == _connInfo.sock )
		{
			_connInfo.err.errWSA = WSAGetLastError( );
			_connInfo.err.errOS = GetLastError( );
			return false;
		}

		int result = connect( _connInfo.sock, _connInfo.info->ai_addr, ( int ) _connInfo.info->ai_addrlen );
		if( SOCKET_ERROR == result )
		{
			_connInfo.err.errWSA = WSAGetLastError( );
			_connInfo.err.errOS = GetLastError( );
			int err = 0;
			if( WSAECONNREFUSED == ( err = WSAGetLastError( ) ) )
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

	static bool	disconnect( _Inout_ connectionInfo& _connInfo )
	{
		if( INVALID_SOCKET != _connInfo.sock )
		{
			shutdown( _connInfo.sock, SD_BOTH );
			closesocket( _connInfo.sock );
			_connInfo.sock = INVALID_SOCKET;
		}
		return false;
	}

	static int	send( _Inout_ connectionInfo& _connInfo )
	{
		int totalSend = 0;

		for( Header* packet : _connInfo.sendBuff )
		{
			::send( _connInfo.sock, ( char* ) packet, packet->length, 0 );
			totalSend += packet->length;
		}

		return totalSend;
	}

	static int	recv( _Inout_ connectionInfo& _connInfo )
	{
		int			Receive = 0;
		uint64_t	length = 0;
		char* bufferStart = nullptr;

		if( _connInfo.recvBuff.get_buffer( &bufferStart, &length ) )
		{
			Receive = ::recv( _connInfo.sock, bufferStart, length, 0 );
			if( 0 < Receive )
				_connInfo.recvBuff.commit( Receive );
		}

		return Receive;
	}

	static int	Recorver( _Inout_ connectionInfo& _connInfo )
	{
		return 0;
	}

	//=====================================================================================================================

	static bool	createInfo( _Inout_ bindingInfo& _connInfo )
	{
		sockaddr_storage storage;
		memset( &storage, 0, sizeof storage );
		// The socket address to be passed to bind
		_connInfo.info.sin_family = AF_INET;
		_connInfo.info.sin_addr.s_addr = htonl( INADDR_ANY );
		_connInfo.info.sin_port = htons( _connInfo.port );

		return true;
	}

	static bool	bind( _Inout_ bindingInfo& _connInfo )
	{
		int returnValue = 0;
		int loopCounter = 0;

		// 새로 바인딩하면 초기화해버리기
		if( INVALID_SOCKET != _connInfo.sock )
		{
			closesocket( _connInfo.sock );
			_connInfo.sock = INVALID_SOCKET;
			return false;
		}

		do
		{
			if( 5 < loopCounter )
			{
				int winerror = GetLastError( );
				// exception 객체 생성되면 throw하면서 에러 정보 송신
				return false;
			}

			_connInfo.sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			if( INVALID_SOCKET == _connInfo.sock )
			{
				continue;
			}

			returnValue = bind( _connInfo.sock, ( SOCKADDR* ) &_connInfo.info, sizeof( _connInfo.info ) );
			if( returnValue == SOCKET_ERROR )
			{
				int err = 0;
				if( WSAECONNREFUSED == ( err = WSAGetLastError( ) ) )
				{
					closesocket( _connInfo.sock );
					_connInfo.sock = INVALID_SOCKET;
					continue;
				}
				printf( "connect failed with error: %d\n", err );
				return INVALID_SOCKET;
			}
		} while( S_OK != returnValue );

		FD_ZERO( &_connInfo.selectSet );

		if( SOCKET_ERROR == listen( _connInfo.sock, 5 ) )
			return false;

		FD_SET( _connInfo.sock, &_connInfo.selectSet );

		return true;
	}

	static int	aSelect_Read( _Inout_ bindingInfo& _connInfo )
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		_connInfo.readSet = _connInfo.selectSet;
		return  select( _connInfo.sock + 1, &_connInfo.readSet, NULL, NULL, &tv );
	}

	static int	aSelect_Write( _Inout_ bindingInfo& _connInfo )
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		_connInfo.writeSet = _connInfo.selectSet;
		return  select( _connInfo.sock + 1, NULL, &_connInfo.writeSet, NULL, &tv );
	}

	static int	aSelect_RW( _Inout_ bindingInfo& _connInfo )
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		_connInfo.readSet = _connInfo.selectSet;
		_connInfo.writeSet = _connInfo.selectSet;
		return  select( _connInfo.sock + 1, &_connInfo.readSet, &_connInfo.writeSet, NULL, &tv );
	}

	static bool	release( _Inout_ bindingInfo& _connInfo )
	{

		for( std::pair<const SOCKET, sessionInfo>& iter : _connInfo.sessionMap )
		{
			const SOCKET& sock = iter.first;
			closesocket( sock );
			FD_CLR( sock, &_connInfo.selectSet );
		}

		_connInfo.sessionMap.clear( );

		if( INVALID_SOCKET != _connInfo.sock )
		{
			shutdown( _connInfo.sock, SD_BOTH );
			closesocket( _connInfo.sock );
			_connInfo.sock = INVALID_SOCKET;
		}

		return true;
	}

	static bool	accept( _Inout_ bindingInfo& _connInfo )
	{
		SOCKET newThing = ::accept( _connInfo.sock, NULL, NULL );
		if( INVALID_SOCKET != newThing )
		{
			_connInfo.sessionMap.insert( std::make_pair( newThing, sessionInfo( ) ) );
			FD_SET( newThing, &_connInfo.selectSet );
			return false;
		}
		return true;
	}

	static uint64_t	send( _Inout_ bindingInfo& _connInfo )
	{
		uint64_t result = 0;
		Ping temp_ping;
		std::vector<SOCKET> removeList;
		for( uint64_t count = 0; _connInfo.writeSet.fd_count > count; ++count )
		{
			SOCKET currSock = _connInfo.writeSet.fd_array[ count ];

			if( FD_ISSET( currSock, &_connInfo.writeSet ) )
			{
				std::unordered_map<SOCKET, sessionInfo>::iterator session = _connInfo.sessionMap.find( currSock );
				if( 0 != session->second.sendBuff.size( ) )
				{
					for( Header* header : session->second.sendBuff )
					{
						int Result = ::send( currSock, ( char* ) header, header->length, 0 );
						if( -1 == Result )
						{
							removeList.push_back( currSock );
							continue;
						}

						result += header->length;
						printf( "send length : %lld  / data : %s", header->length, ( ( ChatBuffer* ) header )->buffer );

						delete header;
					}

					session->second.sendBuff.clear( );
				}
			}
		}

		for( SOCKET del : removeList )
		{
			FD_CLR( del, &_connInfo.writeSet );
			closesocket( del );
			_connInfo.sessionMap.erase( del );
		}

		// totalSend
		return result;
	}

	static uint64_t recv( _Inout_ bindingInfo& _connInfo )
	{
		uint64_t result = 0;
		std::vector<SOCKET> removeList;
		for( uint64_t count = 0; _connInfo.readSet.fd_count > count; ++count )
		{
			SOCKET currSock = _connInfo.readSet.fd_array[ count ];

			if( FD_ISSET( currSock, &_connInfo.readSet ) )
			{
				if( currSock == _connInfo.sock )
				{
					accept( _connInfo );
				}
				else
				{
					std::unordered_map<SOCKET, sessionInfo>::iterator session = _connInfo.sessionMap.find( currSock );
					if( _connInfo.sessionMap.end( ) != session )
					{
						char* buffer = nullptr;
						uint64_t length = 0;
						if( session->second.recvBuff.get_buffer( &buffer, &length ) )
						{
							int readCount = ::recv( currSock, buffer, length, 0 );
							session->second.recvBuff.commit( readCount );
							result += readCount;
							session->second.recvBuff.get_data( &session->second.sendBuff );
						}
					}
				}
			}
		}

		for( SOCKET del : removeList )
		{
			FD_CLR( del, &_connInfo.readSet );
			closesocket( del );
			_connInfo.sessionMap.erase( del );
		}

		return result;
	}

	static int	Recorver( _Inout_ bindingInfo& _connInfo )
	{
		if( bind( _connInfo ) )
		{
			return 0;
		}

		return -1;
	}
}
