//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Quad.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Quad.h"
#include "TextureMgr.h"

CQuad::CQuad(void)
: m_nTextureID(0)
{
}

CQuad::~CQuad(void)
{
	if (m_nTextureID != 0)
	{
		TextureMgr::GetInstance()->UnLoadTexture(m_nTextureID);
	}
}
static void SetQuadVertex(CQuad::QuadVertex* pQuadVertex, D3DXVECTOR3 vPos, float fTexX, float fTexY)
{
	assert(pQuadVertex);

	pQuadVertex->fPosX = vPos.x;
	pQuadVertex->fPosY = vPos.y;
	pQuadVertex->fPosZ = vPos.z;

	pQuadVertex->fTexX = fTexX;
	pQuadVertex->fTexY = fTexY;
}

int CQuad::Init(
	const char* cpszTexPath, 
	D3DXVECTOR3 vTopLeft, 
	D3DXVECTOR3 vTopRight, 
	D3DXVECTOR3 vBottomRight,
	D3DXVECTOR3 vBottomLeft
)
{
	int nResult  = FALSE;
	int nRetCode = FALSE;
	
	nRetCode = TextureMgr::GetInstance()->LoadTexture(cpszTexPath, m_nTextureID);
	LOG_FAILD_JUMP(nRetCode);

	SetQuadVertex(&(m_vVertex[POS2D_TOP_LEFT]),		vTopLeft,		0.f, 0.f);
	SetQuadVertex(&(m_vVertex[POS2D_TOP_RIGHT]),	vTopRight,		1.f, 0.f);
	SetQuadVertex(&(m_vVertex[POS2D_BOTTOM_RIGHT]),	vBottomRight,	1.f, 1.f);
	SetQuadVertex(&(m_vVertex[POS2D_BOTTOM_LEFT]),	vBottomLeft,	0.f, 1.f);

	nResult = TRUE;
Exit0:
	return nResult;
}
