//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   XFileMesh.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-08 19:20
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once

class CXFileMesh
{
	friend class C3DModelFactory;
public:
	CXFileMesh();
	~CXFileMesh(void);
	int LoadXFile(TCHAR* szXFileName);
	void Direct3DRelease();

private:
	DWORD				m_dwMaterials;
	ID3DXMesh*			m_pD3DXMesh;
	ID3DXBuffer*		m_pAdjacencyBuffer;
	ID3DXBuffer*		m_pMaterialBuffer;
	D3DMATERIAL9*		m_pD3DMaterialArray;
	IDirect3DDevice9*	m_pIDirect3DDevice;
	IDirect3DTexture9** m_ppDirect3DTextureArray;
};

