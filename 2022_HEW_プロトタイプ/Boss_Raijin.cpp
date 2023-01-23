#include "Boss_Raijin.h"
#include "player.h"
#include "camera.h"
#include "sprite.h"
#include "Block.h"
#include "texture.h"
#include "game.h"
#include "RayFactory.h"

Boss_Raijin::Boss_Raijin(D3DXVECTOR2 pos, int ID, int textureNo)
	: Enemy(pos, ID, D3DXVECTOR2(480.0f, 480.0f), D3DXVECTOR2(6.0f, 10.0f), textureNo, Enemy::ENEMY_TYPE::BOSS_RAIJIN)
{
	// “G‚ÌƒTƒCƒY‚ðÝ’è
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = 0;
	m_State = BULLET_T;
	// UŒ‚—p•Ï”
	m_AttackTextureNo = LoadTexture((char*)"data/TEXTURE/fade_white.png");
	m_AttackCollisionSize = D3DXVECTOR2(200.0f, 100.0f);
	// ‹z‚¢ž‚ÝUŒ‚—p•Ï”
	m_InHalePower = D3DXVECTOR2(5.0f, 5.0f);
}

void Boss_Raijin::Init()
{
	m_pPlayer = GetPlayer();
	m_pRayFactory = GetRayFactory();
}

void Boss_Raijin::Uninit()
{
}

void Boss_Raijin::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	m_OldPos = m_Pos;
	DWORD result = 0;
	// Ž€–S‚µ‚Ä‚¢‚½‚È‚çŽ€–Só‘Ô‚Ö
	/*if (m_IsDie)
	{
		m_IsDie = false;
		m_AnimationPtn = 0;
		if (m_Muki % 2 == 0)
		{
			m_Muki = 2;
		}
		else
		{
			m_Muki = 3;
		}
		m_WaitFrame = 0;
		m_State = DEAD;
	}
*/
	switch (m_State)
	{
	case Boss_Raijin::IDLE:
		m_WaitFrame++;
		if (m_WaitFrame >= 120)
		{
			m_State = BULLET_T;

			m_WaitFrame = 0;
		}

		break;
	case Boss_Raijin::ATTACK:
		break;
	case Boss_Raijin::SWITCH_BULLET:
		break;
	case Boss_Raijin::BULLET_T:
		m_pRayFactory->CreateXRay(m_Pos, m_pPlayer->pos);

		m_State = IDLE;
		break;
	case Boss_Raijin::THUNDERBLADE:
		break;
	case Boss_Raijin::AVATOR:
		break;
	case Boss_Raijin::MOVE:
		break;
	case Boss_Raijin::WAIT:
		break;
	case Boss_Raijin::DEAD:
		break;
	default:
		break;
	}
}

void Boss_Raijin::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	if (m_State == Boss_Raijin::ATTACK)
	{
		if (m_Muki % 2 == 0)
		{
			DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x - 300.0f, basePos.y + m_Pos.y, m_AttackCollisionSize.x, m_AttackCollisionSize.y,
				0.0f, 0.0f, 1.0f, 1.0f);
		}
		else
		{
			DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x + 300.0f, basePos.y + m_Pos.y, m_AttackCollisionSize.x, m_AttackCollisionSize.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
		}
	}
}

void Boss_Raijin::Avator()
{
}

void Boss_Raijin::AfterHitCheckBlockX(DWORD result)
{
	if (m_State != MOVE)
	{
		//“–‚½‚è”»’èˆ—
		if (result & HIT_LEFT)
		{
			if (m_Vel.x > 0.0)
				m_Vel.x = 0.0f;
		}
		if (result & HIT_RIGHT)
		{
			if (m_Vel.x < 0.0)
				m_Vel.x = 0.0f;
		}
		m_Vel.y += m_Gravity;
	}

	m_Pos.x += m_Vel.x;

	result = 0;
}

void Boss_Raijin::AfterHitCheckBlockY(DWORD result)
{
	if (m_State != MOVE)
	{
		//—Ž‰º‚³‚¹‚é‚©Hˆ—
		if ((result & HIT_UP) == 0 && m_IsGround == true)
		{
			m_IsGround = false;
		}

		//—Ž‰ºˆ—
		if (m_IsGround == false)
		{
			if (result & HIT_UP)
			{
				m_IsGround = true;
				m_Pos.y = GetBlockHeight() - (m_Size.y / 2);
				m_Vel.y = 0.0f;
			}
		}
		else // ÅI“I‚É’n–Ê‚ÉG‚ê‚Ä‚¢‚é
		{
			m_Vel.y = 0.0f;
		}
	}

	m_Pos.y += m_Vel.y;
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Boss_Raijin::~Boss_Raijin()
{
}
