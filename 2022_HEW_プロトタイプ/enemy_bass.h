#pragma once

#include "enemy_cloud.h"
#include "enemy_turret.h"

class ENEMY_BASS:public ENEMY_CLOUD
{
private:
	int muki = 0;
	int m_frame = 0;
	ENEMY_BASS();
public:
	ENEMY_BASS(float x, float y, float rot, float ratio, int frame) :ENEMY_CLOUD(x, y, rot, ratio) { m_hp = 100;  m_size = ENEMY_MONO_SUZE * 2; };

	void Update(void) override{
		ENEMY_CLOUD::Update();

		m_frame++;

		if (m_frame >= 120)
		{
			SetBullet(D3DXVECTOR2(ENEMY_PROT::GetPosX(), ENEMY_PROT::GetPosY() + 32.0f), (muki++ % 4));

			m_frame = 0;
		}
	}
};
