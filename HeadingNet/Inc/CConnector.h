#pragma once

namespace Heading
{
	class CConnector
	{
	public:
		CConnector();
		~CConnector();

		void newConnect( connectionInfo& _info );



		// Pong Event 하면서 Socket 검사.
		void onPing( Header* _pong );

	private:

	};
}

