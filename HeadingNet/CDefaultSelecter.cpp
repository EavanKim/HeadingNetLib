#include "pch.h"

namespace Heading
{
	CDefaultSelecter::CDefaultSelecter()
		: m_router(nullptr)
	{

	}
	
	CDefaultSelecter::~CDefaultSelecter()
	{

	}

	bool CDefaultSelecter::IsReady()
	{
		return false;
	}

	bool CDefaultSelecter::IsLive()
	{
		return false;
	}

	void CDefaultSelecter::Init( ServerSetupInfo& _info )
	{

	}
	
	void CDefaultSelecter::Update()
	{
		int result = select(CPU_SETSIZE, &m_read, NULL, NULL, NULL);

	}
	
	void CDefaultSelecter::Stop()
	{

	}
	
	void CDefaultSelecter::Dispose()
	{

	}
}
