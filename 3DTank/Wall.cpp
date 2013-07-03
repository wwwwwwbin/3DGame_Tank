#include "StdAfx.h"
#include "Wall.h"
#include "3DModelFactory.h"
#include "GraphicsEngine.h"


CWall::CWall(float fX,float fY,float fZ)
{
	m_vPosition.x = fX;
	m_vPosition.y = fY;
	m_vPosition.z = fZ;

	m_pModel = new C3DModel();
	C3DModelFactory::GetInstance()->GetModel(m_pModel,C3DModelFactory::GM_WALL);
	SetMatrix();
}


CWall::~CWall(void)
{
	delete m_pModel;
}

void CWall::Render( void )
{
	CGraphicsEngine::GetInstance()->AddModel(m_pModel);
}

void CWall::SetMatrix( void )
{
	D3DXMatrixTranslation(&m_pModel->m_mtrWorldMatrix,m_vPosition.x,m_vPosition.y,m_vPosition.z);
}
