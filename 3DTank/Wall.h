#pragma once
#include "3DModel.h"

class CWall
{
private:
	C3DModel* m_pModel;
	D3DXVECTOR3 m_vPosition;

public:
	CWall(float fX,float fY,float fZ);
	~CWall(void);
	void SetMatrix(void);
	void Render(void);
};

