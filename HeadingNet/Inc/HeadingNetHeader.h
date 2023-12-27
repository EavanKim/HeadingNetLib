#pragma once
#define WIN32_LEAN_AND_MEAN

// ===================[standard And OS Header]===================

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

// ===================[Base And Interface]===================

#include "Util/HeadingDefine.h"

#include "Base/HeadingBuffer.h"
#include "HeadingSession.h"

#include "Data/HeadingServerInfo.h"

#include "Base/CHeadingObject.h"
#include "Interface/IMessage.h"
#include "Interface/ISocket.h"
#include "Interface/ISocketFactory.h"
#include "Interface/ISession.h"
#include "Interface/IHeadingSelecter.h"
#include "Base/SConnInfo.h"
#include "Interface/ISessionFectory.h"

#include "CSession_Client.h"


#include "Util/HeadingDefine.h"

#include "Base/HeadingBuffer.h"

#include "Base/CSimpleState.h"

#include "CConnector.h"

// ===================[Message]===================



// ===================[Socket]===================

#include "Base/CSimpleSocket.h"
#include "Base/CSocket_Stream.h"

// ===================[Session]===================

#include "Base/CEventBaseSession_v2.h"
#include "CSessionFactory_Conn.h"
#include "Base/CSession_Base.h"

// ===================[Session Rounter]===================

#include "CRouter_Session.h"

// ===================[Selecter]===================

#include "CDefaultSelecter.h"

// ===================[Utils For CPP File]===================

#include "Util/HeadingFunction.h"