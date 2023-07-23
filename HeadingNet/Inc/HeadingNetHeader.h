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
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>
#include <concurrent_priority_queue.h>

#include <condition_variable>

#include "Util/HeadingDefine.h"
#include "Base/HeadingBuffer.h"
#include "HeadingSession.h"

#include "Base/CEventBaseSession.h"
#include "CAccepter.h"
#include "CClientSession.h"
#include "CAcceptManager.h"
#include "Base/CEventSelecter.h"
#include "../Test/CBufferGroup.h"
#include "Base/CSelecter.h"

#include "Base/CPacketHandler.h"

#include "Util/HeadingFunction.h"