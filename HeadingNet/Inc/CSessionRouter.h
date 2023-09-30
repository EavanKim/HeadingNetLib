#pragma once

//=================================================================================================
// 
// 구성 기본 방향
// 
// 1. Session은 OSI의 5계층
// 2. 3계층과 4계층을 거쳐서 5계층에 의미가 부여
// 3. 그렇다면 ip도 port도 세션을 구분하는데 의미가 있는 앞단 정보
// 4. 그러므로 ip와 port와 조합하여 session 번호를 지정
// 
// Rounter 라고 해봐야 접속 정보를 관리하고 올바른 게이트웨이로 연동하는 정도의 동작이고
// 이걸 2계층 기준이면 스위치, 3계층 기준부터 라우터라고 부를 뿐이고,
// 사람에 따라선 어플리케이션 라우터가 아니면 3계층 스위치로 구분할 정도의 단순작업.
// 단순작업을 정확하고 빠르게 처리하는것이 목적.
// 
//=================================================================================================

#include <stdio.h>
#include <ctime>

struct ConnInfoIP
{
	uint32_t ip; // 3계층
	uint16_t port; // 4계층
	uint16_t session; // 5계층
};

union ConnectionInfo
{
	uint64_t connectionKey; // 모든 계층을 합쳐서 키로 읽기
	ConnInfoIP detailedInfo; // 각 계층을 구분해서 읽기
};

// 클라이언트에 대한 접속 관련 정보는 무엇이 더 필요할까...?
struct ClientConnInfo
{
	ConnectionInfo	info_Conn; // 접속 정보
	time_t			info_lastPing; // 마지막 핑 갱신 시각 정보
	time_t			info_lastSend; // 마지막 전송 시각 정보
	time_t			info_lastRecv; // 마지막 수신 시각 정보

};

// Session은 OSI 5번 계층이므로
// 패킷이 오고 가는동안 자동으로 라우팅 되어야 함.
class CSessionRouter
{
public:
	static void init();
	static CSessionRouter* get();
	void dispose();

private:
	std::unordered_map<uint64_t, ClientConnInfo*> m_value;

	static CSessionRouter* m_instance;
};

