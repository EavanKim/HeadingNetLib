#include "pch.h"

namespace Heading
{
	Buffer::Buffer()
	{
	}

	Buffer::Buffer( uint64_t _sessionKey )
	{
		memset( m_data, 0, DEFAULT_SOCKET_BUFFER_LENGTH );
	}

	Buffer::Buffer( Buffer& _copy )
	{
		m_sessionKey = _copy.m_sessionKey;
		m_seek = _copy.m_seek;
		memcpy_s( m_data, DEFAULT_SOCKET_BUFFER_LENGTH, _copy.m_data, DEFAULT_SOCKET_BUFFER_LENGTH );
	}

	Buffer::Buffer( Buffer&& _move ) noexcept
	{
		m_sessionKey = _move.m_sessionKey;
		m_seek = _move.m_seek;
		memcpy_s( m_data, DEFAULT_SOCKET_BUFFER_LENGTH, _move.m_data, DEFAULT_SOCKET_BUFFER_LENGTH );
	}

	Buffer::~Buffer()
	{
	}

	void Buffer::commit( int _length )
	{
		printf( "commit length : %i \n", _length );
		m_dataSize += _length;
		m_seek = 0;
	}

	bool Buffer::get_buffer( char** _buffer, int* _length )
	{
		if( DEFAULT_SOCKET_BUFFER_LENGTH > m_dataSize )
		{
			if( 0 != m_dataSize )
			{
				memcpy_s( m_data, m_dataSize, ((char*)m_data) + m_seek, m_dataSize );
				m_seek = 0;
			}

			*_length = DEFAULT_SOCKET_BUFFER_LENGTH - m_dataSize;
			*_buffer = ((char*)m_data) + m_dataSize;
			printf( "return length : %i / [%llX / %llX] \n", *_length, (uint64_t)m_data, (uint64_t)*_buffer );
			return true;
		}

		return false;
	}

	Header* Buffer::get_data()
	{
		Header* result = nullptr;
		if( m_dataSize >= sizeof( Header ) )
		{
			Header* getHeader = ( Header* )((char*)m_data + m_seek);

			if( m_dataSize >= getHeader->length )
			{
				switch( getHeader->type )
				{
					case 1:
						result = new PCK_SessionKey();
						break;
					case 2:
						result = new PCK_Shutdown();
						break;
					case 3:
						result = new PCK_Ping();
						break;
					case 4:
						result = new PCK_Pong();
						break;
					case 10000:
						result = new PCK_CS_Enter();
						break;
					case 10001:
						result = new PCK_CS_Exit();
						break;
					case 10002:
						result = new PCK_CS_Chatting();
						break;
					case 10003:
						result = new PCK_CS_Wispering();
						break;
					case 10004:
						result = new PCK_CS_RequestPrevious();
						break;
					case 10005:
						result = new PCK_SC_ReturnEnter();
						break;
					case 10006:
						result = new PCK_SC_OthersChatting();
						break;
					default:
						break;
				}
			}
		}

		if( nullptr != result )
		{
			memcpy_s( result, result->length, m_data + m_seek, result->length );
			m_seek += result->length;
			m_dataSize -= result->length;
		}

		return result;
	}

	void Buffer::get_data( packetBuff* _datas )
	{
		m_seek = 0;
		Header* parse = get_data();
		while( nullptr != parse )
		{
			printf( "[seek : %lld][data] : %s \n", m_seek, ( ( SendStruct<0, 1>* )parse )->buffer );
			_datas->push( parse );
			parse = get_data();
		}
	}

	void Buffer::get_send_data( char** _buffer, int* _length )
	{
		*_buffer = new char[m_dataSize];
		memcpy_s(*_buffer, m_dataSize, m_data, m_dataSize);
		*_length = m_dataSize;

		// 다 꺼냈으니 비워줍니다.
		m_seek = 0;
		m_dataSize = 0;
	}

	bool Buffer::set_data( char* _data, uint64_t _length )
	{
		if( m_seek != (DEFAULT_SOCKET_BUFFER_LENGTH / 2) )
		{
			uint64_t emptySize = (DEFAULT_SOCKET_BUFFER_LENGTH/2) - m_seek;
			if( emptySize > _length )
			{
				memcpy_s(&m_data[m_seek], emptySize, _data, _length);
				m_seek += _length;
				m_dataSize += _length;

				return true;
			}
		}

		return false;
	}

	bool Buffer::isEmpty( )
	{
		return 0 == m_dataSize;
	}
}