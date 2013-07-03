#include "StdAfx.h"
#include "GameStateManager.h"
#include "GSWar.h"

CGameStateManager::CGameStateManager(void)
{
	m_bHasInit = false;
}


CGameStateManager::~CGameStateManager(void)
{
}

void CGameStateManager::Initialize( void )
{
	m_ePreGameState = CGameInformation::GetInstance()->m_eGameState;
	m_ePosGameState = m_ePreGameState;
	
	//m_vGameState[CGameInformation::GS_BEGIN] = new ;
	m_vGameState[CGameInformation::GS_WAR] = new CGSWar();
	//m_vGameState[CGameInformation::GS_PASS] = new ;
	
}

void CGameStateManager::Update( void )
{
	if(m_bHasInit == false){
		Initialize();
		m_bHasInit = true;
	}
	m_ePosGameState = CGameInformation::GetInstance()->m_eGameState;
	if(m_ePreGameState != m_ePosGameState){
		m_vGameState[m_ePreGameState]->Leave();
		m_vGameState[m_ePosGameState]->Enter();
		m_ePreGameState = m_ePosGameState;
	}
	m_vGameState[m_ePosGameState]->Update();
}

void CGameStateManager::Render( void )
{
	m_vGameState[m_ePosGameState]->Render();
}
