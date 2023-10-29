#pragma once

namespace Heading
{
	struct ISession
	{
		virtual void				Set( Heading::connectionInfo& _info )	= 0;

		virtual SOCKADDR_IN&		GetInfo	( )								= 0;

		// 보낼때는 메세지를 예약도 할 수 있도록 메세지 자체를 받아서 처리
		virtual void				Set		( IMessage& _message )			= 0;

		// 꺼내는 내용은 현재 활성화 된 버퍼 내 메세지만 참조하기.
		// 즉, Recv가 발생하여 처리하는 한 번의 Function 내에서만 올바른 정보를 잠시 조회만 하는 데이터.
		virtual IMessage&			Get		( )								= 0;
	};
}