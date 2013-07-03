#pragma once

class CXFileMesh
{
public:
	ID3DXBuffer* m_pAdjacencyBuffer;
	ID3DXBuffer* m_pMaterialBuffer;
	D3DMATERIAL9* m_pD3DMaterialArray;
	IDirect3DTexture9** m_ppDirect3DTextureArray;
	DWORD m_dwMaterials;
	ID3DXMesh* m_pD3DXMesh;
	IDirect3DDevice9* m_pIDirect3DDevice;

public:
	CXFileMesh();
	~CXFileMesh(void);
	bool LoadXFile(TCHAR* szXFileName);
	void Direct3DRelease();
};

