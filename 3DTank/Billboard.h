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
	D3DXMATRIX m_matWorld;
	int m_nTextureID;
};
