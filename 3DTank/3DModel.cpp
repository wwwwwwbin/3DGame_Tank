#include "StdAfx.h"
#include "3DModel.h"


C3DModel::C3DModel(void)
{
	m_nNumOfMaterials = 0;
	m_pD3DMaterialArray = NULL;
	m_ppDirect3DTextureArray = NULL;
	m_pD3DXMesh = NULL;
}


C3DModel::~C3DModel(void)
{
}
