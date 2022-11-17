#pragma once

#include "enemy_prototype.h"
#include "main.h"
#include <math.h>

class ENEMY_CLOUD : public ENEMY_PROT
{
private:
	float m_moveposX = 0.0f;
	float m_moveposY = 0.0f;
	float m_rot = 0.0f;
	float m_ratio = 1.0f;
	ENEMY_CLOUD();
public:
	ENEMY_CLOUD(float x, float y, float rot,float ratio) :ENEMY_PROT(x, y),m_rot(rot),m_ratio(ratio) { m_hp = 1; }

	virtual float GetPosX(void) override{ return ENEMY_PROT::GetPosX() + m_moveposX; }
	virtual float GetPosY(void) override{ return ENEMY_PROT::GetPosY() + m_moveposY; }

	virtual void Update(void) override{
		m_moveposX = cos(2 * D3DX_PI / 90.0f * m_rot) * (m_ratio);
		m_moveposY = sin(2 * D3DX_PI / 90.0f * m_rot) * (m_ratio);

		m_rot += 1;

		if (m_rot >= 90)
			m_rot = 0;
	};
};

