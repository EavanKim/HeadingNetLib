#pragma once

enum APPLICATION_STATE
{
	APPLICATION_STATE_ACCEPT
	, APPLICATION_STATE_RECEIVE
	, APPLICATION_STATE_SEND
};

class EventManager_v2
{
public:
	static void OnAccept();
	static void OnReceive();
	static void OnSend();

private:

};

