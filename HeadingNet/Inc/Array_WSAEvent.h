#pragma once

namespace Heading
{
	class Array_WSAEvent
	{
	public:
		bool add( WSAEVENT _newEvent );
		void remove( WSAEVENT _delete );
		void clear();
		uint8_t size( );
		WSAEVENT* operator*( );
		WSAEVENT operator[]( DWORD _index );

	private:
		uint8_t m_size = 0;
		WSAEVENT m_events[ WSA_MAXIMUM_WAIT_EVENTS ] = {};
	};
}

