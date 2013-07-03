#pragma once
#include "GameInformation.h"
#include "GameState.h"
class CGameStateManager
{
private:
	bool m_bHasInit;
	CGameInformation::GAMESTATE m_ePreGameState,m_ePosGameState;
	CGameState* m_vGameState[CGameInformation::GS_SIZE];
private:
		void Initialize(void);
public:
	CGameStateManager(void);
	~CGameStateManager(void);
	void Update(void);
	void Render(void);
};

