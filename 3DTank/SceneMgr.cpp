#include "StdAfx.h"
#include "SceneMgr.h"
#include "EngineDefine.h"
#include "MessageMgr.h"
#include "GraphicsEngine.h"

CSceneMgr::CSceneMgr(void)
: m_pTank(NULL)
{
	memset(m_vWalls, 0, sizeof(m_vWalls));
	memset(m_pQuad, 0, sizeof(m_pQuad));
	memset(m_pTree, 0, sizeof(m_pTree));
}


CSceneMgr::~CSceneMgr(void)
{
	delete m_pTank;
	for(int i = 0 ; i < 10 ; ++i){
		for(int j = 0 ; j < 10 ; ++j){
			delete m_vWalls[i*10+j];// = new CWall(-22.5f+i*5.f,0,-22.5f+j*5.f);
		}
	}
}

void CSceneMgr::Update( void )
{
	CBullet*			pBullet		= NULL;
	FireParameter*		pFirePar	= NULL;
	CGraphicsEngine*	pEngine		= NULL;
	
	
	pEngine = CGraphicsEngine::GetInstance();

	if(pEngine->m_pDXInput->ReadKeyboard()){
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_UP)){
			pEngine->ChangeCameraPosition(0,-1,0);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_DOWN)){
			pEngine->ChangeCameraPosition(0,1,0);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_LEFT)){
			pEngine->ChangeCameraPosition(0,0,-1);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_RIGHT)){
			pEngine->ChangeCameraPosition(0,0,1);
		}
	}
	m_pTank->Updata();

	CMessageMgr::Message msgGet;

	while(CMessageMgr::GetInstance()->GetMessage(CMessageMgr::MSG_FIRE, &msgGet))
	{
		pFirePar = (FireParameter*)msgGet.dwParameterA;
		pBullet = new CBullet(pFirePar->vPosition, pFirePar->vDirection, pFirePar->fAngle);
		m_BulletList.push_back(pBullet);
		delete pFirePar;
	}

	BulletList::iterator itDelete;
	for (BulletList::iterator it = m_BulletList.begin(); it != m_BulletList.end();)
	{
		pBullet = *it;

		pBullet->Update();

		if (pBullet->IsDead())
		{
			itDelete = it;
			++it;
			delete pBullet;
			m_BulletList.erase(itDelete);
		}
		else
			++it;
	}

}

void CSceneMgr::Render( void )
{
	m_pTank->Render();
	for(int i = 0 ; i < 160 ; ++i){
		m_vWalls[i]->Render();
	}
	for (int i = 0; i < 4; ++i)
	{
		m_pTree[i]->Render();
	}
	for (BulletList::iterator it = m_BulletList.begin(); it != m_BulletList.end(); ++it)
	{
		(*it)->Render();
	}
}

int CSceneMgr::InitSkyBox( void )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;
	
	float fBoxPosition = 595.f;
	float fBoxHeigh = 700.f;
	float fDelta = 0.5;

	m_pQuad[0] = new CQuad();
	LOG_FAILD_JUMP(m_pQuad[0]);

	nRetCode = m_pQuad[0]->Init(
		"Lawn.jpg", 
		D3DXVECTOR3(-fBoxPosition, 0.f, -fBoxPosition),
		D3DXVECTOR3(-fBoxPosition, 0.f, fBoxPosition),
		D3DXVECTOR3(fBoxPosition,  0.f, fBoxPosition),
		D3DXVECTOR3(fBoxPosition,  0.f, -fBoxPosition)
	);
	LOG_FAILD_JUMP(nRetCode);

	m_pQuad[1] = new CQuad();
	LOG_FAILD_JUMP(m_pQuad[1]);

	nRetCode = m_pQuad[1]->Init(
		"Front.jpg", 
		D3DXVECTOR3(-fBoxPosition, fBoxHeigh, -fBoxPosition - fDelta),
		D3DXVECTOR3(-fBoxPosition, fBoxHeigh, fBoxPosition + fDelta),
		D3DXVECTOR3(-fBoxPosition, 0.f,		  fBoxPosition + fDelta),
		D3DXVECTOR3(-fBoxPosition, 0.f,		  -fBoxPosition - fDelta)
	);
	LOG_FAILD_JUMP(nRetCode);

	m_pQuad[2] = new CQuad();
	LOG_FAILD_JUMP(m_pQuad[2]);

	nRetCode = m_pQuad[2]->Init(
		"Left.jpg", 
		D3DXVECTOR3(fBoxPosition + fDelta,	fBoxHeigh, -fBoxPosition),
		D3DXVECTOR3(-fBoxPosition - fDelta, fBoxHeigh, -fBoxPosition),
		D3DXVECTOR3(-fBoxPosition - fDelta, 0.f,	   -fBoxPosition),
		D3DXVECTOR3(fBoxPosition + fDelta,	0.f,	   -fBoxPosition)
	);
	LOG_FAILD_JUMP(nRetCode);

	m_pQuad[3] = new CQuad();
	LOG_FAILD_JUMP(m_pQuad[3]);

	nRetCode = m_pQuad[3]->Init(
		"Back.jpg", 
		D3DXVECTOR3(fBoxPosition, fBoxHeigh, fBoxPosition + fDelta),
		D3DXVECTOR3(fBoxPosition, fBoxHeigh, -fBoxPosition - fDelta),
		D3DXVECTOR3(fBoxPosition, 0.f,		 -fBoxPosition - fDelta),
		D3DXVECTOR3(fBoxPosition, 0.f,		 fBoxPosition + fDelta)
	);
	LOG_FAILD_JUMP(nRetCode);

	m_pQuad[4] = new CQuad();
	LOG_FAILD_JUMP(m_pQuad[4]);

	nRetCode = m_pQuad[4]->Init(
		"Right.jpg", 
		D3DXVECTOR3(-fBoxPosition - fDelta, fBoxHeigh,	fBoxPosition),
		D3DXVECTOR3(fBoxPosition + fDelta,	fBoxHeigh,	fBoxPosition),
		D3DXVECTOR3(fBoxPosition + fDelta,	0.f,		fBoxPosition),
		D3DXVECTOR3(-fBoxPosition - fDelta, 0.f,		fBoxPosition)
	);
	LOG_FAILD_JUMP(nRetCode);
	
	m_pQuad[5] = new CQuad();
	LOG_FAILD_JUMP(m_pQuad[5]);

	nRetCode = m_pQuad[5]->Init(
		"Top.jpg", 
		D3DXVECTOR3(fBoxPosition,	fBoxHeigh,	-fBoxPosition),
		D3DXVECTOR3(fBoxPosition,	fBoxHeigh,	fBoxPosition),
		D3DXVECTOR3(-fBoxPosition,	fBoxHeigh,	fBoxPosition),
		D3DXVECTOR3(-fBoxPosition,	fBoxHeigh,	-fBoxPosition)
	);
	LOG_FAILD_JUMP(nRetCode);

	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[0]);
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[1]);
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[2]);
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[3]);
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[4]);
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[5]);

	nResult = TRUE;
Exit0:
	if (!nResult)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (m_pQuad[i])
			{
				delete m_pQuad[i];
			}
		}
	}
	return nResult;
}

int CSceneMgr::Init()
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	m_pTank = new CTank(0.f, 0.f, 0.f);
	LOG_FAILD_JUMP(m_pTank);

	CGraphicsEngine::GetInstance()->SetCameraPosition(
		m_pTank->GetPosition().x + 50,
		m_pTank->GetPosition().y + 30,
		m_pTank->GetPosition().z
	);

	for(int i = 0 ; i < 40 ; ++i)
	{
		m_vWalls[i] = new CWall(-395.f, 0, -395.f + i * 20.f);
		LOG_FAILD_JUMP(m_vWalls[i]);

		m_vWalls[40 + i] = new CWall(395.f, 0, -395.f + i * 20.f);
		LOG_FAILD_JUMP(m_vWalls[40 + i]);
	}

	for(int i = 0 ; i < 40 ; ++i)
	{
		m_vWalls[80 + i] = new CWall(-395.f + i*20.f,0,-395.f);
		LOG_FAILD_JUMP(m_vWalls[80 + i]);

		m_vWalls[120 + i] = new CWall(-395.f + i*20.f,0,395.f);
		LOG_FAILD_JUMP(m_vWalls[120 + i]);
	}

	m_pTree[0] = new CTree();
	LOG_FAILD_JUMP(m_pTree[0]);

	m_pTree[0]->SetPosition(-350.f, 55.f, -350.f);

	m_pTree[1] = new CTree();
	LOG_FAILD_JUMP(m_pTree[1]);

	m_pTree[1]->SetPosition(-350.f, 55.f, 350.f);

	m_pTree[2] = new CTree();
	LOG_FAILD_JUMP(m_pTree[2]);

	m_pTree[2]->SetPosition(350.f, 55.f, -350.f);

	m_pTree[3] = new CTree();
	LOG_FAILD_JUMP(m_pTree[3]);

	m_pTree[3]->SetPosition(350.f, 55.f, 350.f);

	m_BulletList.clear();

	nRetCode = InitSkyBox();
	LOG_FAILD_JUMP(nRetCode);

	nResult = TRUE;
Exit0:
	return nResult;
}
