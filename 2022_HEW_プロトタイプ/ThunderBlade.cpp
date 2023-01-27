#include "ThunderBlade.h"
#include "sprite.h"
#include "camera.h"

ThunderBlade::ThunderBlade(D3DXVECTOR2 pos, D3DXVECTOR2 size,int textureNo,ThunderBlade::THUNDERBLADE_MODE mode,bool isDuo):
	m_Pos(pos),m_Size(size),m_TextureNo(textureNo),m_IsDuo(isDuo)
{
	if (mode == THUNDERBLADE_MODE::NORMAL)
	{
		m_CollisionSize = D3DXVECTOR2(size.x / 4.0f, size.y);
	}
	else
	{
		m_CollisionSize = D3DXVECTOR2(size.x-250.0f, size.y);
	}
	
	m_pCamera = GetCamera();
	m_divid = D3DXVECTOR2(13.0f, 1.0f);
	m_pttern.x = 1.0f / m_divid.x;
	m_pttern.y = 1.0f / m_divid.y;
	m_IsActive = true;
}

void ThunderBlade::Init()
{
}

void ThunderBlade::Uninit()
{
}

void ThunderBlade::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
	switch (m_State)
	{
	case ThunderBlade::IDLE:
		m_State = THUNDERBOLT;
		break;
	case ThunderBlade::THUNDERBOLT:
		if (m_WaitFrame >= 2)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn == 4)
			{
				m_IsCanHit = true;
			}
			else if (m_AnimationPtn >= 12)
			{
				m_IsCanHit = false;
				m_State = DEAD;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case ThunderBlade::DEAD:
		m_IsActive = false;
		break;
	default:
		break;
	}
}

void ThunderBlade::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

ThunderBlade::~ThunderBlade()
{
}

void ThunderBlade::ThunderBolt()
{
}
