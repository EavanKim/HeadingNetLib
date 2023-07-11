#pragma once

#define DEFAULT_SOCKET_BUFFER_LENGTH 1 << 13

namespace Heading
{
	enum ENUM_SESSION_PRIORITY
	{
		ENUM_SESSION_PRIORITY_REST_IN_PEACE,
		ENUM_SESSION_PRIORITY_NOMAL,
		ENUM_SESSION_PRIORITY_NEED_HARD_WORK,
		ENUM_SESSION_PRIORITY_MAX
	};

	//이 객체 하나당 1session.
	// 작업자가 알아보기 쉬운 이름입니다.
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
		uint64_t sessionKey = 0;
		uint64_t type = 0;
		uint64_t length = 0;
		time_t m_time = time( NULL );
	};

	template<uint64_t _type, uint64_t _buffersize>
	struct SendStruct : public Header
	{
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
	typedef SendStruct<1000, 1000> ChatBuffer;
#pragma pack(pop)

	typedef std::vector<Header*>			packetBuff;
	typedef std::vector<SOCKET>				SocketList;
	typedef std::vector<CreatedSocketInfo>	NewSocketList;
	typedef std::vector<WSAEVENT>			EventList;
}