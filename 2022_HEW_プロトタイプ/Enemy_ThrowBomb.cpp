
#include "Enemy_ThrowBomb.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"
#include "Block.h"

Enemy_ThrowBomb::Enemy_ThrowBomb(D3DXVECTOR2 pos,int ID):
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(2.0f, 2.0f))
{
	m_HP = 1;
	m_WaitTime = 60;
	m_Gravity = 4.0f;
	m_CooldownTime = 60;
	m_ActiveRange = 400.0f;

}

void Enemy_ThrowBomb::Init()
{
	// 敵のテクスチャを読み込み
	m_EnemyTextureNo = LoadTexture((char*)"data/TEXTURE/Enemy_ThrowBomb.png");
	m_pBombFactory = GetBombFactory();
}

void Enemy_ThrowBomb::Uninit()
{
}

void Enemy_ThrowBomb::Update()
{
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
	default:
		break;
	}
	result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
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

	result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);

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

void Enemy_ThrowBomb::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

Enemy_ThrowBomb::~Enemy_ThrowBomb()
{
}

void Enemy_ThrowBomb::Throw()
{
	// 爆弾を投げる場所を決定
	PLAYER* pPlayer = GetPlayer();
	// 始点と終点と開始ベクトルと終了ベクトルを決定
	D3DXVECTOR2 startVec = D3DXVECTOR2(0.0f, -500.0f);
	D3DXVECTOR2 endVec = D3DXVECTOR2( 0.0f,  500.0f);
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	if (pVec.x < 0)
	{
		startVec.x = -5.0f;
		endVec.x =   -5.0f;
	}
	else
	{
		startVec.x = 5.0f;
		endVec.x = 5.0f;
	}
	// 爆弾を生成
	m_pBombFactory->CreateContactBomb(m_Pos,pPlayer->pos,startVec,endVec);
	m_State = STATE_ENEMY_THROWBOMB::WAIT;
}

void  Enemy_ThrowBomb::LookPlayer()
{
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	if (pVec.x > 0)
	{
		m_Muki = 1;
	}
	else
	{
		m_Muki = 0;
	}
}
