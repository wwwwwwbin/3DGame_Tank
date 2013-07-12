#include "StdAfx.h"
#include "Tree.h"
#include "GraphicsEngine.h"

CTree::CTree(void)
: m_pBillboard(NULL)
{
	Init();
}

CTree::~CTree(void)
{
}

int CTree::Init( void )
{
	int nRetCode	= FALSE;
	int nResult		= FALSE;
	int nTextureID	= 0;

	D3DXVECTOR3 treeDefalut(50, 50, 0);

	m_pBillboard = new CBillboard();
	LOG_FAILD_JUMP(m_pBillboard);

	nRetCode = TextureMgr::GetInstance()->LoadTexture("tree.png", nTextureID);
	LOG_FAILD_JUMP(nRetCode);

	nRetCode = m_pBillboard->Init(nTextureID, treeDefalut);
	LOG_FAILD_JUMP(nRetCode);

	nResult = TRUE;
Exit0:
	if (!nResult && m_pBillboard)
	{
		delete m_pBillboard;
		m_pBillboard = NULL;
	}
	return nResult;
}

void CTree::SetPosition( float fX, float fY, float fZ )
{
	m_pBillboard->SetPosition(fX, fY , fZ);
}

void CTree::Render( void )
{
	CGraphicsEngine::GetInstance()->AddBillboard(m_pBillboard);
}
