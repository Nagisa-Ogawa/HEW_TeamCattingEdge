#include "Enemy_GhostFire.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"
#include "Block.h"
#include "FireBall.h"

Enemy_GhostFire::Enemy_GhostFire(D3DXVECTOR2 pos, int muki,int ID, int textureNo):
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(1.0f, 2.0f),textureNo)
{
	m_WaitTime = 30;
	m_WaitIdleTime = 30;
	m_WaitNextThrowTime = 10;
	int m_CooldownTime = 30;
}

void Enemy_GhostFire::Init()
{
	m_pFireBallFactory = GetFireBallFactory();
}

void Enemy_GhostFire::Uninit()
{
}

void Enemy_GhostFire::Update()
{
	// Ž€–S‚µ‚Ä‚¢‚½‚È‚çŽ€–Só‘Ô‚Ö
	if (m_IsDie) {
		m_IsDie = false;
		m_AnimationPtn = 0;
		m_Muki += 2;
		m_WaitFrame = 0;
		m_State = DEAD;
	}
	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();
	switch (m_State)
	{
	case Enemy_GhostFire::IDLE:
	{
		// ‹K’èŽžŠÔŒo‰ß‚µ‚Ä‚¢‚½‚È‚ç‰Î‚Ì‹Ê‚ð“Š‚°‚é
		if (m_WaitFrame >= m_WaitIdleTime)
		{
			m_State = Enemy_GhostFire::THROW;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	}
	case Enemy_GhostFire::THROW:
		Throw();
		break;
	case Enemy_GhostFire::WAIT:
		if (m_WaitFrame >= m_WaitTime)
		{
			m_State = Enemy_GhostFire::IDLE;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Enemy_GhostFire::DEAD:
		//if (m_WaitFrame >= m_DeadAnimeFrame)
		//{
		//	m_AnimationPtn++;
		//	m_WaitFrame = 0;
		//}
		//else
		//{
		//	m_WaitFrame++;
		//}
		//if (m_AnimationPtn > m_DeadAnimeNum - 1) {
			m_IsActive = false;
		//}
		break;
	default:
		break;
	}
	result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
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

	result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);

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

	m_Pos += m_Vel;

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

void Enemy_GhostFire::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			0.0f, 0.0f, 1.0f, 1.0f);// m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

Enemy_GhostFire::~Enemy_GhostFire()
{
}

void Enemy_GhostFire::Throw()
{
	if (m_IsThrow) {
		m_pFireBallFactory->Create(m_Pos, m_Muki, D3DXVECTOR2(10.0f, 0.0f),FireBall::MODE::GHOSTFIRE);
		m_ThrowNum++;
		m_IsThrow = false;
	}
	else
	{
		m_WaitFrame++;
		if (m_WaitFrame >= m_WaitNextThrowTime) {
			m_IsThrow = true;
			m_WaitFrame = 0;
		}
	}
	if (m_ThrowNum >= 2) {
		m_State = Enemy_GhostFire::WAIT;
		m_IsThrow = true;
		m_ThrowNum = 0;
	}
}
