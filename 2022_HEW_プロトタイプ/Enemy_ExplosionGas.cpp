#include "Enemy_ExplosionGas.h"
#include "texture.h"
#include "camera.h"
#include "sprite.h"
#include "player.h"
#include "Block.h"
#include "game.h"
#include "ExplosionFactory.h"

Enemy_ExplosionGas::Enemy_ExplosionGas(D3DXVECTOR2 pos, int ID):
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(1.0f, 1.0f))
{
	// 敵のサイズを設定
	m_HP = 1;
	m_BombFrame = 100;
	m_BombRange = 300.0f;
	m_MaxSize = 30.0f;
	m_ChangeSizeValue = m_MaxSize / m_BombFrame;
	m_pExplosionFactory = GetExplosionFactory();
	m_DeadAnimeNum = 3;
	m_DeadAnimeFrame = 10;
}

void Enemy_ExplosionGas::Init()
{
	// 敵のテクスチャを読み込み
	m_EnemyTextureNo = LoadTexture((char*)"data/TEXTURE/ExplosionGas.png");
}

void Enemy_ExplosionGas::Uninit()
{
}

void Enemy_ExplosionGas::Update()
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
	case Enemy_ExplosionGas::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら爆発
		if (len < m_BombRange)
		{
			m_State = Enemy_ExplosionGas::SETUP_BOMB;
		}
		break;
	}
	case Enemy_ExplosionGas::SETUP_BOMB:
		SetUp();
		break;
	case Enemy_ExplosionGas::BOMB:
		// 自壊
		// 爆発を作成
		m_pExplosionFactory->Create(m_Pos, D3DXVECTOR2(200.0f, 200.0f));
		m_IsDead = true;
		m_IsDie = true;
		// m_State = Enemy_SelfDestruct::IDLE;
		break;
	case Enemy_ExplosionGas::DEAD:
		m_IsActive = false;
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
	if (m_WaitAnimeFrame >= 10)
	{
		m_WaitAnimeFrame = 0;
		m_AnimationPtn++;
		if (m_AnimationPtn == 2) {
			m_AnimationPtn = 0;
		}
	}
	else
	{
		m_WaitAnimeFrame++;
	}
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);

}

void Enemy_ExplosionGas::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

Enemy_ExplosionGas::~Enemy_ExplosionGas()
{
}

void Enemy_ExplosionGas::SetUp()
{
	// 爆発するかどうかをチェック
	if (m_WaitFrame >= m_BombFrame)
	{
		// 爆発へ
		m_State = Enemy_ExplosionGas::BOMB;
		// サイズを戻す
		m_Size = D3DXVECTOR2(120.0f, 120.0f);
		m_WaitFrame = 0;
	}
	else
	{
		// サイズを変える
		m_Size.x += m_ChangeSizeValue;
		m_Size.y += m_ChangeSizeValue;
		m_WaitFrame++;
	}
}
