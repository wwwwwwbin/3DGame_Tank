#pragma once

#include "Billboard.h"

class CTree
{
private:
	CBillboard* m_pBillboard;
public:
	CTree(void);
	~CTree(void);

	int Init(void);
	void SetPosition(float fX, float fY, float fZ);
	void Render(void);
};
