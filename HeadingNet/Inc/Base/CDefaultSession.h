#pragma once

namespace Heading
{
	class CDefaultSession : public ISession
	{
	public:
		const SOCKADDR_IN*	GetInfo	( )						override;

		void				Set		( ISocket*		_sock )		override;
		bool				Set		( IMessage*		_message )	override;
		void				Get		( IMessage*&	_message )	override;

		void Update() override;

	private:
		std::queue<IMessage*> m_sendQueue;
		ISocket* m_sock = nullptr;
	};
}

