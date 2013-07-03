#pragma once
class CQuad
{
public:
	struct QuadVertex{
		float fPosX,fPosY,fPosZ;
		float fTexX,fTexY;
		static const DWORD FVF = D3DFVF_XYZ|D3DFVF_TEX1;
	};
	// 0-3 分别是左上，右上，右下，左下4点
	QuadVertex m_vVertex[4];
	IDirect3DTexture9* m_pTexture;

public:
	CQuad(void);
	~CQuad(void);
};

