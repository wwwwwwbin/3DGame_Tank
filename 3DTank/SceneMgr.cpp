#include "StdAfx.h"
#include "SceneMgr.h"
#include "EngineDefine.h"
#include "MessageMgr.h"
#include "GraphicsEngine.h"


CSceneMgr::CSceneMgr(void)
{
	m_pTank = new CTank(0.f,0.f,0.f);
	CGraphicsEngine::GetInstance()->SetCameraPosition(
		m_pTank->GetPosition().x + 50,
		m_pTank->GetPosition().y + 30,
		m_pTank->GetPosition().z
	);
	for(int i = 0 ; i < 40 ; ++i){
		m_vWalls[i] = new CWall(-395.f,0,-395.f+i*20.f);
		m_vWalls[40 + i] = new CWall(395.f,0,-395.f+i*20.f);
	}
	for(int i = 0 ; i < 40 ; ++i){
		m_vWalls[80 + i] = new CWall(-395.f + i*20.f,0,-395.f);
		m_vWalls[120 + i] = new CWall(-395.f + i*20.f,0,395.f);
	}

	m_pTree[0] = new CTree();
	m_pTree[0]->SetPosition(-350.f, 55.f, -350.f);

	m_pTree[1] = new CTree();
	m_pTree[1]->SetPosition(-350.f, 55.f, 350.f);

	m_pTree[2] = new CTree();
	m_pTree[2]->SetPosition(350.f, 55.f, -350.f);

	m_pTree[3] = new CTree();
	m_pTree[3]->SetPosition(350.f, 55.f, 350.f);

	m_BulletList.clear();

	SetSkyBox();
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

void CSceneMgr::SetSkyBox( void )
{
	float fBoxPosition = 595.f;
	float fBoxHeigh = 700.f;
	float fDelta = 0.5;

	m_pQuad[0] = new CQuad();
	m_pQuad[0]->m_nTextureID = TEXTURE_INDEX_LAWN;
	m_pQuad[0]->m_vVertex[0].fPosX = -fBoxPosition;
	m_pQuad[0]->m_vVertex[0].fPosZ = -fBoxPosition;
	m_pQuad[0]->m_vVertex[0].fPosY = 0.f;
	m_pQuad[0]->m_vVertex[0].fTexX = 0.f;
	m_pQuad[0]->m_vVertex[0].fTexY = 0.f;

	m_pQuad[0]->m_vVertex[1].fPosX = -fBoxPosition;
	m_pQuad[0]->m_vVertex[1].fPosZ = fBoxPosition;
	m_pQuad[0]->m_vVertex[1].fPosY = 0.f;
	m_pQuad[0]->m_vVertex[1].fTexX = 1.f;
	m_pQuad[0]->m_vVertex[1].fTexY = 0.f;

	m_pQuad[0]->m_vVertex[2].fPosX = fBoxPosition;
	m_pQuad[0]->m_vVertex[2].fPosZ = fBoxPosition;
	m_pQuad[0]->m_vVertex[2].fPosY = 0.f;
	m_pQuad[0]->m_vVertex[2].fTexX = 1.f;
	m_pQuad[0]->m_vVertex[2].fTexY = 1.f;

	m_pQuad[0]->m_vVertex[3].fPosX = fBoxPosition;
	m_pQuad[0]->m_vVertex[3].fPosZ = -fBoxPosition;
	m_pQuad[0]->m_vVertex[3].fPosY = 0.f;
	m_pQuad[0]->m_vVertex[3].fTexX = 0.f;
	m_pQuad[0]->m_vVertex[3].fTexY = 1.f;
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[0]);

	m_pQuad[1] = new CQuad();
	m_pQuad[1]->m_nTextureID = TEXTURE_INDEX_BOX_FONT;
	m_pQuad[1]->m_vVertex[0].fPosX = -fBoxPosition;
	m_pQuad[1]->m_vVertex[0].fPosZ = -fBoxPosition - fDelta;
	m_pQuad[1]->m_vVertex[0].fPosY = fBoxHeigh;
	m_pQuad[1]->m_vVertex[0].fTexX = 0.f;
	m_pQuad[1]->m_vVertex[0].fTexY = 0.f;

	m_pQuad[1]->m_vVertex[1].fPosX = -fBoxPosition;
	m_pQuad[1]->m_vVertex[1].fPosZ = fBoxPosition + fDelta;
	m_pQuad[1]->m_vVertex[1].fPosY = fBoxHeigh;
	m_pQuad[1]->m_vVertex[1].fTexX = 1.f;
	m_pQuad[1]->m_vVertex[1].fTexY = 0.f;

	m_pQuad[1]->m_vVertex[2].fPosX = -fBoxPosition;
	m_pQuad[1]->m_vVertex[2].fPosZ = fBoxPosition + fDelta;
	m_pQuad[1]->m_vVertex[2].fPosY = 0.f;
	m_pQuad[1]->m_vVertex[2].fTexX = 1.f;
	m_pQuad[1]->m_vVertex[2].fTexY = 1.f;

	m_pQuad[1]->m_vVertex[3].fPosX = -fBoxPosition;
	m_pQuad[1]->m_vVertex[3].fPosZ = -fBoxPosition - fDelta;
	m_pQuad[1]->m_vVertex[3].fPosY = 0.f;
	m_pQuad[1]->m_vVertex[3].fTexX = 0.f;
	m_pQuad[1]->m_vVertex[3].fTexY = 1.f;
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[1]);

	m_pQuad[2] = new CQuad();
	m_pQuad[2]->m_nTextureID = TEXTURE_INDEX_BOX_LEFT;
	m_pQuad[2]->m_vVertex[0].fPosX = fBoxPosition + fDelta;
	m_pQuad[2]->m_vVertex[0].fPosZ = -fBoxPosition;
	m_pQuad[2]->m_vVertex[0].fPosY = fBoxHeigh;
	m_pQuad[2]->m_vVertex[0].fTexX = 0.f;
	m_pQuad[2]->m_vVertex[0].fTexY = 0.f;

	m_pQuad[2]->m_vVertex[1].fPosX = -fBoxPosition - fDelta;
	m_pQuad[2]->m_vVertex[1].fPosZ = -fBoxPosition;
	m_pQuad[2]->m_vVertex[1].fPosY = fBoxHeigh;
	m_pQuad[2]->m_vVertex[1].fTexX = 1.f;
	m_pQuad[2]->m_vVertex[1].fTexY = 0.f;

	m_pQuad[2]->m_vVertex[2].fPosX = -fBoxPosition - fDelta;
	m_pQuad[2]->m_vVertex[2].fPosZ = -fBoxPosition;
	m_pQuad[2]->m_vVertex[2].fPosY = 0.f;
	m_pQuad[2]->m_vVertex[2].fTexX = 1.f;
	m_pQuad[2]->m_vVertex[2].fTexY = 1.f;

	m_pQuad[2]->m_vVertex[3].fPosX = fBoxPosition + fDelta;
	m_pQuad[2]->m_vVertex[3].fPosZ = -fBoxPosition;
	m_pQuad[2]->m_vVertex[3].fPosY = 0.f;
	m_pQuad[2]->m_vVertex[3].fTexX = 0.f;
	m_pQuad[2]->m_vVertex[3].fTexY = 1.f;
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[2]);

	m_pQuad[3] = new CQuad();
	m_pQuad[3]->m_nTextureID = TEXTURE_INDEX_BOX_BACK;
	m_pQuad[3]->m_vVertex[0].fPosX = fBoxPosition;
	m_pQuad[3]->m_vVertex[0].fPosZ = fBoxPosition + fDelta;
	m_pQuad[3]->m_vVertex[0].fPosY = fBoxHeigh;
	m_pQuad[3]->m_vVertex[0].fTexX = 0.f;
	m_pQuad[3]->m_vVertex[0].fTexY = 0.f;

	m_pQuad[3]->m_vVertex[1].fPosX = fBoxPosition;
	m_pQuad[3]->m_vVertex[1].fPosZ = -fBoxPosition - fDelta;
	m_pQuad[3]->m_vVertex[1].fPosY = fBoxHeigh;
	m_pQuad[3]->m_vVertex[1].fTexX = 1.f;
	m_pQuad[3]->m_vVertex[1].fTexY = 0.f;

	m_pQuad[3]->m_vVertex[2].fPosX = fBoxPosition;
	m_pQuad[3]->m_vVertex[2].fPosZ = -fBoxPosition - fDelta;
	m_pQuad[3]->m_vVertex[2].fPosY = 0.f;
	m_pQuad[3]->m_vVertex[2].fTexX = 1.f;
	m_pQuad[3]->m_vVertex[2].fTexY = 1.f;

	m_pQuad[3]->m_vVertex[3].fPosX = fBoxPosition;
	m_pQuad[3]->m_vVertex[3].fPosZ = fBoxPosition + fDelta;
	m_pQuad[3]->m_vVertex[3].fPosY = 0.f;
	m_pQuad[3]->m_vVertex[3].fTexX = 0.f;
	m_pQuad[3]->m_vVertex[3].fTexY = 1.f;
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[3]);

	m_pQuad[4] = new CQuad();
	m_pQuad[4]->m_nTextureID = TEXTURE_INDEX_BOX_RIGHT;
	m_pQuad[4]->m_vVertex[0].fPosX = -fBoxPosition - fDelta;
	m_pQuad[4]->m_vVertex[0].fPosZ = fBoxPosition;
	m_pQuad[4]->m_vVertex[0].fPosY = fBoxHeigh;
	m_pQuad[4]->m_vVertex[0].fTexX = 0.f;
	m_pQuad[4]->m_vVertex[0].fTexY = 0.f;

	m_pQuad[4]->m_vVertex[1].fPosX = fBoxPosition + fDelta;
	m_pQuad[4]->m_vVertex[1].fPosZ = fBoxPosition;
	m_pQuad[4]->m_vVertex[1].fPosY = fBoxHeigh;
	m_pQuad[4]->m_vVertex[1].fTexX = 1.f;
	m_pQuad[4]->m_vVertex[1].fTexY = 0.f;

	m_pQuad[4]->m_vVertex[2].fPosX = fBoxPosition + fDelta;
	m_pQuad[4]->m_vVertex[2].fPosZ = fBoxPosition;
	m_pQuad[4]->m_vVertex[2].fPosY = 0.f;
	m_pQuad[4]->m_vVertex[2].fTexX = 1.f;
	m_pQuad[4]->m_vVertex[2].fTexY = 1.f;

	m_pQuad[4]->m_vVertex[3].fPosX = -fBoxPosition - fDelta;
	m_pQuad[4]->m_vVertex[3].fPosZ = fBoxPosition;
	m_pQuad[4]->m_vVertex[3].fPosY = 0.f;
	m_pQuad[4]->m_vVertex[3].fTexX = 0.f;
	m_pQuad[4]->m_vVertex[3].fTexY = 1.f;
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[4]);

	/*
	m_pQuad[0] = new CQuad();
	m_pQuad[0]->m_pTexture = CGraphicsEngine::GetInstance()->m_vTextures[TEXTURE_INDEX_BOX_TOP];
	m_pQuad[0]->m_vVertex[0].fPosX = fBoxPosition;
	m_pQuad[0]->m_vVertex[0].fPosZ = -fBoxPosition;
	m_pQuad[0]->m_vVertex[0].fPosY = fBoxHeigh;
	m_pQuad[0]->m_vVertex[0].fTexX = 0.f;
	m_pQuad[0]->m_vVertex[0].fTexY = 0.f;

	m_pQuad[0]->m_vVertex[1].fPosX = fBoxPosition;
	m_pQuad[0]->m_vVertex[1].fPosZ = fBoxPosition;
	m_pQuad[0]->m_vVertex[1].fPosY = fBoxHeigh;
	m_pQuad[0]->m_vVertex[1].fTexX = 1.f;
	m_pQuad[0]->m_vVertex[1].fTexY = 0.f;

	m_pQuad[0]->m_vVertex[2].fPosX = -fBoxPosition;
	m_pQuad[0]->m_vVertex[2].fPosZ = fBoxPosition;
	m_pQuad[0]->m_vVertex[2].fPosY = fBoxHeigh;
	m_pQuad[0]->m_vVertex[2].fTexX = 1.f;
	m_pQuad[0]->m_vVertex[2].fTexY = 1.f;

	m_pQuad[0]->m_vVertex[3].fPosX = -fBoxPosition;
	m_pQuad[0]->m_vVertex[3].fPosZ = -fBoxPosition;
	m_pQuad[0]->m_vVertex[3].fPosY = fBoxHeigh;
	m_pQuad[0]->m_vVertex[3].fTexX = 0.f;
	m_pQuad[0]->m_vVertex[3].fTexY = 1.f;
	CGraphicsEngine::GetInstance()->AddQuad(m_pQuad[0]);
	*/
}
