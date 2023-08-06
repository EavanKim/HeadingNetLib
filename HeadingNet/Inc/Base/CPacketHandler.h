#pragma once

#define PACKET_HANDLER_SIZE 1024

namespace Heading
{
	struct PacketCallback
	{
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
		
		void (*callback)(CClientSession* _session, Header* _packet) = nullptr;
	};

	// 핸들러 별로 타입이 변경되므로
	// 반드시 서버 - 클라가 동일한 핸들러 초기화를 써야합니다.
	class CPacketHandler
	{
	public:
		// 여기서 널이 들어오면 콜백이 설정되지 않은 대상에서 크래시가 나고
		// 널 콜백을 만들어두면 빈 응답이 들어왔을 때 로그를 남기거나 할 수 있습니다.
		CPacketHandler(void (*_nullCallback)(IN Heading::CClientSession* _session, IN Heading::Header* _packet));

		template<class T>
		void AddPacketType(void (*_callback)(CClientSession* _session, Header* _packet) = nullptr )
		{
			static_assert(std::is_base_of<T, Header>::value);

			// type 정보가 새로 생성되는 부분을 제외하면 size는 얻을 수 있으므로
			// 로컬 변수 하나로 얻을 수 있는 데이터는 다 획득시도합니다.
			T strLocalInfo;

			new ( m_callbackArray[ m_size ] ) PacketCallback( typeid( T ).name( )
															  , typeid( T ).hash_code( )
															  , m_size + 1
															  , strLocalInfo.size
															  , _callback );

			++m_size;
		}

		void Do_Process(CClientSession* _session, Header* _packet);

		// 여기 들어오면 나머지 조건은 무시하고 일단 밀어넣습니다.
		bool Create_Header(int _ePacketType , char* _ptr, char* _data, int _size );

	private:
		void (*m_nullCallback)(CClientSession* _session, Header* _packet) = nullptr;
		uint8_t m_size = 0;
		PacketCallback m_callbackArray[ PACKET_HANDLER_SIZE ];
	};
}

