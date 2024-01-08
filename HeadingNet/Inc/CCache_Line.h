#pragma once

#define MAX_CACHE_SIZE DEFAULT_SOCKET_BUFFER_LENGTH
namespace Heading
{
	class CCache_Line
	{
	public:
		CCache_Line();
		~CCache_Line();

		CLock_UserData* GetCacheData(char*& _result, uint64_t& _size);
		CLock_UserData* SetCacheData(char*& _result, uint64_t& _size);

		// 아무것도 안하면 최대값으로 처리합니다.
		void Commit(uint64_t _size = MAX_CACHE_SIZE);

		uint64_t GetMaximum();

	private:
		CLock_UserData m_lock = {};
		uint64_t m_size = MAX_CACHE_SIZE;

		char m_buffer[MAX_CACHE_SIZE] = {};
	};
}

