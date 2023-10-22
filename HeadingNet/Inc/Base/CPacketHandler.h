#pragma once

#define PACKET_HANDLER_SIZE 1024

namespace Heading
{
	typedef void (*packet_Callback)(CClientSession*, packetHeader_t*);

	struct PacketCallback
	{
		PacketCallback() = default;

		PacketCallback( std::string _name, size_t _hash, uint64_t _type, int _size )
			: className(_name)
			, classHash(_hash)
			, type(_type)
			, size(_size )
		{

		}

		const std::string className = "";
		const size_t classHash = 0;
		const uint64_t type = 0;
		const int size = 0;

		packet_Callback callback = nullptr;
	};

	class CPacketHandler : public CHeadingObject
	{
	public:
		CPacketHandler(packet_Callback _nullCallback);

		template<class T>
		void AddPacketType(void (*_callback)(CClientSession*, packetHeader_t*) = nullptr )
		{
			static_assert(std::is_base_of<T, packetHeader_t>::value);

			T strLocalInfo;

			new ( m_callbackArray[ m_size ] ) PacketCallback( typeid( T ).name( )
															  , typeid( T ).hash_code( )
															  , m_size + 1
															  , strLocalInfo.size
															  , _callback );

			++m_size;
		}

		void Do_Process(CClientSession* _session, packetHeader_t* _packet);

		bool Create_Header(int _ePacketType , char* _ptr, char* _data, int _size );

	private:
		packet_Callback m_nullCallback = nullptr;
		uint8_t m_size = 0;
		PacketCallback m_callbackArray[ PACKET_HANDLER_SIZE ] = {};
	};
}

