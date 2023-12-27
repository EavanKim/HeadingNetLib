#include "pch.h"

namespace Heading
{
    CSocket_Stream::CSocket_Stream(connectionInfo& _info)
        : ISocket()
    {
        addrinfo info = {};
        ZeroMemory(&info, sizeof(addrinfo));
        info.ai_family = AF_INET;
        info.ai_socktype = SOCK_STREAM;
        info.ai_protocol = IPPROTO_TCP;

        int fnResult = getaddrinfo(_info.ip.c_str(), _info.port.c_str(), &info, &_info.info);
        if (S_OK != fnResult)
        {
            _info.sock = INVALID_SOCKET;
            return;
        }

        _info.sock = socket(_info.info->ai_family, _info.info->ai_socktype, _info.info->ai_protocol);
        m_sock = _info.sock;
    }

    CSocket_Stream::CSocket_Stream(SOCKET _sock)
        : ISocket()
        , m_sock( _sock )
    {

    }

    CSocket_Stream::~CSocket_Stream()
    {
        if (INVALID_SOCKET != m_sock)
        {
            // 에러 상태면 socket 삭제 패스
            // 혹은 일부러 삭제를 피하겠다고 하면 패스
            if (!m_state.haveState(HS_ERROR | HS_CLOSETROUGH))
                closesocket(m_sock);

            m_sock = INVALID_SOCKET;
        }
    }

    void CSocket_Stream::setNoDelay(bool _onoff)
    {
        int on = 0;

        if (_onoff)
        {
            on = 1;
        }

        setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(on));
    }

    void CSocket_Stream::setLINGLE(bool _state, u_short _timeout)
    {
        // 종료 관련 버퍼 처리 - LINGLE
        LINGER  ling = { 0, };
        if (_state)
            ling.l_onoff = 1;
        else
            ling.l_onoff = 0;

        ling.l_linger = _timeout;

        setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (CHAR*)&ling, sizeof(ling));
    }

    const SOCKADDR_IN* CSocket_Stream::GetInfo()
    {


        return nullptr;
    }

    bool CSocket_Stream::Send(IMessage* _send)
    {
        int sendResult = ::send(m_sock, _send->GetData(), _send->GetSize(), 0);

        if (SOCKET_ERROR == sendResult)
        {
            m_state.setState(true, HS_ERROR);
            if (WSAEWOULDBLOCK == WSAGetLastError())
                m_state.setState(true, HS_WOULDBLOCK);

            return false;
        }

        if (0 == sendResult)
        {
            // 발송 실패 == terminate
            m_state.setState(true, HS_SOCKETDOWN);
            return false;
        }

        return true;
    }

    // 내가 고민 한 부분
    // 가장 빠른 수동 동적할당을 통해
    // 버퍼가 부족한 일 없이 계속 받게할 수 있을까?
    // 하지만 결국 스팸성 발신까지 올라가면 한도끝도 없으니 발신량 자체를 제한할 수 밖에 없을 것.
    // 발신량 테스트는 일단 기존 구현에서 먼저 테스트해보기
    // 이 부분은 돌려보면서 테스트하자.
    void CSocket_Stream::Recv(IMessage* _recv)
    {
        char* dataPtr = nullptr;
        int length = 0;
        if (m_recvBuffer.get_buffer(&dataPtr, &length))
        {
            int recvResult = ::recv(m_sock, dataPtr, length, 0);

            if (SOCKET_ERROR == recvResult)
            {
                // Error Flag 설정하기
                m_state.setState(true, HS_ERROR);
                return;
            }

            if (0 == recvResult)
            {
                // Error Flag 설정하기
                // size 0을 연결 확인으로 사용하는 경우도 있다고 하시니 넘겨봅니다.
                return;
            }
        }
    }
}
