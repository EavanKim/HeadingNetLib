#pragma once

namespace Heading
{
	class CAccepter : public CEventBaseSession
	{
	public:
		CAccepter( uint16_t	_listenPort );
		virtual ~CAccepter( );

		bool Bind();

		SOCKET CreateConnect( sockaddr_in& _info );

		uint16_t Get_Port();

	private:
		uint16_t	m_listenPort	= 0;
	};

	typedef std::unordered_map<SOCKET, CAccepter*> AcceptSessionMap;
	typedef std::unordered_map<WSAEVENT, CAccepter*> AcceptSessionEventMap;
}

