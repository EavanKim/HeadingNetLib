#pragma once

//=================================================================================
// 
// 조합을 통해 하나의 동작을 만들고,
// 하나의 조립된 동작을 통해 트랜잭션 시키는 것으로 데이터 정합성 확보
// 
// 트랜잭션 정합성 확보 방식
// Abort -> DB 실패
// Suspend -> DB Lock, 혹은 Memory Lock, 혹은 IO 대기
// 
// std::function의 operator()을 따라가다보면 invoker1을 invoker_functor로 호출하는데
// invoker1은 object와 function pointer를 저장하고 있음.
// 
//=================================================================================

namespace Heading
{
	template<typename kernalType, class... parameterTypes>
	class CWorkerBase
	{
	public:
		void add(kernalType _kernal)
		{
			m_aKernal.push_back(_kernal);
			++m_size;
		}

		void clear()
		{
			m_aKernal.clear();
			m_size = 0;
		}

	protected:
		int run(parameterTypes... _Args)
		{
			// 실행이 끝났다면 처음으로 돌림
			if ( m_size <= m_index )
			{
				m_index = 0;
			}

			// 마지막 시작 위치부터 다시 스타트
			for ( ; m_size > m_index; ++m_index )
			{
				switch ( m_aKernal[m_index](std::forward<parameterTypes>(_Args)...) )
				{
					case -1: // abort
						m_index = 0;
						return -1;
					case 0: // contined
						break;
					case 1: // suspend
						return 1;
				}
			}

			return 0;
		}

		std::vector<kernalType> m_aKernal;
		uint8_t m_size = 0; // 설마 250개를 넘는 함수를 연동할 리 없다고 판정하고 8Byte만 사용
		uint8_t m_index = 0; // 설마 250개를 넘는 함수를 연동할 리 없다고 판정하고 8Byte만 사용

	private:
		CSimpleLock_Win m_lock;
	};
}

