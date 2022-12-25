#pragma once

#include <list>
#include "Explosion.h"

class ExplosionFactory
{
private:
	std::list<Explosion*> m_pExplosion;
	int m_TextureNo = -1;
public:
	ExplosionFactory();
	// “G‚ğì¬
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Init();
	// ”š’eHê‚ÌI—¹ˆ—
	void Uninit();
	// ”š’eHê‚ÌXVˆ—
	void Update();
	// ”š’eHê‚Ì•`‰æˆ—
	void Draw();
	~ExplosionFactory();

	// ƒvƒŒƒCƒ„[‚Æ”š•—‚Ì“–‚½‚è”»’è
	void CollisionBlastToPlayer();
};

