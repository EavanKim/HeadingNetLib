#pragma once

typedef void(* fnCallback)(void*, void*);
namespace Heading
{
	// 무조건 Weak 상태의 데이터만 들고있다고 가정하는 함수 호출용 인터페이스 객체입니다.
	class CSimpleCallback
	{
	public:
		CSimpleCallback( );
		~CSimpleCallback( );

		void operator()( fnCallback _fn = nullptr, void* _reserveParam = nullptr );
		void operator()( void* _callerParam );

	private:
		fnCallback m_fn = nullptr;
		void* m_param;
	};
}

