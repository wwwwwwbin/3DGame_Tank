#pragma once
#include "gamestate.h"
#include "SceneMgr.h"
class CGSWar :
	public CGameState
{
private:
	bool m_bHasInit;
	CSceneMgr* m_pControllor;
public:
	CGSWar(void);
	~CGSWar(void);
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Enter(void);
	virtual void Leave(void);
};

