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
		// 굳이 안에서 이렇게 복잡하게 처리하는 이유는
		// 최종적인 복사가 얼마나 필요할 지 외부에서 알려면
		// 복사를 두 번 해야 알 수 있기 때문에
		// 한번으로 제한하기 위함입니다.
		_data = nullptr;

		bufferIndex_t bufferSize = getSize();
		bufferIndex_t straightBufferLength = m_bufferEnd - m_start;
		if ( sizeof(packetHeader_t) < bufferSize )
		{
			bufferIndex_t packetLength = 0;
			//혹시 수신이 쪼개졌을때를 대비해서 무작정 일렬로 받지않고 조건을 체크합니다.
			if ( sizeof(packetHeader_t) < ( straightBufferLength ) )
			{
				// 어찌되었건 마지막까지의 거리가 멀쩡하므로 packetHeader_t를 꺼내옵니다
				packetLength = ((packetHeader_t*)&m_buffer[m_start])->length;
			}
			else
			{
				// 알아내기 어려운 네트워크 사정으로 헤더 데이터가 쪼개졌습니다!
				// 길이 데이터를 어떤 형태로든 추론해야합니다.
				// packetHeader_t 시작이 length이긴 하지만 설마가 있으니 검사합니다.
				if ( straightBufferLength > sizeof(packetSize_t) )
				{
					packetLength = *(( packetSize_t *)&m_buffer[m_start]);
				}
				else
				{
					//설마설마싶지만 진짜 길이마저 잘려서 들어왔다면 직접 연산합니다.
					// 패킷이 255는 넘길 수 있어도 6만은 넘기지 않을 것 같아서 2byte 정도면 거의 쓸 일이 없을거 같고
					// 쓸데없이 for 루프로 부적절하게 돌 것 같지만
					// 일단 개념탐색용 프로젝트니까 적당히 넘기고 나중에 실사용에 문제 생기면 헤더 사이즈에 제약을 넣고 단순처리로 변경합니다.
					// 사실 여기는 거의 안 걸리지 않을까 싶은게 본심입니다.
					bufferIndex_t seek = 0;
					char buffer[sizeof(packetSize_t)];

					for ( bufferIndex_t currentSeek = m_start; m_bufferEnd > currentSeek; ++currentSeek, ++seek )
					{
						buffer[seek] = m_buffer[currentSeek];
					}

					bufferIndex_t retrySize = sizeof(packetSize_t) - straightBufferLength;

					for ( bufferIndex_t currentSeek = 0; retrySize > currentSeek; ++currentSeek, ++seek )
					{
						buffer[seek] = m_buffer[currentSeek];
					}

					packetLength = *( packetSize_t* ) buffer;
				}
			}

			// 사이즈를 먼저 검사했으므로 packet의 length 데이터가 오염될 일은 없습니다.
			// 마저 읽을 데이터 사이즈가 유효한지 확인합니다.
			if ( bufferSize > packetLength )
			{
				// 여기가 유효한 시점에서 데이터는 확정적으로 복제 될 예정이므로 할당을 시작합니다.
				// 클라이언트에서 new로 생성된 데이터를 그대로 보내준걸 받아든 것이므로
				// 지울땐 delete로 날려도 사실 큰 변화는 없다고 가정하고 작업합니다.
				_data = (packetHeader_t*)malloc(packetLength);

				if ( straightBufferLength > packetLength ) // 아주 적절한 상황입니다. 그냥 복제하고 데이터 시작 위치도 마음놓고 옮깁니다.
				{
					memcpy_s(_data, packetLength, &m_buffer[m_start], packetLength);
					m_start += packetLength;
				}
				else if ( straightBufferLength == packetLength ) // 이 경우 시작을 초기화 해버리는게 빠르니 별도 검사합니다.
				{
					memcpy_s(_data, packetLength, &m_buffer[m_start], packetLength);
					m_start = 0;
				}
				else
				{
					bufferIndex_t leftSize = packetLength - straightBufferLength;
					memcpy_s(_data, packetLength, &m_buffer[m_start], straightBufferLength); // 시작 지점부터 버퍼 끝 까지 복사하고
					memcpy_s(_data, packetLength, &m_buffer[0], leftSize); // 버퍼 최초로 돌아가서 남은 사이즈를 복사한다음
					m_start = leftSize; // 남은 사이즈 양 만큼으로 시작 인덱스를 변경해주면 끝.
				}
			}
		}
	}
}
