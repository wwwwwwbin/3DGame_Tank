#include "StdAfx.h"
#include "MessageMgr.h"

CMessageMgr::CMessageMgr(void)
{
	m_MsgList.clear();
}

CMessageMgr::~CMessageMgr(void)
{

}

CMessageMgr* CMessageMgr::GetInstance( void )
{
	static CMessageMgr* pInstance = NULL;

	if (pInstance == NULL)
	{
		pInstance = new CMessageMgr();
	}

	return pInstance;
}

bool CMessageMgr::GetMessage( MESSAGE_TYPE eMsg, Message* pOutput )
{
	MsgList::iterator itDelet;

	bool bHasFind = false;
	Message* pMsg = NULL;

	pOutput->eMgs = MSG_INVALID;
	
	for (MsgList::iterator it = m_MsgList.begin(); it != m_MsgList.end();)
	{
		pMsg = *it;

		if (pMsg->eMgs == eMsg)
		{
			pOutput->eMgs = pMsg->eMgs;
			pOutput->dwParameterA = pMsg->dwParameterA;
			pOutput->dwParameterB = pMsg->dwParameterB;

			itDelet = it;
			++it;
			delete pMsg;
			m_MsgList.erase(itDelet);

			bHasFind = true;
			break;
		}
		else 
			++it;
	}

	return bHasFind;
}

void CMessageMgr::AddMessage( MESSAGE_TYPE eMsg, DWORD dwParameterA, DWORD dwParameterB )
{
	Message* pMsg = NULL;

	pMsg = new Message();
	pMsg->eMgs = eMsg;
	pMsg->dwParameterA = dwParameterA;
	pMsg->dwParameterB = dwParameterB;

	m_MsgList.push_back(pMsg);
}
