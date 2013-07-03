#pragma once

#include <list>

class CMessageMgr
{
public:

	enum MESSAGE_TYPE
	{
		MSG_BEGIN = 0,
		MSG_FIRE = MSG_BEGIN,
		MSG_SIZE,
		MSG_INVALID = MSG_SIZE
	};

	struct Message
	{
		MESSAGE_TYPE eMgs;
		DWORD dwParameterA;
		DWORD dwParameterB;
	};

private:

	CMessageMgr(void);
	~CMessageMgr(void);

	typedef std::list<Message*> MsgList;

	MsgList m_MsgList;

public:
	static CMessageMgr* GetInstance(void);

	void AddMessage(MESSAGE_TYPE eMsg, DWORD ParameterA, DWORD ParameterB);
	bool GetMessage(MESSAGE_TYPE eMsg, Message* pOutput);
};
