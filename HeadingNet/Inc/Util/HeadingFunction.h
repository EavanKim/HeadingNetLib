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

	enum E_WaitEvent_Result
	{
		E_Wait_OK,
		E_Wait_Delayed,
		E_Wait_Reset_WSA,
		E_Wait_Reset_SOCK,
		E_Wait_Reset_EVENTS_ARRAY,
		E_Wait_MEM_FULL,
		E_Wait_Max
	};

	static E_WaitEvent_Result WaitObjectCheck( DWORD ret )
	{
		switch( ret )
		{
		case WSA_WAIT_FAILED:
		{
			switch( WSAGetLastError( ) )
			{
			case WSANOTINITIALISED: // WSAStartup이 실패한 것이므로 여기까지 들어온 것 자체가 망한 상황.
				return E_Wait_Reset_WSA;
			case WSAENETDOWN: // network subsystem이 실패. 다시 처음부터 연결 해 보자
				return E_Wait_Reset_SOCK;
			case WSAEINPROGRESS: // blocking Windows sockets 1.1 call is in progress, 콜백 함수안에 갇힌 상황
				return E_Wait_Max; // 이건 좀 더 공부해보기
			case WSA_NOT_ENOUGH_MEMORY: // 메모리 부족. FULL Memory 로그만 남기고 크래시하자
				return E_Wait_MEM_FULL; 
			case WSA_INVALID_HANDLE: // event 배열에 한 개 이상의 이벤트가 정상이 아님.
				return E_Wait_Reset_EVENTS_ARRAY; // WSAEVENT 들은 배열 정보 다시 정리하기
			case WSA_INVALID_PARAMETER: // event 갯수를 잘못 넣은 것 같다 다시 확인하자.
				return E_Wait_Reset_EVENTS_ARRAY; // WSAEVENT 들은 배열 정보 다시 정리하기
			default:
				return E_Wait_Max;
			}
		}
		case WSA_WAIT_IO_COMPLETION:
		case WSA_WAIT_TIMEOUT:
			return E_Wait_Delayed; // 여기까진 대기할 때 발생할 수 있는 정상상태
		default:
			return E_Wait_OK;
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
