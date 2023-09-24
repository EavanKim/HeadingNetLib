#pragma once

namespace Heading
{
	class Array_WSAEvent
	{
	public:
		bool add( WSAEVENT _newEvent );
		void remove( WSAEVENT _delete );
		// Accepter 동작으로 0번을 고정 Accept 번호로 쓰기위한 추가함수.
		WSAEVENT swap( int _index, WSAEVENT _newTarget );
		void clear();
		uint8_t size( );
		WSAEVENT* operator*( );
		WSAEVENT operator[]( DWORD _index );

	private:
		uint8_t m_size = 0;
		WSAEVENT m_events[ WSA_MAXIMUM_WAIT_EVENTS ] = {};
	};
}

