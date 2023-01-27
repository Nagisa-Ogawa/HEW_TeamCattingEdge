#pragma once
#include "main.h"

#define RAYSPEED 10.0f
#define MAXSIZE 300.0f

enum RayState
{
	Normal,
	Expo,
};

struct PLAYER;

class RayInterface
{
protected:
	D3DXVECTOR2 m_pos;
	D3DXVECTOR2 m_goalpos;
	D3DXVECTOR2 m_vec;
	float		m_size = 60.0f;
	PLAYER*		m_pPlayer;
	int			m_texture;
	RayState	m_state;
	bool		m_use = true;
	bool		m_IsDuo = false;
public:
	RayInterface(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos,bool isDuo)
		:m_pos(pos), m_goalpos(playerpos),m_IsDuo(isDuo) {};

	virtual void Init(void) = 0;
	virtual void Update(void)= 0;
	virtual void Draw(void)	= 0;
	virtual void Uninit(void) = 0;

	bool GetUse() {	return m_use;}
	bool GetIsDuo() { return m_IsDuo; };
};