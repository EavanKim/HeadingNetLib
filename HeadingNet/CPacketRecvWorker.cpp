#include "pch.h"

namespace Heading
{
	CPacketRecvWorker::CPacketRecvWorker(CPacketDecoder* _decoder)
		: m_decoder(_decoder)
	{
	}

	int CPacketRecvWorker::run(packetHeader_t* _param)
	{
		// 이미 저장된 데이터가 있었는가에 대한 처우는 외부에서 알아서 처리
		// 해당 내용은 리턴값이 1일때 외부에서 알아서 처리

		// 새로운 패킷 처리가 들어왔다면
		if ( nullptr != _param )
		{
			// 해당 패킷 처리
			m_packet = _param;
		}

		// 무언가의 이유로 pending 되었던 작업이 없거나
		// 새로 들어온 내용도 없이 실행되었다면
		if ( nullptr == m_packet )
			return 0; // 종료

		// do Delegate!
		int result = CWorkerBase::run( m_decoder, m_packet );

		// 결과로 분류되는 리턴처리
		switch ( result )
		{
			case -1: // 취소되거나
			case 0: // 제대로 끝났다면
				m_packet = nullptr; // 비워주기
				break;
		}

		return result;
	}
}
