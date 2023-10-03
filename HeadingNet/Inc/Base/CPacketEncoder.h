#pragma once

namespace Heading
{
	class CPacketEncoder
	{
	public:
		CPacketEncoder(CPacketStorage* _storage);
		~CPacketEncoder();



	private:
		CPacketStorage* m_storage = nullptr;
	};
}

