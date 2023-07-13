#pragma once

namespace Heading
{
	class CBroadCastSession : public CClientSession
	{
	public:
		CBroadCastSession( SOCKET _sock );
		virtual ~CBroadCastSession();

	private:

		// Inherited via CClientSession
		virtual void Update( ) override;

	};
}

