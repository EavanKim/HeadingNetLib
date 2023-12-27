#pragma once

namespace Heading
{
	class CSession_Base : public ISession
	{
	public:
		virtual void				Release	(	)						override;
		virtual void				Reset	(	)						override;

		virtual const SOCKADDR_IN*	GetInfo	(	)						override;

		virtual void				Set		( ISocket*		_sock )		override;
		virtual bool				Set		( IMessage*		_message )	override;
		virtual void				Get		( IMessage*&	_message )	override;

		virtual void				Flush	(	)						override;
		virtual void				Update	(	)						override;

	private:
		std::queue<IMessage*> m_sendQueue;
		ISocket* m_sock = nullptr;
	};
}

