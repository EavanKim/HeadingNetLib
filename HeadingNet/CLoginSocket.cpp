#include "pch.h"

namespace Heading
{
	CLoginSocket::CLoginSocket(SOCKET _sock)
		: CSimpleSocket( _sock )
	{
		m_event = WSACreateEvent();
		// 로그인 정보가 들어오는지만 볼 것이고, 시간이 오래된 것은 연결을 날릴 것 이므로
		// 읽기와 끊김 여부만 처리합니다.
		WSAEventSelect(_sock, m_event, FD_READ | FD_CLOSE | FD_CONNECT);
	}

	CLoginSocket::~CLoginSocket()
	{
		WSACloseEvent(m_event);
	}

	WSAEVENT CLoginSocket::getEvent()
	{
		return m_event;
	}

	SOCKET CLoginSocket::getSock()
	{
		return m_sock;
	}
}
