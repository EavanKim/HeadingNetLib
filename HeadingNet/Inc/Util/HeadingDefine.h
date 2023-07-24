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
		static_assert(_buffersize < 1);

		char buffer[ _buffersize ];

		SendStruct()
		{
			type = _type;
			length = sizeof( Header ) + _buffersize;
			ZeroMemory( buffer, _buffersize );
		}
	};

	typedef SendStruct<1, 1> SessionKey;
	typedef SendStruct<2, 1> Shutdown;
	typedef SendStruct<3, 1> Ping;
	typedef SendStruct<100, 43> TestBuffer;
	typedef SendStruct<1000, 100> ChatBuffer;
	typedef SendStruct<1001, 8> UpdateBuffer;
	typedef SendStruct<1002, 1080> ChatLogBuffer;
#pragma pack(pop)

	typedef std::vector<Header*>			packetBuff;
	typedef std::vector<SOCKET>				SocketList;
	typedef std::vector<CreatedSocketInfo>	NewSocketList;
	typedef std::vector<WSAEVENT>			EventList;

	struct SessionData
	{
		WSAEVENT m_sessionKey = INVALID_HANDLE_VALUE;
		Header* m_message = nullptr;
	};
}