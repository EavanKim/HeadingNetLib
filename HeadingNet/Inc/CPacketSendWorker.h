#pragma once

namespace Heading
{
	typedef int( *fnPacket_t )( CPacketEncoder*&, Header* );

	class CPacketSendWorker : CWorkerBase<fnPacket_t, CPacketEncoder*&, Header*>
	{
	public:
		CPacketSendWorker(CPacketEncoder* _encoder);
		int run(Header* _param = nullptr);

	private:
		CPacketEncoder* m_encoder = nullptr;
		Header* m_packet = nullptr;
	};
}

