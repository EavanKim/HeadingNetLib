#pragma once

//====================================================================================================================================================================================
// 
// Default Select
// 
// 가장 기본적인 Select를 사용한 통신 객체입니다.
// 
//====================================================================================================================================================================================

namespace Heading
{
	// 이 객체는 CHeadingObject에 대한 자료구조를 상속하고
	// IHeadingSelecter 함수를 vPtr에 포함하여 지원하는 객체입니다.
	class CDefaultSelecter :  public CHeadingObject, public IHeadingSelecter
	{
	public:
		CDefaultSelecter();
		virtual ~CDefaultSelecter();

		// Inherited via IHeadingSelecter
		virtual bool IsReady	( )							override;
		virtual bool IsLive		( )							override;
		virtual void Init		( ServerSetupInfo& _info )	override;
		virtual void Update		( )							override;
		virtual void Stop		( )							override;
		virtual void Dispose	( )							override;

	private:
		std::vector<WSAEVENT> m_WSALists = {};
		CRouter_Session* m_router = nullptr;
		fd_set m_read;
		fd_set m_send;
	};
}

