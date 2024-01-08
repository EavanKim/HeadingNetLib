#pragma once

namespace Heading
{
	class CSession_Client : public CSession_Base
	{
	public:
		CSession_Client(ISocket* _socket);
		virtual ~CSession_Client();

	private:
	};
}