#include "StdAfx.h"
#include "GameInformation.h"


CGameInformation::CGameInformation(void)
{
	m_eGameState = GS_WAR;
}


CGameInformation::~CGameInformation(void)
{
}

CGameInformation* CGameInformation::GetInstance( void )
{
	static CGameInformation* m_pInstance =  NULL;
	if(m_pInstance == NULL){
		m_pInstance = new CGameInformation();
	}
	return m_pInstance;
}
