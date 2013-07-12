//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   XFileMesh.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-08 19:20
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XFileMesh.h"
#include "GraphicsEngine.h"

CXFileMesh::CXFileMesh()
: m_dwMaterials(0)
, m_pD3DXMesh(NULL)
, m_pMaterialBuffer(NULL)
, m_pAdjacencyBuffer(NULL)
, m_pD3DMaterialArray(NULL)
, m_ppDirect3DTextureArray(NULL)
{
	m_pIDirect3DDevice = CGraphicsEngine::GetInstance()->GetD3DDevice();
}


CXFileMesh::~CXFileMesh(void)
{
	Direct3DRelease();
}

int CXFileMesh::LoadXFile( TCHAR* szXFileName )
{	
	int				nResult  		= false;
	HRESULT 		hRetCode 		= E_FAIL;
	D3DXMATERIAL*	pD3DXMaterial	= NULL;

	LOG_FAILD_JUMP(szXFileName);
	
	hRetCode = D3DXLoadMeshFromX(
		szXFileName,
		D3DXMESH_IB_MANAGED,
		m_pIDirect3DDevice,
		&m_pAdjacencyBuffer,
		&m_pMaterialBuffer,
		0,
		&m_dwMaterials,
		&m_pD3DXMesh
	);
	
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, szXFileName, _T("--!装入.X文件失败"), NULL);
		goto Exit0;
	}
	if (m_pMaterialBuffer == NULL || m_dwMaterials == 0)
	{
		MessageBox(NULL, _T("装入.X文件失败!!"), NULL, NULL);
		goto Exit0;
	}
	m_pAdjacencyBuffer->GetBufferPointer();
	pD3DXMaterial = (D3DXMATERIAL*)m_pMaterialBuffer->GetBufferPointer();
	if (pD3DXMaterial != NULL)
	{
		m_pD3DMaterialArray = new D3DMATERIAL9[m_dwMaterials];
		LOG_FAILD_JUMP(m_pD3DMaterialArray);

		m_ppDirect3DTextureArray = new IDirect3DTexture9*[m_dwMaterials];
		LOG_FAILD_JUMP(m_ppDirect3DTextureArray);

		for (DWORD i = 0; i < m_dwMaterials; ++i)
		{
			m_pD3DMaterialArray[i] = pD3DXMaterial[i].MatD3D;
			if (pD3DXMaterial[i].pTextureFilename != NULL)
			{
				hRetCode = D3DXCreateTextureFromFileA(
					m_pIDirect3DDevice,
					pD3DXMaterial[i].pTextureFilename,
					&m_ppDirect3DTextureArray[i]
				);
				if (FAILED(hRetCode))
				{
					m_ppDirect3DTextureArray[i] = NULL;
					MessageBox(NULL, _T("纹理加载错误"), NULL, NULL);
				}
			}
		}
	}
	m_pD3DXMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)m_pAdjacencyBuffer->GetBufferPointer(),
		NULL,
		NULL,
		NULL
	);
	nResult = true;
Exit0:
	if (m_pMaterialBuffer)
	{
		m_pMaterialBuffer->Release();
	}
	if (m_pAdjacencyBuffer)
	{
		m_pAdjacencyBuffer->Release();
	}
	return nResult;
}
void CXFileMesh::Direct3DRelease()
{
	for (DWORD i = 0; i < m_dwMaterials; ++i)
	{
		SafeRelease(m_ppDirect3DTextureArray[i]);
	}
	SafeRelease(m_pD3DXMesh);
}