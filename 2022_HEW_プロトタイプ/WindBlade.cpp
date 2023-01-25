#include "WindBlade.h"
#include "sprite.h"
#include "camera.h"

WindBlade::WindBlade(D3DXVECTOR2 pos,D3DXVECTOR2 size, int muki, int textureNo):m_Pos(pos),m_Size(size),m_Muki(muki),m_TextureNo(textureNo)
{
	m_pCamera = GetCamera();
	m_Power = D3DXVECTOR2(20.0f, 0.0f);
	m_divid = D3DXVECTOR2(2.0f, 4.0f);
	m_pttern.x = 1.0f / m_divid.x;
	m_pttern.y = 1.0f / m_divid.y;
	m_IsActive = true;
	if (m_Muki == 1)
	{
		m_Power.x *= -1.0f;
	}
	if (m_Muki == 1)
	{
		m_Muki = 0;
	}
	else
	{
		m_Muki = 1;
	}
	m_Muki += 2;
}

void WindBlade::Init()
{
}

void WindBlade::Uninit()
{
}

void WindBlade::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
	switch (m_State)
	{
	case WindBlade::IDLE:
		m_State = MOVE;
		break;
	case WindBlade::MOVE:
	{
		D3DXVECTOR2 basePos = GetBase();
		m_Vel.x += m_Power.x;
		m_Vel.y += m_Power.y;
		m_Pos += m_Vel;
		// ‰æ–ÊŠO‚È‚çpCamer
		if (m_Pos.x - m_pCamera->pos.x <= -500.0f || m_Pos.x - m_pCamera->pos.x >= SCREEN_WIDTH + 500.0f)
		{
			m_State = DEAD;
		}
		if (m_WaitFrame >= 10)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 2)
			{
				m_AnimationPtn = 0;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	}
	case WindBlade::DEAD:
		m_IsActive = false;
		break;
	default:
		break;
	}
}

void WindBlade::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

WindBlade::~WindBlade()
{
}
