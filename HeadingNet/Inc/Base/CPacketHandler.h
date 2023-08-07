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

	// �ڵ鷯 ���� Ÿ���� ����ǹǷ�
	// �ݵ�� ���� - Ŭ�� ������ �ڵ鷯 �ʱ�ȭ�� ����մϴ�.
	class CPacketHandler
	{
	public:
		// ���⼭ ���� ������ �ݹ��� �������� ���� ��󿡼� ũ���ð� ����
		// �� �ݹ��� �����θ� �� ������ ������ �� �α׸� ����ų� �� �� �ֽ��ϴ�.
		CPacketHandler(void (*_nullCallback)(IN Heading::CClientSession* _session, IN Heading::Header* _packet));

		template<class T>
		void AddPacketType(void (*_callback)(CClientSession* _session, Header* _packet) = nullptr )
		{
			static_assert(std::is_base_of<T, Header>::value);

			// type ������ ���� �����Ǵ� �κ��� �����ϸ� size�� ���� �� �����Ƿ�
			// ���� ���� �ϳ��� ���� �� �ִ� �����ʹ� �� ȹ��õ��մϴ�.
			T strLocalInfo;

			new ( m_callbackArray[ m_size ] ) PacketCallback( typeid( T ).name( )
															  , typeid( T ).hash_code( )
															  , m_size + 1
															  , strLocalInfo.size
															  , _callback );

			++m_size;
		}

		void Do_Process(CClientSession* _session, Header* _packet);

		// ���� ������ ������ ������ �����ϰ� �ϴ� �о�ֽ��ϴ�.
		bool Create_Header(int _ePacketType , char* _ptr, char* _data, int _size );

	private:
		void (*m_nullCallback)(CClientSession* _session, Header* _packet) = nullptr;
		uint8_t m_size = 0;
		PacketCallback m_callbackArray[ PACKET_HANDLER_SIZE ];
	};
}

