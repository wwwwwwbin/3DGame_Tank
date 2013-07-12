//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Particle.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define PARTICLE_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define fRANDOM ( ((float)rand()/RAND_MAX) )

#define COLOR_A (D3DCOLOR_XRGB(255,255,0))
#define COLOR_B (D3DCOLOR_XRGB(255,223,0))
#define COLOR_C (D3DCOLOR_XRGB(255,138,0))


class CSetParticleRS
{
public:
	CSetParticleRS(void);
	~CSetParticleRS(void);
};

class CParticle
{
	struct PARTICLE_RENDERSTRUCT
	{
		D3DXVECTOR3 m_vPosition;
		D3DCOLOR m_Color;
	};
	struct PARTICLE
	{
		D3DXVECTOR3 m_vPosition;
		D3DCOLOR m_Color;
		float m_fLife;
		D3DXVECTOR3 m_vVelocity;
		float m_fSize;
		float m_fMass;
		float m_fFriction;
	};

public:
	CParticle(void);
	~CParticle(void);

	int  Init(int nNumOfParticle, int nTextureID);
	void UnInit(void);

	void CreateParticle(float fVx, float fVy, float fVz);
	void UpdataParticle(void);
	int  Render(void);

	static void SetParticleRS(void);
	static void UnSetParticleRS(void);

	void Explode(D3DXVECTOR3 vPosition);
	void Shoot(D3DXVECTOR3 vStart, D3DXVECTOR3 vDirection);
	void Fire(D3DXVECTOR3 vStart, D3DXVECTOR3 vDirection);

	void SetLife(float fLife);
	void SetMass(float fMass);
	void SetPosition(float fX, float fY, float fZ);
	void SetColor(int r, int g, int b);
	void SetFriction(float fFrition);
	void SetGravity(float fX, float fY, float fZ);
	void SetRadius(float fRadius);

	bool IsDead(void);

private:
	int		m_nTextureID;
	int		m_nNumOfParticles;
	long	m_lVertexBufOffset;
	long	m_lBolckSize;
	bool	m_bIsDead;
	float	m_fLife;
	float	m_fMass;
	float	m_fFriction;
	float	m_fRadius;

	D3DCOLOR	m_Color;
	D3DXVECTOR3 m_vGravity;
	D3DXVECTOR3 m_vPosition;
	PARTICLE*	m_pParticles;
	
	IDirect3DVertexBuffer9* m_pParticleVertexBuf;
};
