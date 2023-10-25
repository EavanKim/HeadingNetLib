#pragma once

namespace Heading
{
	// 이건 다른 객체에 덤으로 올라가는 대상이지만,
	// 여기저기서 조회시도를 할 수 있는 부분 때문에 개별잠금이 들려있는 상황이므로
	// 같은 스레드 검사는 상위 객체의 몫
	class CSimpleState : public CHeadingObject
	{
	public:
		CSimpleState();
		~CSimpleState();

		void setState( bool _onoff, int _flag );
		bool checkState( int _flag );
		bool haveState( int _flag );
		void initialize( int _flag );

	private:
		int m_state;
	};
}

