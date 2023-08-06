#include "pch.h"

namespace Heading
{
	CPacketHandler::CPacketHandler( bool( *_nullCallback )( CClientSession* _session, Header* _packet ) )
		: m_nullCallback( _nullCallback )
	{
		ZeroMemory(m_callbackArray, sizeof(PacketCallback) * PACKET_HANDLER_SIZE);
	}

	void CPacketHandler::Do_Process( CClientSession* _session, Header* _packet )
	{
		m_callbackArray[_packet->type]._callback(_session, _packet);
	}
}
