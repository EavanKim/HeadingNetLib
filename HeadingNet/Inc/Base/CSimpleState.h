#pragma once

namespace Heading
{
	class CSimpleState
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

