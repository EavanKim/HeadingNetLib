#include "pch.h"

namespace Heading
{
	void CChatProcess::Do_Process( SessionData* _data )
	{
		switch( _data->m_message->type )
		{
		case 1000:
		{
			// 새로운 채팅 시도
			// 터질 수 있는 위치
			ChatBuffer* cast = static_cast<ChatBuffer*>(_data->m_message);
			m_chatLog.push(ChatLogNode(cast->buffer));
		}
			break;
		case 1001:
		{
			// 업데이트 시도

		}
			break;
		}
	}
}
