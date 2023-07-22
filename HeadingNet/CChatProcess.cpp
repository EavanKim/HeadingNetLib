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

			// DB가 없는 단순 구조이므로
			// 방어도 단순하게 1000개를 넘어가면 없애는 구조로 합니다.
			if( 1000 < m_chatLog.size() )
				m_chatLog.pop();
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
