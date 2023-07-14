#pragma once
//==================================================================================================================================================================
// 
// Event�� Multiple ���������, microthread ó���ϵ��� �� ���� �ϳ��� ó���˴ϴ�.
// �̺�Ʈ�� ���� �����Ǿ lost �Ǵ� ��쿡 ����� ����å�� �غ��ϼ���
// Ex) 10���� ������ ���� ��
// 0~5���� ������ ó���ϰ�
// 6~9���� �ε��� ������ �ƹ��͵� ó������ �ʴ� ������ �߻����� �ʵ��� ó�� �� �ʿ䰡 �ֽ��ϴ�.
// �����ϰ� �������� GameThread ������ ����ũ�ν����尡 �ƴ϶�, ���� �̺�Ʈ�� ������ ó���� �°�����
// �Ӹ��� �Ẹ�ÿ�...
// ���� �׷��ھ� �ϰ� �ظ̴� ���� �����̾��ٴ� ����...
// 
//==================================================================================================================================================================

namespace Heading
{
	class CEventSelect
	{
	public:
		CEventSelect( uint16_t _port );
		~CEventSelect();

		void SetupAddrInfo();
		bool bind();
		void Do_Select();

	private:
		void Update_Receive();
		void Accept_NewSession( long _NetworkEventType );
		void Update_Send();

		uint16_t				m_port				= 0;
		SOCKET					m_sock				= INVALID_SOCKET;
		SOCKADDR_IN				m_info				= {};
		DWORD					m_eventCount		= {};
		DWORD					m_bytesTransferred	= {};
		WSAEVENT				m_connectEvent		= {};
		WSAOVERLAPPED			m_acceptOverlapped	= {};
		std::vector<WSAEVENT>	m_selectEvent		= {};
		ActiveEventSessionMap	m_sessionMap		= {};
		FreeEventSessionList	m_freeList			= {};
	};
}

