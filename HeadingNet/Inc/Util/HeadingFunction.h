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
}
