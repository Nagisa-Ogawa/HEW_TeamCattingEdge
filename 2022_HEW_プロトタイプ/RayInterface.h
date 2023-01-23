#pragma once
#include "main.h"

#define RAYSPEED 6.0f
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
public:
	RayInterface(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos)
		:m_pos(pos), m_goalpos(playerpos) {};

	virtual void Init(void) = 0;
	virtual void Update(void)= 0;
	virtual void Draw(void)	= 0;
	virtual void Uninit(void) = 0;

	bool GetUse() {	return m_use;}
};