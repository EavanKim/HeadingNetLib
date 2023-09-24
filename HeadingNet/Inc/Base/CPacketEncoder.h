#pragma once

namespace Heading
{
	class CPacketEncoder
	{
	public:
		CPacketEncoder();
		~CPacketEncoder();
		
		// 굳이 클래스를 만들지 않고
		// 전송 가능한 데이터로 변경하기
		// 명시적으로 바이트 데이터를 
		// 어떻게 다루는지가 보이도록 템플릿 처리 배제
		bool addData(Header* _data);

		// 전송 명령에 사용할 리턴
		const packetSize_t const getSize();
		const char* const getData();

	private:
		packetSize_t m_seek = 0;
		char m_data[MAXIMUM_PACKET_DATA_LENGTH];
	};
}

