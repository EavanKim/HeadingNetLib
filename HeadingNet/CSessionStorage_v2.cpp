#include "pch.h"

namespace Heading
{
	CSessionStorage_v2::CSessionStorage_v2()
	{
	}

	CSessionStorage_v2::~CSessionStorage_v2()
	{
	}

	char* CSessionStorage_v2::getBuffer()
	{
		// 혹시 마지막 데이터로부터 새로 전해줄 데이터가
		// 예정된 마지막을 넘어갔다면
		if ( RINGBUFFER_LIMIT < ( MAXIMUM_PACKET_DATA_LENGTH + m_end ) )
		{
			// 밀려난 만큼 마지막을 밀어냅니다.
			m_bufferEnd = MAXIMUM_PACKET_DATA_LENGTH + m_end;
		}
		else
		{
			// 만약 마지막 데이터로부터 새로 꺼내줄 데이터가 한계치를 넘지 않았는데
			// 버퍼 유효범위가 제한사항 안으로 돌아왔다면
			// 다시 한계치를 제한치로 돌려둡니다.
			if( (RINGBUFFER_LIMIT > m_start) && (RINGBUFFER_LIMIT > m_end) )
				m_bufferEnd = RINGBUFFER_LIMIT;
		}

		// 데이터 끝자락부터 MAXIMUM_PACKET_DATA_LENGTH 을 확정적으로 돌려줍니다.
		return &m_buffer[m_end];
	}

	void CSessionStorage_v2::commitBuffer(packetSize_t _size)
	{
		// 마지막 유효 범위에서 늘어난다는 사실은 확정이므로 일단 늘려놓고
		m_end += _size;

		// 할당해줬던 버퍼가 리밋을 넘어갔었다면
		if ( RINGBUFFER_LIMIT < m_bufferEnd )
		{
			// 마지막을 최종 데이터 사이즈로 변경해주고
			m_bufferEnd = m_end;
			// 데이터는 다시 처음부터 작성되도록 넘깁니다.
			// 이 이후에 비었는가 체크를 시도해서 m_start까지의 거리가 MAXIMUM_PACKET_DATA_LENGTH을 확보할 수 없다면
			// WouldBlock 상태가 되는것이 의도입니다.
			m_end = 0;
		}
	}

	packetSize_t CSessionStorage_v2::getSize()
	{
		packetSize_t result = 0;

		// 버퍼가 한계를 넘어가서 끝이 초기화 되었다면
		if ( 0 == m_end )
		{
			// 한계치까지의 사이즈가 데이터 사이즈
			result = m_bufferEnd - m_start;
		}
		else
		{
			// 혹시 초기화 상태가 아니라면
			// 데이터 처리 전이어서 혹시 데이터 시작부가 끝 보다 더 뒤에 있는 경우에는
			if( m_end < m_start )
				result = (m_bufferEnd - m_start) + m_end; // 시작지부터 버퍼 끝까지의 거리에 추가로 남은 데이터 거리를 알려주고
			else
				result = m_end - m_start; // 그렇지 않았다면 순수하게 시작부터 끝 까지의 사이즈를 알려줍니다.
		}

		return result;
	}

	void CSessionStorage_v2::getData( OUT packetHeader_t*& _data)
	{
		_data = nullptr;

		bufferIndex_t bufferSize = getSize();
		if ( sizeof(packetHeader_t) < bufferSize )
		{
			uint8_t processType = 0;
			bufferIndex_t packetLength = 0;
			//혹시 수신이 쪼개졌을때를 대비해서 무작정 일렬로 받지않고 조건을 체크합니다.
			if ( sizeof(packetHeader_t) < ( m_bufferEnd - m_start ) )
			{
				// 어찌되었건 마지막까지의 거리가 멀쩡하므로 packetHeader_t를 꺼내옵니다
				packetLength = ((packetHeader_t*)&m_buffer[m_start])->length;
			}
			else
			{
				// 알아내기 어려운 네트워크 사정으로 헤더 데이터가 쪼개졌습니다!
				// 길이 데이터를 어떤 형태로든 추론해야합니다.
				// packetHeader_t 시작이 길이니까, 설마 
			}

			// 사이즈를 먼저 검사했으므로 packet의 length 데이터가 오염될 일은 없습니다.
			// 마저 읽을 데이터 사이즈가 유효한지 확인합니다.
			if ( bufferSize > packetLength )
			{
				// 여기가 유효한 시점에서 데이터는 확정적으로 복제 될 예정이므로 할당을 시작합니다.
				// packetHeader_t는 구조체이므로 메모리 할당에 두려움 없이 malloc을 사용합니다.
				// 클래스가 된다면, new를 써야만 합니다.
				_data = (packetHeader_t*)malloc(packetLength);

				switch ( processType )
				{
					case 0:
						{

						}
						break;
					case 1: 
						{ // 길이가 유효한데 헤더에서 이미 잘려있다면 더이상 두려울 것은 없습니다.

						}
						break;
				}
			}
		}
	}
}
