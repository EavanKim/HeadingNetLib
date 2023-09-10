#include "pch.h"

namespace Heading
{
	CLoginWaiter::CLoginWaiter(CAccepter* _accepter)
	{
		m_array.add(_accepter->Get_Event());
	}

	CLoginWaiter::~CLoginWaiter()
	{
	}

	void CLoginWaiter::preSelect(WSAEVENT** _ptr, DWORD* count)
	{
		*_ptr = *m_array;
		*count = m_array.size();
	}

	void CLoginWaiter::updateAccepter(CAccepter* _accepter)
	{
		m_array.swap(0, _accepter->Get_Event());
	}

	void CLoginWaiter::addNewSocket( SOCKET _new )
	{
		if ( WSA_MAXIMUM_WAIT_EVENTS == m_sockMap.size() )
		{
			// 연결을 못 받습니다.

			// 지금은 이유를 알려주지 않고 바로 닫습니다.
			closesocket(_new);

			return;
		}

		CLoginSocket* sock = new CLoginSocket( _new );
		sock->m_state.setState( true, HS_CLOSETROUGH );

		m_sockMap.insert( std::make_pair( sock->getEvent(), sock ) );
	}

	void CLoginWaiter::getLoginData(int _index, void*& _data)
	{
		WSAEVENT targetEvt = m_array[ _index ];
		auto target = m_sockMap.find( targetEvt );
		if ( m_sockMap.end() != target )
		{
			CLoginSocket* sock = target->second;
			Buffer buf;
			sock->recv(buf);

			std::string string = ( ( SendStruct<0, 1>* )buf.get_data() )->buffer;

		}
	}

	SOCKET CLoginWaiter::removeSocket(int _index)
	{
		WSAEVENT targetEvt = m_array[ _index ];
		auto target = m_sockMap.find( targetEvt );
		if ( m_sockMap.end() != target )
		{
			CLoginSocket* sock = target->second;
			SOCKET result = sock->getSock();
			delete sock;
			return result;
		}

		return SOCKET_ERROR;
	}
}