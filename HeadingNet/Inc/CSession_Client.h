#pragma once

namespace Heading
{
	class CConnSession : public CSession_Base
	{
	public:
		CConnSession();
		~CConnSession();

		// ISession을(를) 통해 상속됨
		virtual void Release() override;
		virtual void Reset() override;
		const SOCKADDR_IN* GetInfo() override;
		void Set(ISocket* _sock) override;
		bool Set(IMessage* _message) override;
		void Get(IMessage*& _message) override;
		void Update() override;

	private:
		ISocket* m_sock = nullptr;

	};
}