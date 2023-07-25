#pragma once

namespace Heading
{
	class CAcceptThread : public std::thread
	{
	public:
		CAcceptThread( uint16_t _port, void (*_acceptCallback)(SOCKET _sock) );

		static int functionAccept(void* _ptr);

	private:
		CAccepter* m_accepter;
		void (*m_acceptCallback)(SOCKET _sock);
	};
}

