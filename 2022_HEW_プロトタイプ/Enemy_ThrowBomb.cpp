
#include "Enemy_ThrowBomb.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"
#include "Block.h"
#include "sound.h"

static int	g_SE_dead;	// SEの識別子
static int	g_SE_throw;		// SEの識別子

Enemy_ThrowBomb::Enemy_ThrowBomb(D3DXVECTOR2 pos,int ID, int textureNo):
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(3.0f, 4.0f),textureNo,ENEMY_TYPE::THROWBOMB)
{
	m_HP = 1;
	m_WaitTime = 60;
	m_Gravity = 4.0f;
	m_CooldownTime = 60;
	m_ActiveRange = 400.0f;
	m_DeadAnimeNum = 3;
	m_DeadAnimeFrame = 10;
}

void Enemy_ThrowBomb::Init()
{
	m_pBombFactory = GetBombFactory();

	g_SE_throw = LoadSound((char*)"data/SE/Tengu_throw.wav");
	SetVolume(g_SE_throw, 1.0f);
	g_SE_dead = LoadSound((char*)"data/SE/Tengu_dead.wav");
	SetVolume(g_SE_dead, 0.5f);
}

void Enemy_ThrowBomb::Uninit()
{
}

void Enemy_ThrowBomb::Update()
{
	// 死亡していたなら死亡状態へ
	if (m_IsDie) {
		m_IsDie = false;
		m_AnimationPtn = 0;
		m_Muki += 2;
		m_WaitFrame = 0;
		m_State = DEAD;
		PlaySound(g_SE_dead, 0);
	}
	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();
	switch (m_State)
	{
	case Enemy_ThrowBomb::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら爆弾を投げる
		if (len < m_ActiveRange)
		{
			m_AnimationPtn++;
			m_State = STATE_ENEMY_THROWBOMB::THROW;
		}
		break;
	}
	case Enemy_ThrowBomb::THROW:
		Throw();
		break;
	case Enemy_ThrowBomb::WAIT:
		if (m_WaitFrame >= m_WaitTime)
		{
			m_State = STATE_ENEMY_THROWBOMB::COOLDOWN;
			m_AnimationPtn = 0;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Enemy_ThrowBomb::COOLDOWN:
		if (m_WaitFrame >= m_CooldownTime)
		{
			m_State = STATE_ENEMY_THROWBOMB::IDLE;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Enemy_ThrowBomb::DEAD:
		if (m_WaitFrame >= m_DeadAnimeFrame)
		{
			m_AnimationPtn++;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		if (m_AnimationPtn > m_DeadAnimeNum - 1) {
			m_IsActive = false;
			
		}
		break;
	default:
		break;
	}
	
}

void Enemy_ThrowBomb::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

void Enemy_ThrowBomb::AfterHitCheckBlockX(DWORD result)
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

void Enemy_ThrowBomb::AfterHitCheckBlockY(DWORD result)
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

Enemy_ThrowBomb::~Enemy_ThrowBomb()
{
}

void Enemy_ThrowBomb::Throw()
{
	// 爆弾を投げる場所を決定
	PLAYER* pPlayer = GetPlayer();
	// 始点と終点と開始ベクトルと終了ベクトルを決定
	//D3DXVECTOR2 startVec = D3DXVECTOR2(0.0f, -500.0f);
	//D3DXVECTOR2 endVec = D3DXVECTOR2( 0.0f,  500.0f);
	//D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	//if (pVec.x < 0)
	//{
	//	startVec.x = -5.0f;
	//	endVec.x =   -5.0f;
	//}
	//else
	//{
	//	startVec.x = 5.0f;
	//	endVec.x = 5.0f;
	//}
	// 爆弾を生成
	m_pBombFactory->CreateContactBomb(m_Pos,pPlayer->pos);
	//SEを入力
	PlaySound(g_SE_throw, 0);
	m_State = STATE_ENEMY_THROWBOMB::WAIT;
}

void  Enemy_ThrowBomb::LookPlayer()
{
	if (m_State == DEAD) {
		return;
	}
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	if (pVec.x > 0)
	{
		m_Muki = 0;
	}
	else
	{
		m_Muki = 1;
	}
}
