#include "StdAfx.h"
#include "GSWar.h"


CGSWar::CGSWar(void)
{
	m_pControllor = NULL;
	m_bHasInit = false;
}


CGSWar::~CGSWar(void)
{
	if(m_pControllor != NULL){
		delete m_pControllor;
	}
}

int CGSWar::Initialize( void )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	m_pControllor = new CSceneMgr();
	LOG_FAILD_JUMP(m_pControllor);

	nRetCode = m_pControllor->Init();
	LOG_FAILD_JUMP(nRetCode);

	nResult = TRUE;
Exit0:
	return nResult;
}

void CGSWar::Update( void )
{
	if(m_bHasInit == false){
		Initialize();
		m_bHasInit = true;
	}
	m_pControllor->Update();
}

void CGSWar::Render( void )
{
	m_pControllor->Render();
}

void CGSWar::Enter( void )
{
	//...
}

void CGSWar::Leave( void )
{
	delete m_pControllor;
	m_pControllor = NULL;
	m_bHasInit = false;
}
