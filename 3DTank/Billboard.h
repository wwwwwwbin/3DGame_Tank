//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Billboard.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once

class CBillboard
{
	friend class CGraphicsEngine;
public:
	CBillboard(void);
	~CBillboard(void);

	int	 Init(int nTextureID, D3DXVECTOR3& pos);
	void SetPosition(float fX, float fY, float fZ);

private:
	int			m_nTextureID;
	D3DXMATRIX	m_matWorld;
};
