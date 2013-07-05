#include "StdAfx.h"
#include "GameApp.h"

extern HWND g_hWnd;

CGameApp::CGameApp(void)
: m_pEngine(NULL)
, m_pGameStateManager(NULL)
{
}


CGameApp::~CGameApp(void)
{
}

int CGameApp::Initialize( void )
{
	int nResult  = false;
	int nRetCode = false;

	nRetCode = CLogSystem::Init();
	LOG_FAILD_JUMP(nRetCode);

	m_pEngine = CGraphicsEngine::GetInstance();
	LOG_FAILD_JUMP(m_pEngine);
	
	m_pGameStateManager = new CGameStateManager();
	LOG_FAILD_JUMP(m_pGameStateManager);

	nRetCode = m_pEngine->Initialize();
	LOG_FAILD_JUMP(nRetCode);

	m_pEngine->m_pCamera->Pitch(0.1f);

	m_fGameTime = m_pEngine->m_pTimer->GetGamePlayTime();


	nResult = true;
Exit0:
	if (!nResult)
	{
		MessageBox(NULL, _T("³õÊ¼»¯Ê§°Ü"), NULL, NULL);
	}
	return nResult;
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

void CGameApp::Exit( void )
{
	CLogSystem::LogPrint(LOG_TYPE_LOG, "Game Exit!");
	CLogSystem::UnInit();
}
