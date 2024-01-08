#pragma once

namespace Heading
{
	class CLock_UserData
	{
	public:
		void lock_read();
		void lock_write();

		bool tryLock_read();
		bool tryLock_write();

		void release_read();
		void release_write();
		
	private:
		// simple cache read lock
		volatile LONG64 m_rLock = 0;
		// simple cache write lock
		volatile LONG64 m_wLock = 0;
	};

	// 읽기중이면 바로 진입
	// 쓰기중이면 쓰기가 끝날 때 까지 대기
	class CLock_Read
	{
	public:
		CLock_Read(CLock_UserData* _data)
			: m_data(_data)
		{
			m_data->lock_read();
		}

		~CLock_Read()
		{
			m_data->release_read();
		}

	private:
		CLock_UserData* m_data;
	};

	// 읽기중이면 바로 진입
	// 쓰기가 발생하고 있다면 굳이 대기하지 않고 다음 기회에 쓰기
	class CLock_TryRead
	{
	public:
		CLock_TryRead(CLock_UserData* _data)
			: m_data(_data)
			, m_result(m_data->tryLock_read())
		{
			
		}

		~CLock_TryRead()
		{
			m_data->release_read();
		}

	private:
		bool m_result = false;

		CLock_UserData* m_data;
	};

	// 무조건 베타동작
	class CLock_Write
	{
	public:
		CLock_Write(CLock_UserData* _data)
			: m_data(_data)
		{
			m_data->lock_write();
		}

		~CLock_Write()
		{
			m_data->release_write();
		}

	private:
		CLock_UserData* m_data;
	};

	// 무조건 베타동작 시도
	// 갱신을 기록하지 않고 끝날 일은 거의 없을테니 거의 안 쓸 동작
	class CLock_TryWrite
	{
	public:
		CLock_TryWrite(CLock_UserData* _data)
			: m_data(_data)
			, m_result(m_data->tryLock_write())
		{

		}

		~CLock_TryWrite()
		{
			m_data->release_write();
		}

	private:
		bool m_result = false;

		CLock_UserData* m_data;
	};
}

