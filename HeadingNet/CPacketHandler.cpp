#include "pch.h"

namespace Heading
{
	CPacketHandler::CPacketHandler( packet_Callback _nullCallback )
		: m_nullCallback( _nullCallback )
	{
		ZeroMemory(m_callbackArray, sizeof(PacketCallback) * PACKET_HANDLER_SIZE);
	}

	void CPacketHandler::Do_Process( CClientSession* _session, packetHeader_t* _packet )
	{
		if( nullptr != m_callbackArray[ _packet->type ].callback )
			m_callbackArray[ _packet->type ].callback( _session, _packet );
		else
		{
			if( nullptr != m_nullCallback )
				m_nullCallback( _session, _packet );
		}
	}

	bool CPacketHandler::Create_Header( int _ePacketType, char* _ptr, char* _data, int _size )
	{
		bool result = false;

		if( m_size > _ePacketType )
		{
			int dataLength = m_callbackArray[ _ePacketType ].size - sizeof(packetHeader_t);
			if( dataLength > _size )
			{
				packetHeader_t* headPtr = reinterpret_cast<packetHeader_t*>(_ptr);
				char* dataPtr = _ptr + sizeof(packetHeader_t);

				*(const_cast<uint64_t*>(&headPtr->type)) = _ePacketType;
				*(const_cast<packetSize_t*>(&headPtr->length)) = m_callbackArray[ _ePacketType ].size;

				memcpy_s(dataPtr, dataLength, _data, _size);
			}
		}

		return result;
	}
}
