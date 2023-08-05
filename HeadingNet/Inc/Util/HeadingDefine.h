#pragma once

#define DEFAULT_SOCKET_BUFFER_LENGTH (1 << 13)

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

#pragma pack(push, 1)
	struct Header
	{
		uint64_t type = 0;
		int length = 0;
	};

	template<uint64_t _type, uint64_t _buffersize>
	struct SendStruct : public Header
	{
		static_assert(_buffersize > 0);

		char buffer[ _buffersize ];

		SendStruct()
		{
			type = _type;
			length = sizeof( Header ) + _buffersize;
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