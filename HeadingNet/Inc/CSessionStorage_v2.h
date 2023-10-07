#pragma once

//==================================================================
// 
// 링버퍼 궁리해보기
// 
// 문제점
//		버퍼를 넘겨 줄 때는 반드시 연속된 주소의 메모리를 줘야하는데,
//		최대 대기량에 미치지 못 하는 양만 남았을 경우 어떻게 대처할 것인가?
// 
// 어차피 데이터의 트랜잭션을 위해 Recv를 1번밖에 하지 못한다면
// 2회 이상의 맥시멈 수치만 보존하면 되지 않을까...?
// 
//==================================================================

#define RINGBUFFER_MAXIMUM MAXIMUM_PACKET_DATA_LENGTH * 3
#define RINGBUFFER_LIMIT MAXIMUM_PACKET_DATA_LENGTH * 2

typedef uint16_t packetSize_t;
typedef uint16_t bufferIndex_t;

namespace Heading
{
	class CSessionStorage_v2
	{
	public:
		CSessionStorage_v2();
		~CSessionStorage_v2();

		char* getBuffer();
		void commitBuffer(packetSize_t _size);
		packetSize_t getSize();
		void getData( OUT packetHeader_t*& _data);

	private:
		// 유효한 데이터의 시작지점
		bufferIndex_t m_start = 0;
		// 유효한 데이터의 끝 지점
		bufferIndex_t m_end = 0;
		// 개념적인 데이터의 마지막은 m_end이지만
		// 완충지를 만들어야만 합니다.
		// 왜냐하면, 받아온 버퍼 데이터가 애매하게 끝에서 남아버리면
		// 링 버퍼 구조상 버퍼 마지막까지로 수신버퍼를 제약받아서
		// 다운로드 하는 데이터를 2회로 쪼개버리는 아주 좋지않은 IO가 발생하기 때문
		// m_bufferEnd는 기본적으로 버퍼 2개분을 커버하고 있지만
		// 필요에 따라 3번째 분량의 끝 까지 늘어날 수 있고
		// 그렇게 되면 복사할 위치가 바뀌므로 갖고있는 변수입니다.
		// 이를 통해서 매번 MAXIMUM_PACKET_DATA_LENGTH 길이의 recv를 보장합니다.
		// getBuffer 할 때 시작지점으로부터의 남은 거리로 매번 갱신됩니다.
		bufferIndex_t m_bufferEnd = RINGBUFFER_LIMIT;
		char m_buffer[RINGBUFFER_MAXIMUM];
	};
}
