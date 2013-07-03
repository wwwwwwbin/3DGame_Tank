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

void CGSWar::Initialize( void )
{
	m_pControllor = new CSceneMgr();
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
