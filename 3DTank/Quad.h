#pragma once
class CQuad
{
public:
	struct QuadVertex
	{
		float fPosX, fPosY, fPosZ;
		float fTexX, fTexY;
		static const DWORD FVF = D3DFVF_XYZ|D3DFVF_TEX1;
	};

	CQuad(void);
	~CQuad(void);

	// 0-3 �ֱ������ϣ����ϣ����£�����4��
	QuadVertex m_vVertex[4];
	int m_nTextureID;
};

