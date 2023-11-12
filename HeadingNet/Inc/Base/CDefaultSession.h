#pragma once

namespace Heading
{
	class CDefaultSession : public ISession
	{
	public:
		const SOCKADDR_IN*	GetInfo	( )						override;

		void				Set		( ISocket*	_sock )		override;
		void				Set		( IMessage&	_message )	override;
		void				Get		( IMessage&	_message )	override;

	private:
		ISocket* m_sock = nullptr;
	};
}

