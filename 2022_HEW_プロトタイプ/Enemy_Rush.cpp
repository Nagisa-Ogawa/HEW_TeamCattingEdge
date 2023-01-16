#include "Enemy_Rush.h"
#include "texture.h"
#include "camera.h"
#include "sprite.h"
#include "player.h"
#include "Block.h"
#include "game.h"

Enemy_Rush::Enemy_Rush(D3DXVECTOR2 pos, int ID, int textureNo):
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(2.0f, 6.0f),textureNo,ENEMY_TYPE::RUSH)
{
	m_ChaseSpeed = 4.0f;
	m_ActiveRange = 300.0f;	// 追いかけ始める範囲
	m_DistanceMax = 500.0f;	// 追いかける距離の最大値
}

void Enemy_Rush::Init()
{
}

void Enemy_Rush::Uninit()
{
}

void Enemy_Rush::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();

	// 死亡していたなら死亡状態へ
	if (m_IsDie) {
		m_IsDie = false;
		m_AnimationPtn = 0;
		m_Muki += 2;
		m_WaitFrame = 0;
		m_State = DEAD;
	}
	switch (m_State)
	{
	case Enemy_Rush::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら追いかける
		if (len < m_ActiveRange)
		{
			if (pVec.x > 0) {
				m_ChaseDirection = D3DXVECTOR2(-1.0f, 0.0f);
			}
			else {
				m_ChaseDirection = D3DXVECTOR2(1.0f, 0.0f);
			}
			m_State = Enemy_Rush::CHASE;
		}
		break;
	}
	case Enemy_Rush::CHASE:
	{
		Chase();
		break;
	}
	case Enemy_Rush::WAIT:
		break;
	case Enemy_Rush::DEAD:
		m_IsActive = false;
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
	//LookPlayer();
	//m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

void Enemy_Rush::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

void Enemy_Rush::AfterHitCheckBlockX(DWORD result)
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

void Enemy_Rush::AfterHitCheckBlockY(DWORD result)
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
	LookPlayer();
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Enemy_Rush::~Enemy_Rush()
{
}

void Enemy_Rush::LookPlayer()
{
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	if (pVec.x > 0)
	{
		if (m_State == IDLE) {
			m_Muki = 0;
		}
		else if (m_State == CHASE) {
			m_Muki = 1;
		}
	}
	else
	{
		if (m_State == IDLE) {
			m_Muki = 3;
		}
		else if (m_State == CHASE) {
			m_Muki = 4;
		}
	}
}

void Enemy_Rush::Chase()
{
	m_Vel.x += m_ChaseDirection.x*m_ChaseSpeed;
	m_Vel.y += m_ChaseDirection.y*m_ChaseSpeed;
	m_NowDistance += abs(m_ChaseDirection.x*m_ChaseSpeed);
	if (m_NowDistance >= m_DistanceMax) {
		m_State = Enemy_Rush::WAIT;
	}
}
