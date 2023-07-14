#pragma once

//====================================================================
// 
// Buffer �� �ϳ��� Session�� ���� 1:1�� �����մϴ�
// Buffer �� Header�� �����Ǵ� �޼����� ���� �����͵��� ���� ��ġ�Դϴ�
// ���⼭ ������ �޼����� ó���� ������ ������ ��ġ���� �ݵ�� �����ֽʽÿ�
// 
//====================================================================

// ���α׷����� ���������� ������ �������̴� ��ŷ���� �ʽ��ϴ�.
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
							, int*			_length		);	// ���۰� ���� �� �ִ� ������ �� true�� �Բ� ������ ��ȯ�մϴ�.
		Header* get_data	(							);											// �����͸� �� ���� �н��ϴ�.
		void	get_data	( packetBuff*	_datas		);						// ��� �����͸� �о �����ϴ�.

	private:
		uint64_t	m_sessionKey	= 0;		// ��� ���ǿ� ���� ���������� ����Դϴ�.
		uint64_t	m_seek			= 0;		// ������ Ž�� ��ġ�Դϴ�.
		int			m_dataSize		= 0;		// ��ȿ ������ ������ �����ϴ� ���Դϴ�.
		bool		m_isReady		= true;		// �дµ� ��� ������ �����Դϴ�.
		char		m_data[ DEFAULT_SOCKET_BUFFER_LENGTH ] = {};		// ���۴� �׽� �������� ����մϴ�.
	};
	//#pragma pack(pop)

}