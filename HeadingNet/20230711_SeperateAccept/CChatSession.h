#pragma once

namespace Heading
{
	class CChatSession : public CClientSession
	{
	public:
		CChatSession( SOCKET _sock );
		virtual ~CChatSession();

	private:


		// Inherited via CClientSession
		virtual void Update( ) override;

	};
}

