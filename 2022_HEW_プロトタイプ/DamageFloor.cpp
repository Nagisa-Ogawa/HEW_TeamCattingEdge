#include "DamageFloor.h"
#include "texture.h"
#include "camera.h"
#include "sprite.h"

DamageFloor::DamageFloor(D3DXVECTOR2 pos, int textureNo):m_Pos(pos),m_TextureNo(textureNo)
{
	m_Size = D3DXVECTOR2(1860.0f, 62.0f);
	m_IsActive = true;
	m_divid = D3DXVECTOR2(4.0f, 2.0f);
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
	if (m_WaitFrame >= 5)
	{
		m_WaitFrame = 0;
		m_AnimationPtn++;
		if (m_AnimationPtn >= 4)
		{
			m_AnimationPtn = 0;
		}
	}
	else
	{
		m_WaitFrame++;
	}
}

void DamageFloor::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], M_MukiTable[m_Muki] + 7.5f, m_pttern.y);
	}
}

DamageFloor::~DamageFloor()
{
}
