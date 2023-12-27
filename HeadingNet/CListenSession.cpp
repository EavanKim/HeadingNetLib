#include "pch.h"

namespace Heading
{
    CListenSession::CListenSession()
    {

    }

    CListenSession::~CListenSession()
    {
        Release();
    }

    void CListenSession::Release()
    {
        if (nullptr != m_listenSock)
        {
            delete m_listenSock;
            m_listenSock = nullptr;
        }
    }

    void CListenSession::Reset()
    {
    }

    const SOCKADDR_IN* CListenSession::GetInfo()
    {

        return nullptr;
    }

    void CListenSession::Set(ISocket* _sock)
    {
        Release();

        m_listenSock = _sock;
    }

    bool CListenSession::Set(IMessage* _message)
    {


        return false;
    }

    void CListenSession::Get(IMessage*& _message)
    {

    }

    void CListenSession::Update()
    {

    }
}
