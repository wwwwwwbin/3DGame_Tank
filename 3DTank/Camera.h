#pragma once
#include "EngineDefine.h"

class CCamera
{

public:
	static CCamera* GetInstance(CameraType eCameraType);

	void SetCameraType(CameraType eCameraType);

	void Strafe(float fUnits);
	void Fly(float fUnits);
	void Walk(float fUnits);

	void Pitch(float fAngle);
	void Yaw(float fAngle);
	void Roll(float fAngle);

	void MovePosition(float fX,float fY,float fZ);
	void PointRotate(D3DXVECTOR3 pos, float fAngle);

	void GetViewMatrix(D3DXMATRIX* pOutput);

	D3DXVECTOR3 GetPosition();
	void SetPosition(const D3DXVECTOR3& pos);

	D3DXVECTOR3 GetRight();
	void SetRight(const D3DXVECTOR3& pos);

	D3DXVECTOR3 GetUp();
	void SetUp(const D3DXVECTOR3& pos);

	D3DXVECTOR3 GetLook();
	void SetLook(const D3DXVECTOR3& pos);

private:
	CCamera(CameraType eCameraType);
	~CCamera(void);

	CameraType m_eCameraType;
	
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vUP;
	D3DXVECTOR3 m_vLook;
};

