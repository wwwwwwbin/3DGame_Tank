#pragma once

#define PARTICLE_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define fRANDOM ( ((float)rand()/RAND_MAX) )

#define COLOR_A (D3DCOLOR_XRGB(255,255,0))
#define COLOR_B (D3DCOLOR_XRGB(255,223,0))
#define COLOR_C (D3DCOLOR_XRGB(255,138,0))


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
private:
	IDirect3DTexture9* m_pParticleTexture;
	IDirect3DVertexBuffer9* m_pParticleVertexBuf;
	PARTICLE* m_pParticles;
	int m_nNumOfParticles;
	D3DXVECTOR3 m_vPosition;
	float m_fLife;
	float m_fMass;
	D3DCOLOR m_Color;
	float m_fFriction;
	D3DXVECTOR3 m_vGravity;
	float m_fRadius;
	long m_lVertexBufOffset;
	long m_lBolckSize;

	bool m_bIsDead;

public:
	CParticle(void);
	~CParticle(void);

	bool Init(int nNumOfParticle, IDirect3DTexture9* pParticleTexture);
	void CreateParticle(float fVx, float fVy, float fVz);
	void UpdataParticle(void);
	void Render(void);

	static void SetParticleRS(void);
	static void UnSetParticleRS(void);

	void Explode(D3DXVECTOR3 vPosition);
	void Shoot(D3DXVECTOR3 vStart, D3DXVECTOR3 vDirection);
	void Fire(D3DXVECTOR3 vStart, D3DXVECTOR3 vDirection);

	void SetLife(float fLife){m_fLife = fLife;}
	void SetMass(float fMass){m_fMass = fMass;}
	void SetPosition(float fX, float fY, float fZ){m_vPosition = D3DXVECTOR3(fX, fY, fZ);}
	void SetColor(int r, int g, int b){m_Color = D3DCOLOR_XRGB(r, g, b);}
	void SetFriction(float fFrition){m_fFriction = fFrition;}
	void SetGravity(float fX, float fY, float fZ){m_vGravity = D3DXVECTOR3(fX, fY, fZ);}
	void SetRadius(float fRadius){m_fRadius = fRadius;}

	bool IsDead(void){return m_bIsDead;}

private:
	void UnInit(void);
};
