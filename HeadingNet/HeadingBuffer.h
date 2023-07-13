#pragma once

//====================================================================
// 
// Buffer 는 하나의 Session에 대해 1:1로 존재합니다
// Buffer 는 Header로 관리되는 메세지에 대한 데이터들의 시작 위치입니다
// 여기서 생성된 메세지는 처리가 끝나면 각각의 위치에서 반드시 지워주십시오
// 
//====================================================================

// 프로그램에서 직접적으로 접근할 데이터이니 패킹하지 않습니다.
namespace Heading
{
	//#pragma pack(push, 1)
	class Buffer
	{
	public:
				Buffer		(							);
				Buffer		( uint64_t		_sessionKey );
				Buffer		( Buffer&		_copy		);
				Buffer		( Buffer&&		_move		)  noexcept ;
				~Buffer		(							);

		void	commit		( int			_length		);

		bool	get_buffer	( char**		_buffer
							, int*			_length		);	// 버퍼가 읽을 수 있는 상태일 때 true와 함께 정보를 반환합니다.
		Header* get_data	(							);											// 데이터를 한 개만 읽습니다.
		void	get_data	( packetBuff*	_datas		);						// 모든 데이터를 읽어서 꺼냅니다.

	private:
		uint64_t	m_sessionKey	= 0;		// 어느 세션에 속한 버퍼인지의 기록입니다.
		uint64_t	m_seek			= 0;		// 마지막 탐색 위치입니다.
		int			m_dataSize		= 0;		// 유효 데이터 범위를 저장하는 값입니다.
		bool		m_isReady		= true;		// 읽는데 사용 가능한 상태입니다.
		char		m_data[ DEFAULT_SOCKET_BUFFER_LENGTH ] = {};		// 버퍼는 항시 고정으로 사용합니다.
	};
	//#pragma pack(pop)

}