#include "pch.h"

namespace Heading
{
	void CChatProcess::Do_Process( SessionData* _data )
	{
		switch( _data->m_message->type )
		{
		case 1000:
		{
			// ���ο� ä�� �õ�
			// ���� �� �ִ� ��ġ
			ChatBuffer* cast = static_cast<ChatBuffer*>(_data->m_message);
			m_chatLog.push(ChatLogNode(cast->buffer));

			// DB�� ���� �ܼ� �����̹Ƿ�
			// �� �ܼ��ϰ� 1000���� �Ѿ�� ���ִ� ������ �մϴ�.
			if( 1000 < m_chatLog.size() )
				m_chatLog.pop();
		}
			break;
		case 1001:
		{
			// ������Ʈ �õ�

		}
			break;
		}
	}
}
