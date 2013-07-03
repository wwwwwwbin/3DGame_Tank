#include "StdAfx.h"
#include "3DModelFactory.h"
#include "GraphicsEngine.h"

#include "Bullet.h"

CBullet::CBullet( D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection, float fAngle )
{
	m_nLife = 500;
	m_fVelocity = 2.f;
	m_bIsDead = false;
	m_fAngle = fAngle + D3DX_PI/2.f;
	m_vPosition = vPosition;
	m_vDirection = vDirection;
	
	m_pModel = new C3DModel();
	C3DModelFactory::GetInstance()->GetModel(m_pModel,C3DModelFactory::GM_BULLET);

	SetMatrix();
}

CBullet::~CBullet(void)
{
	delete m_pModel;
}

void CBullet::Update( void )
{
	D3DXMATRIX tmpMatrix;

	if (!m_bIsDead)
	{
		m_vPosition += m_vDirection * m_fVelocity;
		SetMatrix();
		-- m_nLife;
		if (m_nLife <= 0)
			m_bIsDead = true;
	}
}

void CBullet::Render( void )
{
	CGraphicsEngine::GetInstance()->AddModel(m_pModel);
}

void CBullet::SetMatrix( void )
{
	D3DXMATRIX tmpMatrix;
	//D3DXMatrixScaling(&tmpMatrix, 2225.2f, 2225.2f, 2225.2f);
	D3DXMatrixRotationY(&m_pModel->m_mtrWorldMatrix,m_fAngle);
	D3DXMatrixTranslation(&tmpMatrix,m_vPosition.x,m_vPosition.y,m_vPosition.z);
	m_pModel->m_mtrWorldMatrix *= tmpMatrix;
}
