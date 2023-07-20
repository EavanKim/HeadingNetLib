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

#include "HeadingDefine.h"
#include "HeadingBuffer.h"
#include "HeadingSession.h"

#include "CChatBuffer.h"
#include "CEventBaseSession.h"
#include "CAccepter.h"
#include "CClientSession.h"
#include "CChatSession.h"
#include "CAcceptManager.h"
#include "CEventSelecter.h"
#include "CBufferGroup.h"
#include "CSelecter.h"
#include "CChatter.h"

#include "CSubProcess.h"
#include "CChatProcess.h"

#include "Manager.h"

#include "HeadingFunction.h"