#pragma once

namespace Heading
{
	// vPtr은 어차피 패킷으로 보낸다는 것 자체가 말이 안되고
	// 내용물만 처리하는 것이 올바르니
	// 해당 내용물을 받거나 세팅해서 보내거나 받기만 하기
	struct IMessage
	{
		virtual ~IMessage() {}

		virtual unsigned int	GetType( )						= 0;
		virtual unsigned int	GetSize( )						= 0;
		virtual char*			GetData( )						= 0;
		virtual void			SetType( unsigned int	_type ) = 0;
		virtual void			SetData( char*			_data
										, unsigned int	_size )	= 0;
	};
}