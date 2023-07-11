#pragma once

namespace Heading
{
	class CAcceptSession : public CEventBaseSession
	{
	public:
		CAcceptSession( uint16_t	_listenPort );
		virtual ~CAcceptSession( );

		bool Bind();

		SOCKET CreateConnect( sockaddr_in& _info );

		uint16_t Get_Port();

	private:
		uint16_t	m_listenPort	= 0;
	};

	typedef std::unordered_map<SOCKET, CAcceptSession*> AcceptSessionMap;
	typedef std::unordered_map<WSAEVENT, CAcceptSession*> AcceptSessionEventMap;
}

