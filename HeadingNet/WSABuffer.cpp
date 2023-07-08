#include "pch.h"

namespace Heading
{
	WSABuffer::WSABuffer( )
	{
	}

	WSABuffer::WSABuffer( uint64_t _sessionKey )
		:m_sessionKey( _sessionKey )
	{
		memset( m_data, 0, DEFAULT_SOCKET_BUFFER_LENGTH );
	}

	WSABuffer::WSABuffer( WSABuffer& _copy )
	{
		m_sessionKey = _copy.m_sessionKey;
		m_seek = _copy.m_seek;
		memcpy_s( m_data, DEFAULT_SOCKET_BUFFER_LENGTH, _copy.m_data, DEFAULT_SOCKET_BUFFER_LENGTH );
	}

	WSABuffer::WSABuffer( WSABuffer&& _move )
	{
		m_sessionKey = _move.m_sessionKey;
		m_seek = _move.m_seek;
		memcpy_s( m_data, DEFAULT_SOCKET_BUFFER_LENGTH, _move.m_data, DEFAULT_SOCKET_BUFFER_LENGTH );
	}

	WSABuffer::~WSABuffer( )
	{

	}

	void WSABuffer::commit( uint64_t _length )
	{
		printf( "commit length : %lld \n", _length );
		m_dataSize += _length;
		m_seek = 0;
	}

	bool WSABuffer::get_buffer( WSABUF** _buffer )
	{
		if( DEFAULT_SOCKET_BUFFER_LENGTH > m_dataSize )
		{
			if( 0 != m_dataSize )
			{
				memcpy_s( m_data, m_dataSize, m_data + m_seek, m_dataSize );
				m_seek = 0;
			}

			( *_buffer )->len = DEFAULT_SOCKET_BUFFER_LENGTH - m_dataSize;
			( *_buffer )->buf = m_data + m_dataSize;
			printf( "return length : %lld / [%llX / %llX] \n", ( *_buffer )->len, ( *_buffer )->buf, *_buffer );
			return true;
		}

		return false;
	}

	WSAHeader* WSABuffer::get_data( )
	{
		WSAHeader* result = nullptr;
		if( m_dataSize >= sizeof( WSAHeader ) )
		{
			WSAHeader* getHeader = ( WSAHeader* ) m_data + m_seek;

			if( m_dataSize >= getHeader->length )
			{
				switch( getHeader->type )
				{
				case 1:
					result = new WSASessionKey( );
					break;
				case 2:
					result = new WSAShutdown( );
					break;
				case 3:
					result = new WSAPing( );
					break;
				case 100:
					result = new WSATestBuffer( );
					break;
				case 1000:
					result = new WSAChatBuffer( );
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

	void WSABuffer::get_data( WSApacketBuff* _datas )
	{
		m_seek = 0;
		WSAHeader* parse = get_data( );
		while( nullptr != parse )
		{
			printf( "[seek : %lld][data] : %s \n", m_seek, ( ( SendStruct<0, 0>* )parse )->buffer );
			_datas->push_back( parse );
			parse = get_data( );
		}
	}
}
