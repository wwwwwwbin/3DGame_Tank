#pragma once
#include "GraphicsEngine.h"
#include "GameStateManager.h"
class CGameApp
{
private:

	float m_fGameTime;
	CGraphicsEngine* m_pEngine;
	CGameStateManager* m_pGameStateManager;
	static const int FPSFLAG = 10;

public:
	CGameApp(void);
	~CGameApp(void);
	int  Initialize(void);
	void Run(void);
	void Exit(void);
};

