#include "DamageFloor.h"
#include "texture.h"
#include "camera.h"
#include "sprite.h"

DamageFloor::DamageFloor(D3DXVECTOR2 pos, int textureNo):m_Pos(pos),m_TextureNo(textureNo)
{
	m_Size = D3DXVECTOR2(1920.0f, 60.0f);
	m_IsActive = true;
	m_divid = D3DXVECTOR2(1.0f, 1.0f);
	m_pttern.x = 1.0f / m_divid.x;
	m_pttern.y = 1.0f / m_divid.y;
}

void DamageFloor::Init()
{
}

void DamageFloor::Uninit()
{
}

void DamageFloor::Update()
{
}

void DamageFloor::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			0.0f, 0.0f, m_pttern.x, m_pttern.y);
	}
}

DamageFloor::~DamageFloor()
{
}
