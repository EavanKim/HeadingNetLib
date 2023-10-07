#include "pch.h"

namespace Heading
{
	CPacketDecoder::CPacketDecoder(packetSize_t _size, char* _data)
		: m_size( _size )
		, m_seek( 0 )
	{
		memcpy_s( m_data, MAXIMUM_PACKET_DATA_LENGTH, _data, _size );
	}

	CPacketDecoder::~CPacketDecoder()
	{

	}

	bool CPacketDecoder::checkValidation()
	{
		packetSize_t seek = 0;

		while ( m_size > seek )
		{
			packetHeader_t* headerData = (packetHeader_t*)m_data[seek];
			seek += headerData->length;
		}

		// 데이터 끝을 지속 탐색 한 결과가 데이터 전체 사이즈와 맞다면
		// 모든 데이터를 올바르게 파싱 가능합니다.
		if ( m_size == seek )
		{
			return true;
		}

		return false;
	}

	bool CPacketDecoder::getData(packetHeader_t*& _get)
	{
		if ( m_size > m_seek )
		{
			_get = (packetHeader_t*) & m_data[m_seek];
			m_seek += _get->length;

			return true;
		}

		return false;
	}
}
