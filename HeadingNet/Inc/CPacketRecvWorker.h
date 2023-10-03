#pragma once

namespace Heading
{
	typedef int( *fnPacketDecode_t )( CPacketDecoder*&, Header* );

	class CPacketRecvWorker : CWorkerBase<fnPacketDecode_t, CPacketDecoder*&, Header*>
	{
	public:
		CPacketRecvWorker(CPacketDecoder* _decoder);
		int run(Header* _param = nullptr);

	private:
		CPacketDecoder* m_decoder = nullptr;
		Header* m_packet = nullptr;
	};
}

