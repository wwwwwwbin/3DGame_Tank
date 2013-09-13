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

#include "GraphicsEngineDef.h"

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

	int m_nTextureID;
	QuadVertex m_vVertex[POS2D_COUNT];

	int Init(
		const char* cpszTexPath, 
		D3DXVECTOR3 vTopLeft, 
		D3DXVECTOR3 vTopRight, 
		D3DXVECTOR3 vBottomRight,
		D3DXVECTOR3 vBottomLeft
	);
};

