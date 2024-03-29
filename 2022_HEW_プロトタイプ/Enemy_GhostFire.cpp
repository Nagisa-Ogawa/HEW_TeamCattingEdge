#include "Enemy_GhostFire.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"
#include "Block.h"
#include "FireBall.h"
#include "sound.h"

static int	g_SE_fire;		// SEの識別子

Enemy_GhostFire::Enemy_GhostFire(D3DXVECTOR2 pos, int muki,int ID, int textureNo):
	Enemy(pos, ID, D3DXVECTOR2(240.0f, 240.0f), D3DXVECTOR2(13.0f, 6.0f),textureNo,ENEMY_TYPE::GHOSTFIRE)
{
	m_Muki = muki;
	m_WaitTime = 30;
	m_WaitIdleTime = 30;
	m_WaitNextThrowTime = 10;
	int m_CooldownTime = 30;
	m_CollisionSize = D3DXVECTOR2(40.0f, 60.0f);

	//音関連の初期化
	g_SE_fire = LoadSound((char*)"data/SE/Kasya_fire.wav");
	SetVolume(g_SE_fire, 0.5f);
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
	// 死亡していたなら死亡状態へ
	if (m_IsDie) {
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
	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();
	switch (m_State)
	{
	case Enemy_GhostFire::IDLE:
	{
		if (m_WaitFrame >= 120) {
			m_State = Enemy_GhostFire::THROW;
			m_Muki += 4;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		// 規定時間経過していたなら火の玉を投げる
		if (m_AnimeFrame >= 6)
		{
			m_AnimeFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 5) {
				m_AnimationPtn = 0;
			}
		}
		else
		{
			m_AnimeFrame++;
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
		if (m_WaitFrame >= 5)
		{
			m_AnimationPtn++;
			m_WaitFrame = 0;
			if (m_AnimationPtn >= 8)
			{
				m_IsActive = false;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	default:
		break;
	}
	//result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
	////当たり判定処理
	//if (result & HIT_LEFT)
	//{
	//	if (m_Vel.x > 0.0)
	//		m_Vel.x = 0.0f;
	//}
	//if (result & HIT_RIGHT)
	//{
	//	if (m_Vel.x < 0.0)
	//		m_Vel.x = 0.0f;
	//}
	//m_Vel.y += m_Gravity;

	//result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);

	////落下させるか？処理
	//if ((result & HIT_UP) == 0 && m_IsGround == true)
	//{
	//	m_IsGround = false;
	//}

	////落下処理
	//if (m_IsGround == false)
	//{
	//	if (result & HIT_UP)
	//	{
	//		m_IsGround = true;
	//		m_Pos.y = GetBlockHeight() - (m_Size.y / 2);
	//		m_Vel.y = 0.0f;
	//	}
	//}
	//else // 最終的に地面に触れている
	//{
	//	m_Vel.y = 0.0f;
	//}

	//m_Pos += m_Vel;

	//m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

void Enemy_GhostFire::Draw()
{
	if (m_IsActive)
	{
		if (m_Muki % 2 == 0)
		{
			D3DXVECTOR2 basePos = GetBase();
			DrawSpriteColor(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		}
		else
		{
			D3DXVECTOR2 basePos = GetBase();
			DrawSpriteColor(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));

		}
	}
}

void Enemy_GhostFire::AfterHitCheckBlockX(DWORD result)
{
	//当たり判定処理
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

void Enemy_GhostFire::AfterHitCheckBlockY(DWORD result)
{
	//落下させるか？処理
	if ((result & HIT_UP) == 0 && m_IsGround == true)
	{
		m_IsGround = false;
	}

	//落下処理
	if (m_IsGround == false)
	{
		if (result & HIT_UP)
		{
			m_IsGround = true;
			m_Pos.y = GetBlockHeight() - (m_Size.y / 2);
			m_Vel.y = 0.0f;
		}
	}
	else // 最終的に地面に触れている
	{
		m_Vel.y = 0.0f;
	}

	m_Pos += m_Vel;

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Enemy_GhostFire::~Enemy_GhostFire()
{
}

void Enemy_GhostFire::Throw()
{
	// アニメーション
	if (m_WaitFrame >= 6)
	{
		m_AnimationPtn++;
		m_WaitFrame = 0;
		if (m_AnimationPtn == 6 || m_AnimationPtn == 9) {
			D3DXVECTOR2 pos = m_Pos;
			pos.y += m_Size.y / 7.5f;
			if (m_Muki % 2 == 0)
			{
				m_pFireBallFactory->Create(pos, 0, D3DXVECTOR2(10.0f, 0.0f), FireBall::MODE::GHOSTFIRE);
			}
			else
			{
				m_pFireBallFactory->Create(pos, 1, D3DXVECTOR2(10.0f, 0.0f), FireBall::MODE::GHOSTFIRE);
			}
			PlaySound(g_SE_fire, 0);
		}
		if (m_AnimationPtn >= 13) {
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_Muki -= 4;
			m_State = Enemy_GhostFire::IDLE;
		}
	}
	else
	{
		m_WaitFrame++;
	}

}
