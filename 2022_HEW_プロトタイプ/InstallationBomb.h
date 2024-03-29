#pragma once
#include "Bomb.h"

class ShockWaveFactory;

class InstallationBomb : public Bomb
{
public:
	enum STATE_INSTALLATIONBOMB
	{
		THROW,
		SET,
		EXPLOSION,
	};
private:

	ShockWaveFactory* m_pShockWaveFactory;

	int m_TextureNo = 0;		// eªgp·éeNX`ÌÔ
	bool m_IsGround = false;
	float m_Gravity = 0.0f;
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);

	STATE_INSTALLATIONBOMB m_State = STATE_INSTALLATIONBOMB::THROW;
public:
	InstallationBomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos);
	// eÌú»
	void Init() override;
	// eÌI¹
	void Uninit() override;
	// eÌXV
	void Update() override;
	// eÌ`æ
	void Draw() override;
	~InstallationBomb();

	// GetnÖ
	STATE_INSTALLATIONBOMB GetState() { return m_State; };

	// ÕgÆÌ½è»èpÖ
	void CollisionBombToShockWave();
	// lpÌ½è»è
	bool HitCheckSquare(D3DXVECTOR2 posA, D3DXVECTOR2 sizeA, D3DXVECTOR2 posB, D3DXVECTOR2 sizeB);

};

