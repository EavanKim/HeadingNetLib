#pragma once

namespace Heading
{
	class CConnector
	{
	public:
		CConnector();
		~CConnector();

		void newConnect( connectionInfo& _info );

		void onPing( );

	private:

	};
}

