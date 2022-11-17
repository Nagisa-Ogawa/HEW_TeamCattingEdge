#pragma once

#define ENEMY_MONO_SUZE 64.0f

class ENEMY_PROT
{
private:
	float m_posX;
	float m_posY;
	bool m_live = true;
protected:
	int	  m_hp = 0;
	float m_size = ENEMY_MONO_SUZE;
public:
	ENEMY_PROT() {}
	ENEMY_PROT(float x, float y) :m_posX(x), m_posY(y){}

	virtual float GetPosX(void) { return m_posX; }
	virtual float GetPosY(void) { return m_posY; }
	virtual float GetSize(void) { return m_size; }
	virtual bool GetLive(void) { return m_live; }

	void SetHP(int hp) { m_hp = hp; }

	virtual void Update(void) = 0;

	void Damage(int how) {
		m_hp -= how;
		if (m_hp >= 0)
			m_live = true;
		else
			m_live = false;
	}
};