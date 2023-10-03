#pragma once

#ifndef DEFAULT_SOCKET_BUFFER_LENGTH
#define DEFAULT_SOCKET_BUFFER_LENGTH (1 << 13)
#endif

#ifndef MAXIMUM_PACKET_DATA_LENGTH
#define MAXIMUM_PACKET_DATA_LENGTH (1 << 12)
#endif

namespace Heading
{
	enum ENUM_SESSION_PRIORITY
	{
		ENUM_SESSION_PRIORITY_REST_IN_PEACE,
		ENUM_SESSION_PRIORITY_NOMAL,
		ENUM_SESSION_PRIORITY_NEED_HARD_WORK,
		ENUM_SESSION_PRIORITY_MAX
	};

	enum ENUM_SESSION_TYPE
	{
		ENUM_SESSION_SYNC,
		ENUM_SESSION_PER_THREAD
	};

	struct CreatedSocketInfo
	{
		uint16_t AcceptPort;
		SOCKET Sock;
	};

	typedef uint16_t session_t;
	typedef uint64_t packetType_t;
	typedef uint32_t packetSize_t;

#pragma pack(push, 1)
	struct Header
	{
		Header( packetType_t _type, packetSize_t _length )
			: type( _type )
			, length( _length )
		{

		}

		const packetType_t type;
		const packetSize_t length;
	};

	template<packetType_t _type, packetSize_t _buffersize>
	struct SendStruct : public Header
	{
		static_assert(_buffersize > 0);

		char buffer[ _buffersize ];

		SendStruct()
			: Header(_type, sizeof( Header ) + _buffersize)
		{
			ZeroMemory( buffer, _buffersize );
		}
	};

	typedef SendStruct<1, 1> PCK_SessionKey;
	typedef SendStruct<2, 1> PCK_Shutdown;
	typedef SendStruct<3, 1> PCK_Ping;
	typedef SendStruct<4, 1> PCK_Pong;
	typedef SendStruct<10000, 12> PCK_CS_Enter;
	typedef SendStruct<10001, 2> PCK_CS_Exit;
	typedef SendStruct<10002, 100> PCK_CS_Chatting;
	typedef SendStruct<10003, 112> PCK_CS_Wispering;
	typedef SendStruct<10004, 112> PCK_SC_Wispering;
	typedef SendStruct<10005, 8> PCK_CS_RequestPrevious;
	typedef SendStruct<10006, 8> PCK_SC_ReturnEnter;
	typedef SendStruct<10007, 120> PCK_SC_OthersChatting;
	typedef SendStruct<10008, 8> PCK_SC_RequestReset;

	struct connInfo_t
	{
		uint32_t	ip;
		uint16_t	port;
		session_t	session;
	};

	// Data Header로서 바이트 데이터 전체를 할당하고 앞쪽을 이 정보로 덮어쓰는 용도
	// 다이나믹한 데이터 처리에 편리
	// 외부에서 전체 배열 사이즈를 넣어주는게 훨씬 편할테니 내부에서 사이즈 보정을 하지 않습니다.
	struct HeadingProtocol_t
	{
		HeadingProtocol_t( session_t _session, packetType_t _type, packetSize_t _size )
			: type( _type )
			, length( _size )
			, session(_session)
		{

		}

		const session_t		session;
		const packetType_t	type;
		const packetSize_t	length;
	};
#pragma pack(pop)

	typedef std::queue<Header*>				packetBuff;
	typedef std::vector<SOCKET>				SocketList;
	typedef std::vector<CreatedSocketInfo>	NewSocketList;
	typedef std::vector<WSAEVENT>			EventList;

	struct SessionData
	{
		WSAEVENT m_sessionKey = INVALID_HANDLE_VALUE;
		Header* m_message = nullptr;
	};
}