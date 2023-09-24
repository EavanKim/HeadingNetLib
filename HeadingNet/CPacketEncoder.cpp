#include "pch.h"

namespace Heading
{
	CPacketEncoder::CPacketEncoder()
	{

	}

	CPacketEncoder::~CPacketEncoder()
	{

	}

	bool CPacketEncoder::addData(Header* _data)
	{
		packetSize_t leftSize = MAXIMUM_PACKET_DATA_LENGTH - m_seek;

		// 사이즈가 작다면 무조건 성공시키기
		if ( leftSize > _data->length )
		{
			memcpy_s( m_data, leftSize, _data, _data->length );
			m_seek += _data->length;

			return true;
		}

		return false;
	}

	const packetSize_t const CPacketEncoder::getSize()
	{
		return m_seek;
	}

	const char* const CPacketEncoder::getData()
	{
		// 암호화가 필요하다면 외부에서 암호화 하고 사이즈도 덮어쓰기.
		return m_data;
	}
}