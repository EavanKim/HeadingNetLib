#pragma once
namespace Heading
{
	class CPacketHandler
	{
	public:

		virtual void Do_Process(SessionData* _data) = 0;

	private:

	};
}

