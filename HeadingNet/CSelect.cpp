#include "pch.h"

namespace Heading
{
	CSelect::CSelect( uint16_t _port )
		: m_port( _port )
	{

	}

	CSelect::~CSelect()
	{

	}

	void CSelect::SetupAddrInfo()
	{
		sockaddr_storage storage;
		memset( &storage, 0, sizeof storage );
		// The socket address to be passed to bind
		m_info.sin_family = AF_INET;
		m_info.sin_addr.s_addr = htonl( INADDR_ANY );
		m_info.sin_port = htons( m_port );
	}

	bool CSelect::bind()
	{
		int returnValue = 0;
		int loopCounter = 0;

		// 새로 바인딩하면 초기화해버리기
		if( INVALID_SOCKET != m_sock )
		{
			closesocket( m_sock );
			m_sock = INVALID_SOCKET;
			return false;
		}

		do
		{
			if( 5 < loopCounter )
			{
				int winerror = GetLastError();
				// exception 객체 생성되면 throw하면서 에러 정보 송신
				return false;
			}

			m_sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			if( INVALID_SOCKET == m_sock )
			{
				continue;
			}

			returnValue = ::bind( m_sock, ( SOCKADDR* )&m_info, sizeof( m_info ) );
			if( returnValue == SOCKET_ERROR )
			{
				int err = 0;
				if( WSAECONNREFUSED == ( err = WSAGetLastError() ) )
				{
					closesocket( m_sock );
					m_sock = INVALID_SOCKET;
					continue;
				}
				printf( "connect failed with error: %d\n", err );
				return INVALID_SOCKET;
			}
		}
		while( S_OK != returnValue );

		FD_ZERO( &m_socketSet );

		if( SOCKET_ERROR == listen( m_sock, 5 ) )
			return false;

		FD_SET( m_sock, &m_socketSet );

		return true;
	}

	void CSelect::setupSelect( bool _read, bool _write )
	{
		readSelect = _read;
		writeSelect = _write;
	}

	void CSelect::Do_Select()
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		fd_set* readTarget = readSelect ? &m_readSet : nullptr;
		fd_set* writeTarget = writeSelect ? &m_writeSet : nullptr;

		int fdResult = select( m_sock + 1, readTarget, writeTarget, NULL, &tv );

		if( 0 != fdResult )
		{
			if( readSelect )
			{
				Update_Receive();
			}
			
			if( writeSelect )
			{
				Update_Send();
			}
		}
	}

	void CSelect::Update_Receive()
	{
		uint64_t result = 0;
		std::vector<SOCKET> removeList;
		for( uint64_t count = 0; m_readSet.fd_count > count; ++count )
		{
			SOCKET currSock = m_readSet.fd_array[ count ];

			if( FD_ISSET( currSock, &m_readSet ) )
			{
				if( currSock == m_sock )
				{
					Accept_NewSession();
				}
				else
				{
					std::unordered_map<SOCKET, sessionInfo>::iterator session = m_sessionMap.find( currSock );
					if( m_sessionMap.end() != session )
					{
						char* buffer = nullptr;
						uint64_t length = 0;
						if( session->second.recvBuff.get_buffer( &buffer, &length ) )
						{
							int readCount = ::recv( currSock, buffer, length, 0 );
							session->second.recvBuff.commit( readCount );
							result += readCount;
							session->second.recvBuff.get_data( &session->second.sendBuff );
						}
					}
				}
			}
		}

		for( SOCKET del : removeList )
		{
			FD_CLR( del, &m_socketSet );
			closesocket( del );
			m_sessionMap.erase( del );
		}
	}

	void CSelect::Accept_NewSession()
	{
		SOCKET newThing = ::accept( m_sock, NULL, NULL );
		if( INVALID_SOCKET != newThing )
		{
			m_sessionMap.insert( std::make_pair( newThing, sessionInfo() ) );
			FD_SET( newThing, &m_socketSet );
		}
	}

	void CSelect::Update_Send()
	{

		uint64_t result = 0;
		Ping temp_ping;
		std::vector<SOCKET> removeList;
		for( uint64_t count = 0; m_writeSet.fd_count > count; ++count )
		{
			SOCKET currSock = m_writeSet.fd_array[ count ];

			if( FD_ISSET( currSock, &m_writeSet ) )
			{
				std::unordered_map<SOCKET, sessionInfo>::iterator session = m_sessionMap.find( currSock );
				if( m_sessionMap.end() != session )
				{
					if( 0 != session->second.sendBuff.size() )
					{
						for( Header* header : session->second.sendBuff )
						{
							int Result = ::send( currSock, ( char* )header, header->length, 0 );
							if( -1 == Result )
							{
								removeList.push_back( currSock );
								continue;
							}

							result += header->length;
							printf( "send length : %lld  / data : %s", header->length, ( ( ChatBuffer* )header )->buffer );

							delete header;
						}

						session->second.sendBuff.clear();
					}
				}
			}
		}

		for( SOCKET del : removeList )
		{
			FD_CLR( del, &m_writeSet );
			closesocket( del );
			m_sessionMap.erase( del );
		}
	}
}
