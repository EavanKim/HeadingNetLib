#pragma once

namespace Heading
{
	typedef int( *fnPacket_t )( CPacketEncoder*&, packetHeader_t* );

	class CPacketSendWorker : CWorkerBase<fnPacket_t, CPacketEncoder*&, packetHeader_t*>
	{
	public:
		CPacketSendWorker(CPacketEncoder* _encoder);
		int run(packetHeader_t* _param = nullptr);

	private:
		CPacketEncoder* m_encoder = nullptr;
		packetHeader_t* m_packet = nullptr;
	};
}

