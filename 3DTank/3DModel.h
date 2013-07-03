#pragma once
class C3DModel
{
public:
	D3DXMATRIX m_mtrWorldMatrix;
	int m_nNumOfMaterials;
	D3DMATERIAL9* m_pD3DMaterialArray;
	IDirect3DTexture9** m_ppDirect3DTextureArray;
	ID3DXMesh* m_pD3DXMesh;
public:
	C3DModel(void);
	~C3DModel(void);
};
