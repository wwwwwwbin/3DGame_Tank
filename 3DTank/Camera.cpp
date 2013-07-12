//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Camera.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Camera.h"

CCamera::CCamera( CameraType eCameraType )
{
	if (eCameraType >= CAMERA_TYPE_BEGIN && eCameraType < CAMERA_TYPE_END)
	{
		m_eCameraType = eCameraType;
	}
	else
	{
		m_eCameraType = CAMERA_TYPE_LANDOBJECT;
	}
}


CCamera::~CCamera(void)
{
}

void CCamera::GetViewMatrix( D3DXMATRIX* pOutput )
{
	assert(pOutput);

	// 标准化look
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	// 根据look，right叉乘求up
	D3DXVec3Cross(&m_vUP, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUP, &m_vUP);

	D3DXVec3Cross(&m_vRight, &m_vUP, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	float x,y,z;
	x = -D3DXVec3Dot(&m_vRight, &m_vPos);
	y = -D3DXVec3Dot(&m_vUP, &m_vPos);
	z = -D3DXVec3Dot(&m_vLook, &m_vPos);

	(*pOutput)(0,0) = m_vRight.x;
	(*pOutput)(0,1) = m_vUP.x;
	(*pOutput)(0,2) = m_vLook.x;
	(*pOutput)(0,3) = 0.f;

	(*pOutput)(1,0) = m_vRight.y;
	(*pOutput)(1,1) = m_vUP.y;
	(*pOutput)(1,2) = m_vLook.y;
	(*pOutput)(1,3) = 0.f;

	(*pOutput)(2,0) = m_vRight.z;
	(*pOutput)(2,1) = m_vUP.z;
	(*pOutput)(2,2) = m_vLook.z;
	(*pOutput)(2,3) = 0.f;

	(*pOutput)(3,0) = x;
	(*pOutput)(3,1) = y;
	(*pOutput)(3,2) = z;
	(*pOutput)(3,3) = 1.f;
}

void CCamera::Pitch( float fAngle )
{
	D3DXMATRIX matPitch;
	D3DXMatrixRotationAxis(&matPitch, &m_vRight, fAngle);
	D3DXVec3TransformCoord(&m_vUP, &m_vUP, &matPitch);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matPitch);
}

void CCamera::Yaw( float fAngle )
{
	D3DXMATRIX matYaw;
	if(m_eCameraType == CAMERA_TYPE_LANDOBJECT)
	{
		D3DXMatrixRotationY(&matYaw, fAngle);
	}
	if(m_eCameraType == CAMERA_TYPE_AIRCRAFT)
	{
		D3DXMatrixRotationAxis(&matYaw, &m_vUP, fAngle);
	}
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matYaw);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matYaw);
}

void CCamera::Roll( float fAngle )
{
	D3DXMATRIX matRoll;
	if(m_eCameraType == CAMERA_TYPE_AIRCRAFT)
	{
		D3DXMatrixRotationAxis(&matRoll, &m_vLook, fAngle);
		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matRoll);
		D3DXVec3TransformCoord(&m_vUP, &m_vUP, &matRoll);
	}
}

void CCamera::Strafe( float fUnits )
{
	if(m_eCameraType == CAMERA_TYPE_LANDOBJECT)
	{
		m_vPos += D3DXVECTOR3(m_vRight.x * fUnits, 0.f, m_vRight.z * fUnits);
	}
	if(m_eCameraType == CAMERA_TYPE_AIRCRAFT)
	{
		m_vPos += m_vRight * fUnits;
	}
}

void CCamera::Fly( float fUnits )
{
	if(m_eCameraType == CAMERA_TYPE_LANDOBJECT)
	{
		m_vPos.y += fUnits;
	}
	if(m_eCameraType == CAMERA_TYPE_AIRCRAFT)
	{
		m_vPos += m_vUP * fUnits;
	}
}

void CCamera::Walk( float fUnits )
{
	if(m_eCameraType == CAMERA_TYPE_LANDOBJECT)
	{
		m_vPos += D3DXVECTOR3(m_vLook.x * fUnits, 0.f, m_vLook.z * fUnits);
	}
	if(m_eCameraType == CAMERA_TYPE_AIRCRAFT)
	{
		m_vPos += m_vLook * fUnits;
	}
}

void CCamera::SetCameraType( CameraType eCameraType )
{
	m_eCameraType = eCameraType;
}

D3DXVECTOR3 CCamera::GetPosition( void )
{
	return m_vPos;
}

void CCamera::SetPosition( const D3DXVECTOR3& pos )
{
	m_vPos = pos;
}

D3DXVECTOR3 CCamera::GetRight( void )
{
	return m_vRight;
}

void CCamera::SetRight( const D3DXVECTOR3& pos )
{
	m_vRight = pos;
}

D3DXVECTOR3 CCamera::GetUp( void )
{
	return m_vUP;
}

void CCamera::SetUp( const D3DXVECTOR3& pos )
{
	m_vUP = pos;
}

D3DXVECTOR3 CCamera::GetLook( void )
{
	return m_vLook;
}

void CCamera::SetLook( const D3DXVECTOR3& pos )
{
	m_vLook = pos;
}

void CCamera::MovePosition( float fX,float fY,float fZ )
{
	m_vPos.x += fX;
	m_vPos.y += fY;
	m_vPos.z += fZ;
}

void CCamera::PointRotate( D3DXVECTOR3 pos, float fAngle )
{
	D3DXMATRIX matRoate;
	D3DXMatrixRotationY(&matRoate, fAngle);
	m_vPos -= pos;
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matRoate);
	m_vPos += pos;

	Yaw(fAngle);
}

CCamera* CCamera::GetInstance( CameraType eCameraType )
{
	static CCamera* pInstance = NULL;
	if (!pInstance)
	{
		pInstance = new CCamera(eCameraType);
	}
	return pInstance;
}
