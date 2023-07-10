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

#include "HeadingDefine.h"
#include "HeadingBuffer.h"
#include "HeadingSession.h"

// Class화 작업 내용들
#include "CSession.h"
#include "CSelect.h"
#include "CEventSession.h"
#include "CEventSelect.h"

#include "HeadingFunction.h"