#include "HeadingNetHeader.h"

int main()
{
	WSAData Data;
	Heading::start( Data );

	//===================================================================================================================================================================

	try
	{
		bool IsServerLive = true;
		//================================================================================================================================================================

		Heading::bindingInfo mainServer;
		Heading::bindingInfo broadCastServer;

		ZeroMemory( &mainServer, sizeof( Heading::bindingInfo ) );
		ZeroMemory( &broadCastServer, sizeof( Heading::bindingInfo ) );

		Heading::createInfo( mainServer );
		Heading::createInfo( broadCastServer );

		Heading::bind( mainServer );
		Heading::bind( broadCastServer );

		while( IsServerLive )
		{
			Heading::aSelect_Read( mainServer );
			Heading::aSelect_RW( broadCastServer );

			Heading::recv( mainServer );
			Heading::recv( broadCastServer );
			Heading::send( broadCastServer );
		}

		//================================================================================================================================================================
	}
	catch( ... )
	{
		// ũ�����ϸ� �׾������, �ٷ� �״°� ������ �츮�� �� ���� ��������... ��� �� ���� ������������ �� �� �ֵ��� �մϴ�.

		TCHAR* message = nullptr;
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					   nullptr,
					   GetLastError(),
					   MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
					   ( TCHAR* )&message,
					   0,
					   nullptr );

		wprintf( L" LastError String : %s", message );
		LocalFree( message );
	}

	//===================================================================================================================================================================

	Heading::end();
}