#include "pch.h"

namespace Heading
{
    CConnSession::CConnSession()
    {
    }

    CConnSession::~CConnSession()
    {
    }

    void CConnSession::Release()
    {
    }

    void CConnSession::Reset()
    {
    }

    const SOCKADDR_IN* CConnSession::GetInfo()
    {
        return nullptr;
    }

    void CConnSession::Set(ISocket* _sock)
    {
    }

    bool CConnSession::Set(IMessage* _message)
    {
        return false;
    }

    void CConnSession::Get(IMessage*& _message)
    {
    }

    void CConnSession::Update()
    {
    }
}
