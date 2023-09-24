#pragma once
struct Stateful
{

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
	std::unordered_map<uint64_t, Stateful*> m_value;

	static CSessionRouter* m_instance;
};

