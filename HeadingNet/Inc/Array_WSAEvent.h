#pragma once

namespace Heading
{
	// Event 제거 할 때 fast 형식 처리를 위한 배열 클래스
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
		WSAEVENT m_events[ WSA_MAXIMUM_WAIT_EVENTS ] = {}; // 일단 기본 옵션인 상태에선 무조건 64개
	};
}

