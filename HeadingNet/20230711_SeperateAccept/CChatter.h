#pragma once

namespace Heading
{
	class CChatter : public CSelecter
	{
	public:
		CChatter( );
		virtual ~CChatter( );

		virtual void Set_NewSession( NewSocketList& _newSocket ) override;

	private:
	};
}

