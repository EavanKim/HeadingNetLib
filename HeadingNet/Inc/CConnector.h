#pragma once

namespace Heading
{
	class CConnector
	{
	public:
		CConnector();
		~CConnector();

		void newConnect( connectionInfo& _info );



		// Ping�� ���� Pong Event �ϸ鼭 Socket �˻�.
		void onPing( );

	private:

	};
}

