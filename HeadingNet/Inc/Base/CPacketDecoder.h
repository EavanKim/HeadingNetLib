#pragma once

namespace Heading
{

	class CPacketDecoder
	{
	public:
		CPacketDecoder(packetSize_t _size, char* _data);
		~CPacketDecoder();

		bool checkValidation();
		bool getData(packetHeader_t*& _get);

	private:
		packetSize_t m_seek = 0;
		const packetSize_t m_size = 0;
		char m_data[MAXIMUM_PACKET_DATA_LENGTH];
	};
}
