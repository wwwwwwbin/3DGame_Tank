#pragma once
#include "XFileMesh.h"
#include "3DModel.h"

class C3DModelFactory
{
public:
	enum GAMEMODEL{GM_TANKTOP,GM_TANKBOTTOM,GM_WALL,GM_BULLET,GM_SIZE};
public:
	CXFileMesh m_vXFiles[GM_SIZE];
public:
	void GetModel(C3DModel* pModel,GAMEMODEL eModelType);
	static C3DModelFactory* GetInstance(void);
private:
	C3DModelFactory(void);
	~C3DModelFactory(void);
	void LoadXFile(void);
};

