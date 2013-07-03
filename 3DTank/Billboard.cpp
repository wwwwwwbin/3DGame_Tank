#include "StdAfx.h"
#include "Billboard.h"
#include "EngineDefine.h"
#include "GraphicsEngine.h"

CBillboard::CBillboard(void)
: m_nTextureID(TEXTURE_INDEX_INVALD)
{
}

CBillboard::~CBillboard(void)
{
}

int CBillboard::Init( int nTextureID, D3DXVECTOR3& pos )
{
	int nResult = FALSE;

	CHECK_FAILD_JUMP(nTextureID >= TEXTURE_INDEX_BEGIN);
	CHECK_FAILD_JUMP(nTextureID < TEXTURE_INDEX_END);

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