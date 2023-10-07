#pragma once

namespace Heading
{
	typedef int( *fnPacketDecode_t )( CPacketDecoder*&, packetHeader_t* );

	class CPacketRecvWorker : CWorkerBase<fnPacketDecode_t, CPacketDecoder*&, packetHeader_t*>
	{
	public:
		CPacketRecvWorker(CPacketDecoder* _decoder);
		int run(packetHeader_t* _param = nullptr);

	private:
		CPacketDecoder* m_decoder = nullptr;
		packetHeader_t* m_packet = nullptr;
	};
}

