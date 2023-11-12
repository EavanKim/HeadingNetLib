#pragma once

namespace Heading
{
	class CDefaultSession : public ISession
	{
	public:
		SOCKADDR_IN& GetInfo() override;

		void Set(connectionInfo& _info) override;
		void Set(IMessage& _message) override;
		IMessage& Get() override;

	private:

	};
}

