#pragma once

namespace Heading
{
	class CPacketEncoder
	{
	public:
		CPacketEncoder(CSessionStorage* _storage);
		~CPacketEncoder();



	private:
		CSessionStorage* m_storage = nullptr;
	};
}

