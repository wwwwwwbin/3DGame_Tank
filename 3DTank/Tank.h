#pragma once
#include "3DModel.h"

class CTank
{
private:
	C3DModel* m_pTopModel;
	C3DModel* m_pBottomModel;

	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vDirectionMove;
	D3DXVECTOR3 m_vDirectionLook;
	float m_fAngleMove;
	float m_fAngleLook;
	float m_fMoveSpeed;
	//bool m_bHasInit;
public:
	CTank(float fX,float fY,float fZ);
	~CTank(void);
	int Updata(void);
	void Render(void);

	void Fire(void);
	void Move(float nUnits);
	void SetPosition(float fX,float fY,float fZ);
	void ChangePosition(float fX,float fY,float fZ);
	void SetDirectionMove(float fAngle);
	void ChangeDirectionMove(float fAngle);
	void SetDirectionLook(float fAngle);
	void ChangeDirectionLook(float fAngle);
	void SetMatrix(void);
	D3DXVECTOR3 GetPosition(void){return m_vPosition;}
	
};

