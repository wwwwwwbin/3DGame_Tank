#pragma once

#include "3DModel.h"

class CBullet
{
private:
	bool m_bIsDead;
	int m_nLife;
	float m_fAngle;
	float m_fVelocity;
	C3DModel* m_pModel;
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vDirection;

public:
	CBullet(D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection, float fAngle);
	~CBullet(void);

	bool IsDead(void){return m_bIsDead;}
	void SetLife(int nLife){m_nLife = nLife;}
	void SetVelocity(float fVelocity){m_fVelocity = fVelocity;}

	void Update(void);
	void Render(void);

private:
	void SetMatrix(void);
};
