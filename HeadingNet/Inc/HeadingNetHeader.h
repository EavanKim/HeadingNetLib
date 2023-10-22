#pragma once
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <stdint.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2ipdef.h>

#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>

#include <memory>
#include <string>
#include <stdexcept>

#include <thread>
#include "pthread.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>
#include <concurrent_priority_queue.h>

#include <condition_variable>

#include "Base/CHeadingObject.h"

#include "Util/HeadingDefine.h"

#include "CSimpleLock_Win.h"
#include "CSimpleThreadLock.h"

#include "CSessionStorage.h"
#include "CSessionStorage_v2.h"

#include "Base/CSimpleCallback.h"

#include "Array_WSAEvent.h"
#include "Base/HeadingBuffer.h"
#include "HeadingSession.h"

#include "Base/CSimpleState.h"
#include "Base/CEventBaseSession.h"
#include "Base/CSimpleSocket.h"
#include "CLoginSocket.h"
#include "Base/CEventBaseSession_v2.h"
#include "CAccepter.h"
#include "CConnector.h"
#include "CLoginWaiter.h"
#include "CAcceptThread.h"
#include "CAcceptThread_v2.h"
#include "CClientSession.h"
#include "Base/CEventSelecter.h"

#include "Base/CPacketEncoder.h"
#include "Base/CPacketDecoder.h"
#include "Base/CPacketHandler.h"

#include "CWorkerBase.h"
#include "CPacketSendWorker.h"
#include "CPacketRecvWorker.h"

#include "CSessionRouter.h"
#include "CSessionMgr.h"

#include "Util/HeadingFunction.h"