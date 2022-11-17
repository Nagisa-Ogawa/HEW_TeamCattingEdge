#pragma once

#include "enemy_prototype.h"
#include "bullet.h"

class ENEMY_TURRET :public ENEMY_PROT
{
private:
	int m_muki = 0;
	int m_frame = 0;
public:
	ENEMY_TURRET(float x,float y,int a): ENEMY_PROT(x,y),m_muki(a){}

	virtual void Update(void) override {
		m_frame++;

		if (m_frame >= 120)
		{
			SetBullet(D3DXVECTOR2(ENEMY_PROT::GetPosX(), ENEMY_PROT::GetPosY() + 32.0f), m_muki);

			m_frame = 0;
		}
	};
};