#pragma 

namespace Heading
{
	class CChatSession : public CEventBaseSession
	{
	public:
		CChatSession();
		virtual ~CChatSession();

	private:

	};

	typedef std::unordered_map<SOCKET, CChatSession*> ChatSessionMap;
	typedef std::unordered_map<WSAEVENT, CChatSession*> ChatSessionEventMap;
}

