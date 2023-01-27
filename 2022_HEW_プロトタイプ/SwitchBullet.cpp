#include "SwitchBullet.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"

SwitchBullet::SwitchBullet(D3DXVECTOR2 pos, D3DXVECTOR2 size, int textureNo,D3DXVECTOR2 targetPos,int muki,SwitchBullet::BULLET_MODE mode,bool isDuo):
		m_Pos(pos), m_Size(size), m_TextureNo(textureNo),m_TargetPos(targetPos),m_Muki(muki),m_mode(mode),m_IsDuo(isDuo)
{
	m_pCamera = GetCamera();
	m_Power = D3DXVECTOR2(20.0f, 0.0f);
	m_divid = D3DXVECTOR2(6.0f, 2.0f);
	m_pttern.x = 1.0f / m_divid.x;
	m_pttern.y = 1.0f / m_divid.y;
	m_TargetDistance = 400.0f;
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
}

void SwitchBullet::Init()
{
}

void SwitchBullet::Uninit()
{
}

void SwitchBullet::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
	switch (m_State)
	{
	case SwitchBullet::IDLE:
		m_State = MOVE;
		break;
	case SwitchBullet::MOVE:
	{
		D3DXVECTOR2 basePos = GetBase();
		m_Vel.x += m_Power.x;
		m_Vel.y += m_Power.y;
		m_Pos += m_Vel;
		switch (m_mode)
		{
		case SwitchBullet::ONCE:
		{
			PLAYER* pPlayer = GetPlayer();
			D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
			float len = pVec.x;
			// プレイヤーを通り過ぎて一定以上
			if (m_Muki == 1) {
				if (len >= m_TargetDistance && m_Pos.x > m_TargetPos.x) {
					m_IsSwitch = true;
					m_State = SWITCH;
					m_SwitchPos = m_Pos;
					m_AnimationPtn = 0;
				}
			}
			else {
				if (len >= m_TargetDistance && m_Pos.x < m_TargetPos.x) {
					m_IsSwitch = true;
					m_State = SWITCH;
					m_SwitchPos = m_Pos;
					m_AnimationPtn = 0;
				}
			}
		}
			break;
		case SwitchBullet::TWICE:
			// 画面外なら
			if (m_Pos.x - m_pCamera->pos.x <= -500.0f || m_Pos.x - m_pCamera->pos.x >= SCREEN_WIDTH + 500.0f)
			{
				m_State = DEAD;
			}
			break;
		default:
			break;
		}
		if (m_WaitFrame >= 5)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 6)
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
	case SwitchBullet::SWITCH:
		break;
	case SwitchBullet::DEAD:
		m_IsActive = false;
		break;
	default:
		break;
	}
}

void SwitchBullet::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

SwitchBullet::~SwitchBullet()
{
}

void SwitchBullet::MoveBullet()
{
}

void SwitchBullet::Switch(D3DXVECTOR2 pos)
{
	m_SwitchPos = pos;
	m_IsSwitch = true;
	m_State = SWITCH;
	m_AnimationPtn = 0;
}

void SwitchBullet::Died()
{
	m_State = DEAD;
}
