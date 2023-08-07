#include "pch.h"

namespace Heading
{
	CPacketHandler::CPacketHandler( void( *_nullCallback )( IN Heading::CClientSession* _session, IN Heading::Header* _packet ) )
		: m_nullCallback( _nullCallback )
	{
		ZeroMemory(m_callbackArray, sizeof(PacketCallback) * PACKET_HANDLER_SIZE);
	}

	void CPacketHandler::Do_Process( CClientSession* _session, Header* _packet )
	{
		if( nullptr != m_callbackArray[ _packet->type ].callback )
			m_callbackArray[ _packet->type ].callback( _session, _packet );
		else
		{
			// 혹시 의도하지 않은 null 동작 관련 처리동작을 하고 있다면 아래 함수 포인터로 처리합니다.
			if( nullptr != m_nullCallback )
				m_nullCallback( _session, _packet );
		}
	}

	bool CPacketHandler::Create_Header( int _ePacketType, char* _ptr, char* _data, int _size )
	{
		bool result = false;

		if( m_size > _ePacketType )
		{
			int dataLength = m_callbackArray[ _ePacketType ].size - sizeof(Header);
			if( dataLength > _size )
			{
				Header* headPtr = reinterpret_cast<Header*>(_ptr);
				char* dataPtr = _ptr + sizeof(Header);

				*(const_cast<uint64_t*>(&headPtr->type)) = _ePacketType;
				*(const_cast<int*>(&headPtr->length)) = m_callbackArray[ _ePacketType ].size;

				memcpy_s(dataPtr, dataLength, _data, _size);
			}
		}

		return result;
	}
}
