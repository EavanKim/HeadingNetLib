#pragma once

namespace Heading
{
	// 연결을 관리하는 객체
	class CLoginSocket : public CSimpleSocket
	{
	public:
					CLoginSocket	( SOCKET _sock );
					~CLoginSocket	( );

		WSAEVENT	getEvent		( );
		SOCKET		getSock			( );

	private:
		WSAEVENT m_event = INVALID_HANDLE_VALUE;
	};
}

