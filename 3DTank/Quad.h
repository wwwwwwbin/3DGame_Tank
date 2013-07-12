//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Quad.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

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

	// 0-3 分别是左上，右上，右下，左下4点
	int m_nTextureID;
	QuadVertex m_vVertex[4];
};

