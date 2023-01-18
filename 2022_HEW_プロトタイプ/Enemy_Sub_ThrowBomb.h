#pragma once
#include "Enemy_ThrowBomb.h"

class Enemy_Sub_ThrowBomb:public Enemy_ThrowBomb
{
public:
	Enemy_Sub_ThrowBomb(D3DXVECTOR2 pos, int ID, int textureNo);
	~Enemy_Sub_ThrowBomb();

	void Uninit(void) override;
	void Update(void)override;
};

