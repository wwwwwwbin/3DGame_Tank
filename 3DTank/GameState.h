#pragma once
class CGameState
{
protected:
	bool m_bHasInit;
public:
	CGameState(void);
	virtual ~CGameState(void);

	virtual void Initialize(void) = NULL;
	virtual void Update(void) = NULL;
	virtual void Render(void) = NULL;
	virtual void Enter(void) = NULL;
	virtual void Leave(void) = NULL;
};

