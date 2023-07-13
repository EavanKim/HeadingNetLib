#pragma once

namespace Heading
{
	class CBroadCaster : public CSelecter
	{
	public:
		CBroadCaster( );
		virtual ~CBroadCaster( );

		virtual void Set_NewSession( NewSocketList& _newSocket ) override;

	private:
	};
}

