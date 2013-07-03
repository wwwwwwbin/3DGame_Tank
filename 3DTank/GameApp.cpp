#include "StdAfx.h"
#include "GameApp.h"

extern HWND g_hWnd;

CGameApp::CGameApp(void)
{
	m_pEngine = NULL;
	m_pGameStateManager = NULL;
}


CGameApp::~CGameApp(void)
{
}

bool CGameApp::Initialize( void )
{
	m_pEngine = CGraphicsEngine::GetInstance();
	m_pGameStateManager = new CGameStateManager();
	if(m_pEngine->Initialize() == false){
		MessageBox(NULL,_T("³õÊ¼»¯Ê§°Ü"),NULL,NULL);
		return false;
	}
	m_pEngine->m_pCamera->Pitch(0.1f);

	m_fGameTime = m_pEngine->m_pTimer->GetGamePlayTime();

	return true;
}

void CGameApp::Run( void )
{
	float fPosTime = m_pEngine->m_pTimer->GetGamePlayTime();
	if(fPosTime > m_fGameTime + FPSFLAG){
		m_fGameTime = fPosTime;
		m_pGameStateManager->Update();
		m_pGameStateManager->Render();

		m_pEngine->Render();
	}
}
