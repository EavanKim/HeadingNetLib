#pragma once
#include <stdint.h>

//===============================================================================
// 
// Referenced By Memory Management Algorithms and Implementation in C/C++ - Bill Blunden
// 라이센스 관련 언급은 책에서 아직 읽지 못해서 작성은 했는데 라이센스 문제 발생하면 삭제하고 다른 방식으로 다시 작성 예정입니다.
// 
// 세션 당 메모리 한계를 정하고,
// OS를 통한 할당 / 할당해제가 연속되지 않게
// 다이나믹한 변칙적인 사이즈의 메모리 할당을 감당할 수 있도록
// 자체 Allocator 사용
// 
// 초반 오동작은 어차피 자체 프로젝트니까 크래시 하면서 검수하기
// 
// 링버퍼 구조는 속도가 확실히 빠르고 안정적일 수 있지만,
// 데이터 재활용이 어렵고 받아둔 데이터를 링버퍼를 위해 굳이 복사하는 문제도 발생하는 경우가 있으며
// 굳이 초기화 할 필요도 없는 데이터를 위한 낭비가 심한 것 같아서
// 전송 도중의 데이터 보존에 대해 고민해보다가 만들어보기.
// 
// 인덱스로 별도 관리하다가 재활용이 필요하면 리셋되지 않은 이상 Encoder나 Decoder에서 재접근 가능하게 하고
// 삭제 기준은 Encoder나 Decoder가 삭제될 때, 내부에서 사용이 끝났을 때로 해보기
// 
//===============================================================================

#ifdef _WIN64
typedef uint64_t intPtr;
#elif _WIN32
typedef uint32_t intPtr;
#endif

typedef uint32_t index_t;
typedef uint32_t storageSize_t;

#define PACKET_BLOCK_SIZE 16
// MAXIMUM_PACKET_DATA_LENGTH << 1
#define MAXIMUM_PACKET_STORAGE_SIZE 1 << 13

namespace Heading
{
	class CPacketStorage
	{
#define PrevIndex(seek)	( * ( index_t*			)	( &m_dataStorage[	seek - 21	]	) ) // 8byte
#define NextIndex(seek)	( * ( index_t*			)	( &m_dataStorage[	seek - 13	]	) ) // 8byte
#define MemState(seek)	( * ( bool*				)	( &m_dataStorage[	seek - 5	]	) ) // 1byte
#define MemSize(seek)	( * ( storageSize_t*	)	( &m_dataStorage[	seek - 4	]	) ) // 4byte
#define GetIndex(seek)	( * ( index_t*			)	( &m_dataStorage[	0			] - seek	) ) // 4byte

#define MemStart 21
#define SZ_MEMHEADER 21

	public:
		CPacketStorage();
		~CPacketStorage();

		template<typename T>
		T* allocate(storageSize_t _size)
		{
			index_t current = MemStart;

			if ( 0 == _size )
			{
				return NULL;
			}
			
			do
			{
				if ( ( MemSize(current) >= _size ) && ( true == MemState(current) ) )
				{
					split(current, _size);
					return( ( T* ) current );
				}
				current = NextIndex(current);
			} while ( NULL != current );

			return NULL;
		}

		template<typename T>
		void free(T* _release)
		{
			if ( nullptr = _release )
			{
				return;
			}

			// 일단 메모리 주소 검사로 변환 후 이상값 방지
			if ( ( &m_dataStorage[MAXIMUM_PACKET_STORAGE_SIZE] <= _release ) || ( &m_dataStorage[MemStart] > _release ) )
				return;

			index_t releaseIndex = GetIndex(_release);

			if ( ( false != MemState(releaseIndex) ) // Double Free
				|| releaseIndex <= PrevIndex(releaseIndex)
				|| MAXIMUM_PACKET_STORAGE_SIZE <= NextIndex(releaseIndex)
				|| MAXIMUM_PACKET_STORAGE_SIZE <= MemSize(releaseIndex)
				|| 0 != MemSize(releaseIndex) )
			{
				return;
			}

			merge(PrevIndex(_release), _release, NextIndex(_release));
		}

	private:
		void split(index_t _addr, storageSize_t _size);
		void merge(index_t _prev, index_t _current, index_t _next);

		char* m_dataStorage = nullptr;
	};
}

