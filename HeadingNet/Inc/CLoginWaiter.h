#pragma once

namespace Heading
{
	class CLoginWaiter
	{
	public:
		CLoginWaiter( CAccepter* _accepter );
		~CLoginWaiter( );

		void preSelect(WSAEVENT** _ptr, DWORD* count);

		// Accepter Socket 문제 발생 시 새로 만들어서 갱신하기 위한 조치입니다.
		// Accepter는 딱히 내부에 저장하지 않고 0번 Event로만 사용합니다.
		void updateAccepter( CAccepter* _accepter );

		void addNewSocket( SOCKET _new );
		void getLoginData( int _index, void*& _data );
		SOCKET removeSocket( int _index );

	private:
		Array_WSAEvent m_array;
		std::unordered_map<WSAEVENT, CLoginSocket*> m_sockMap;
	};
}