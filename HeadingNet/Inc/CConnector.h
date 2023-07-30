#pragma once

namespace Heading
{
	class CConnector
	{
	public:
		CConnector();
		~CConnector();

		void newConnect( connectionInfo& _info );



		// Ping에 대해 Pong Event 하면서 Socket 검사.
		void onPing( );

	private:

	};
}

