#include "StdAfx.h"
#include "Tank.h"
#include "EngineDefine.h"
#include "GraphicsEngine.h"
#include "3DModelFactory.h"
#include "MessageMgr.h"
#include "SceneMgr.h"


CTank::CTank(float fX,float fY,float fZ)
{
	m_fMoveSpeed = 3.f;
	m_pTopModel = new C3DModel();
	m_pBottomModel = new C3DModel();
	C3DModelFactory::GetInstance()->GetModel(m_pTopModel,C3DModelFactory::GM_TANKTOP);
	C3DModelFactory::GetInstance()->GetModel(m_pBottomModel,C3DModelFactory::GM_TANKBOTTOM);
	m_vPosition.x = fX;
	m_vPosition.y = fY;
	m_vPosition.z = fZ;
	m_vDirectionMove.x = -1.f;
	m_vDirectionMove.y = 0.f;
	m_vDirectionMove.z = 0.f;
	m_vDirectionLook = m_vDirectionMove;
	m_fAngleMove = 0.f;
	m_fAngleLook = 0.f;
	//m_bHasInit = false;
	SetPosition(fX,fY,fZ);
}


CTank::~CTank(void)
{
	delete m_pTopModel;
	delete m_pBottomModel;
}

void CTank::Updata( void )
{
	CGraphicsEngine* pEngine = CGraphicsEngine::GetInstance();

	if(pEngine->m_pDXInput->ReadKeyboard()){
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_A)){
			ChangeDirectionMove(-0.02f);
			pEngine->CameraPointRotate(m_vPosition,-0.02f);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_D)){
			ChangeDirectionMove(0.02f);
			pEngine->CameraPointRotate(GetPosition(),0.02f);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_W)){
			D3DXVECTOR3 vDelta = m_vDirectionMove*m_fMoveSpeed;
			ChangePosition(vDelta.x,vDelta.y,vDelta.z);
			pEngine->ChangeCameraPosition(vDelta.x,vDelta.y,vDelta.z);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_S)){
			D3DXVECTOR3 vDelta = -m_vDirectionMove*m_fMoveSpeed;
			ChangePosition(vDelta.x,vDelta.y,vDelta.z);
			pEngine->ChangeCameraPosition(vDelta.x,vDelta.y,vDelta.z);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_U)){
			ChangeDirectionLook(-0.02f);
			pEngine->CameraPointRotate(m_vPosition,-0.02f);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_O)){
			ChangeDirectionLook(0.02f);
			pEngine->CameraPointRotate(m_vPosition,0.02f);
		}
		if(pEngine->m_pDXInput->IsKeyPressed(DIK_SPACE)){
			static float fLastFireTime = pEngine->m_pTimer->GetGamePlayTime();

			float fPostTime = pEngine->m_pTimer->GetGamePlayTime();

			if (fPostTime - fLastFireTime > (10 - m_fMoveSpeed)*50)
			{
				fLastFireTime = fPostTime;

				Fire();
				D3DXVECTOR3 vFirePosition;
				vFirePosition = m_vPosition + (m_vDirectionLook * 20);
				vFirePosition.y += 8;

				CParticle* pParticle = NULL;
				pParticle = pEngine->m_pParticleMgr->CreateParticle(500, pEngine->m_vTextures[TEXTURE_INDEX_PARTICLE]);
				//pParticle->Explode(vFirePosition);
				//pParticle->Shoot(vFirePosition, m_vDirectionLook);
				pParticle->Fire(vFirePosition, m_vDirectionLook);

			}
		}
	}
}

void CTank::Render( void )
{
	CGraphicsEngine::GetInstance()->AddModel(m_pTopModel);
	CGraphicsEngine::GetInstance()->AddModel(m_pBottomModel);
}

void CTank::Fire( void )
{
 	CMessageMgr* pMsgMgr =NULL;
	FireParameter* pParameter = NULL;

	pMsgMgr = CMessageMgr::GetInstance();
	pParameter = new FireParameter();

	pParameter->vPosition = m_vPosition + m_vDirectionLook*20.f;
	pParameter->vDirection = m_vDirectionLook;
	pParameter->fAngle = m_fAngleLook + m_fAngleMove;

 	pMsgMgr->AddMessage(CMessageMgr::MSG_FIRE, (DWORD)pParameter, NULL);
}

void CTank::Move( float nUnits )
{
	m_vPosition += m_vDirectionMove*nUnits;

	SetMatrix();
}

void CTank::ChangePosition( float fX,float fY,float fZ )
{
	m_vPosition.x += fX;
	m_vPosition.y += fY;
	m_vPosition.z += fZ;

	SetMatrix();
}

void CTank::ChangeDirectionMove( float fAngle )
{
	m_fAngleMove += fAngle;
	D3DXMATRIX tmpMatrix;
	D3DXMatrixRotationY(&tmpMatrix,fAngle);

	D3DXVec3TransformCoord(&m_vDirectionMove,&m_vDirectionMove,&tmpMatrix);
	D3DXVec3Normalize(&m_vDirectionMove,&m_vDirectionMove);

	D3DXVec3TransformCoord(&m_vDirectionLook,&m_vDirectionLook,&tmpMatrix);
	D3DXVec3Normalize(&m_vDirectionLook,&m_vDirectionLook);

	//cprintf("%f,%f,%f\n",m_vDirection.x,m_vDirection.y,m_vDirection.z);
	SetMatrix();
}

void CTank::SetMatrix( void )
{
	D3DXMATRIX tmpMatrix;
	D3DXMatrixRotationY(&m_pTopModel->m_mtrWorldMatrix,m_fAngleMove+m_fAngleLook);
	D3DXMatrixTranslation(&tmpMatrix,m_vPosition.x,m_vPosition.y,m_vPosition.z);
	m_pTopModel->m_mtrWorldMatrix *= tmpMatrix;

	D3DXMatrixRotationY(&m_pBottomModel->m_mtrWorldMatrix,m_fAngleMove);
	D3DXMatrixTranslation(&tmpMatrix,m_vPosition.x,m_vPosition.y,m_vPosition.z);
	m_pBottomModel->m_mtrWorldMatrix *= tmpMatrix;
}

void CTank::SetPosition( float fX,float fY,float fZ )
{
	m_vPosition.x = fX;
	m_vPosition.y = fY;
	m_vPosition.z = fZ;
	SetMatrix();
}

void CTank::SetDirectionMove( float fAngle )
{
	m_fAngleMove = fAngle;
	SetMatrix();
}

void CTank::SetDirectionLook( float fAngle )
{
	m_fAngleLook = fAngle;
	SetMatrix();
}

void CTank::ChangeDirectionLook( float fAngle )
{
	m_fAngleLook += fAngle;
	D3DXMATRIX tmpMatrix;
	D3DXMatrixRotationY(&tmpMatrix,fAngle);

	D3DXVec3TransformCoord(&m_vDirectionLook,&m_vDirectionLook,&tmpMatrix);
	D3DXVec3Normalize(&m_vDirectionLook,&m_vDirectionLook);

	SetMatrix();
}
