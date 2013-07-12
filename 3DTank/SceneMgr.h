#pragma once

#include "Tank.h"
#include "Wall.h"
#include "Quad.h"
#include "Tree.h"
#include "Bullet.h"

#include <list>

struct FireParameter
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vDirection;
	float fAngle;
};


class CSceneMgr
{
private:

	typedef std::list<CBullet*> BulletList;

	CTank* m_pTank;
	CWall* m_vWalls[250];
	CQuad* m_pQuad[6];
	CTree* m_pTree[4];
	BulletList m_BulletList;

private:
	int SetSkyBox(void);
public:
	CSceneMgr(void);
	~CSceneMgr(void);

	void Update(void);
	void Render(void);
};

