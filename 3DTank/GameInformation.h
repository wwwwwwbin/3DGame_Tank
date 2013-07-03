#pragma once
class CGameInformation
{
public:
	enum GAMESTATE{GS_BEGIN,GS_WAR,GS_PASS,GS_SIZE};
public:
	GAMESTATE m_eGameState;
public:
	static CGameInformation* GetInstance(void);
private:
	CGameInformation(void);
	~CGameInformation(void);
};

