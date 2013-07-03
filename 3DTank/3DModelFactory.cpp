#include "StdAfx.h"
#include "3DModelFactory.h"


C3DModelFactory::C3DModelFactory(void)
{
	//ZeroMemory(m_vXFiles,sizeof(m_vXFiles));
	LoadXFile();
}


C3DModelFactory::~C3DModelFactory(void)
{
	m_vXFiles[GM_WALL].m_pD3DXMesh->Release();
	m_vXFiles[GM_TANKTOP].m_pD3DXMesh->Release();
	m_vXFiles[GM_TANKBOTTOM].m_pD3DXMesh->Release();
}

C3DModelFactory* C3DModelFactory::GetInstance( void )
{
	static C3DModelFactory* m_pInstance = NULL;
	if(m_pInstance == NULL){
		m_pInstance = new C3DModelFactory();
	}
	return m_pInstance;
}

void C3DModelFactory::LoadXFile( void )
{
	m_vXFiles[GM_WALL].LoadXFile(_T("wall.x"));
	m_vXFiles[GM_TANKTOP].LoadXFile(_T("tankTop.x"));
	m_vXFiles[GM_TANKBOTTOM].LoadXFile(_T("tankBottom.x"));
	m_vXFiles[GM_BULLET].LoadXFile(_T("bullet.x"));
}

void C3DModelFactory::GetModel( C3DModel* pModel,GAMEMODEL eModelType )
{
	CXFileMesh* pXFile = &m_vXFiles[eModelType];
	pModel->m_nNumOfMaterials = pXFile->m_dwMaterials;
	pModel->m_pD3DMaterialArray = pXFile->m_pD3DMaterialArray;
	pModel->m_ppDirect3DTextureArray = pXFile->m_ppDirect3DTextureArray;
	pModel->m_pD3DXMesh = pXFile->m_pD3DXMesh;
}
