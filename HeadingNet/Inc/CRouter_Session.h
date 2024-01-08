#pragma once

namespace Heading
{
	class CRouter_Session
	{
	public:


	private:
		std::unordered_map<sessionKey_t, ISession*> m_sessions;
	};
}
  
