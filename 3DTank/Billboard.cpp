//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Billboard.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Billboard.h"
#include "EngineDefine.h"
#include "GraphicsEngine.h"

CBillboard::CBillboard(void)
: m_nTextureID(0)
{
}

CBillboard::~CBillboard(void)
{
}

int CBillboard::Init( int nTextureID, D3DXVECTOR3& pos )
{
	int nResult = FALSE;

	m_nTextureID = nTextureID;
	SetPosition(pos.x, pos.y, pos.z);

	nResult = TRUE;
Exit0:
	return nResult;
}

void CBillboard::SetPosition( float fX, float fY, float fZ )
{
	D3DXMatrixTranslation(&m_matWorld, fX, fY, fZ);
}