#pragma once

namespace Heading
{
	//#pragma pack(push, 1)
	class Buffer
	{
	public:
				Buffer				(							);
				Buffer				( uint64_t		_sessionKey );
				Buffer				( Buffer&		_copy		);
				Buffer				( Buffer&&		_move		)  noexcept ;
				~Buffer				(							);

		void	commit				( int			_length		);

		bool	get_buffer			( char**		_buffer
									, int*			_length		);
		packetHeader_t* get_data	(							);
		void	get_data			( packetBuff*	_datas		);

		void	get_send_data		( char**		_buffer
									, int*			_length		);
		bool	set_data			( char* _data
									, int _length				);

		bool	isEmpty				();

	private:
		uint64_t	m_sessionKey							= 0;
		uint64_t	m_seek									= 0;
		int			m_dataSize								= 0;
		bool		m_isReady								= true;
		char		m_data[ DEFAULT_SOCKET_BUFFER_LENGTH ]	= {};
	};
	//#pragma pack(pop)

}