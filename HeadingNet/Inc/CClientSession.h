#pragma once

namespace Heading
{
	class CClientSession : public CEventBaseSession
	{
	public:
		CClientSession( SOCKET _sock );
		virtual ~CClientSession();

		virtual void Update() = 0;
	private:
	};

	typedef std::unordered_map<SOCKET, CClientSession*> ChatSessionMap;
	typedef std::unordered_map<WSAEVENT, CClientSession*> ChatSessionEventMap;
}

