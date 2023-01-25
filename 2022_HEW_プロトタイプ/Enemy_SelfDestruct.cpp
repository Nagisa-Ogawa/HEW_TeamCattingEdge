#include "Enemy_SelfDestruct.h"
#include "texture.h"
#include "camera.h"
#include "sprite.h"
#include "player.h"
#include "Block.h"
#include "game.h"
#include "ExplosionFactory.h"
#include "sound.h"

static int	g_SE_bomb;		// SEの識別子

Enemy_SelfDestruct::Enemy_SelfDestruct(D3DXVECTOR2 pos, int ID, int textureNo):
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(2.0f, 6.0f),textureNo,ENEMY_TYPE::SELFDESTRUCT)
{
	// 敵のサイズを設定
	m_HP = 1;
	m_Gravity = 4.0f;
	m_ActiveRange = 400.0f;
	m_ChaseSpeed = 3.0f;
	m_BombFrame = 100;
	m_BombRange = 200.0f;
	m_MaxSize = 30.0f;
	m_ChangeSizeValue = m_MaxSize / m_BombFrame;
	m_pExplosionFactory = GetExplosionFactory();
	m_DeadAnimeNum = 3;
	m_DeadAnimeFrame = 10;

	//音関連の初期化
	g_SE_bomb = LoadSound((char*)"data/SE/Tengu_bomb.wav");
	SetVolume(g_SE_bomb, 0.3f);
}

void Enemy_SelfDestruct::Init()
{
}

void Enemy_SelfDestruct::Uninit()
{
}

void Enemy_SelfDestruct::Update()
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
	case Enemy_SelfDestruct::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら追いかける
		if (len < m_ActiveRange)
		{
			m_State = STATE_ENEMY_SELFDESTRUCT::CHASE;
		}
		break;
	}
	case Enemy_SelfDestruct::CHASE:
	{
		Chase();
		// 爆発範囲に入ったら爆発準備へ
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら爆発準備へ
		if (len < m_BombRange)
		{
			m_State = Enemy_SelfDestruct::SETUP_BOMB;
		}
		break;
	}
	case Enemy_SelfDestruct::SETUP_BOMB:
		SetUp();
		break;
	case Enemy_SelfDestruct::BOMB:
		// 自壊
		// 爆発を作成
		m_pExplosionFactory->Create(m_Pos, D3DXVECTOR2(200.0f, 200.0f));
		PlaySound(g_SE_bomb, 0);
		m_IsDead = true;
		m_IsDie = true;
		// m_State = Enemy_SelfDestruct::IDLE;
		break;
	case Enemy_SelfDestruct::DEAD:
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

	//result = HitChackEnemy_Block(m_Pos,m_Size,m_Vel);

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
	//if (m_WaitAnimeFrame >= 10)
	//{
	//	m_WaitAnimeFrame = 0;
	//	m_AnimationPtn++;
	//	if (m_AnimationPtn == 2) {
	//		m_AnimationPtn = 0;
	//	}
	//}
	//else
	//{
	//	m_WaitAnimeFrame++;
	//}
	//LookPlayer();
	//m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

void Enemy_SelfDestruct::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

void Enemy_SelfDestruct::AfterHitCheckBlockX(DWORD result)
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

void Enemy_SelfDestruct::AfterHitCheckBlockY(DWORD result)
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
	LookPlayer();
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Enemy_SelfDestruct::~Enemy_SelfDestruct()
{
}

void Enemy_SelfDestruct::SetUp()
{
	// 爆発するかどうかをチェック
	if (m_WaitFrame >= m_BombFrame)
	{
		// 爆発へ
		m_State = Enemy_SelfDestruct::BOMB;
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

// プレイヤーを追いかける関数
void Enemy_SelfDestruct::Chase()
{
	// プレイヤーを追いかける
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	if (pVec.x > 0)
	{
		pVec.x = -1.0f;
	}
	else
	{
		pVec.x = 1.0f;
	}
	// 移動
	m_Vel.x += m_ChaseSpeed * pVec.x;
}

void  Enemy_SelfDestruct::LookPlayer()
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
		else if (m_State == SETUP_BOMB) {
			m_Muki = 2;
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
		else if (m_State == SETUP_BOMB) {
			m_Muki = 5;
		}	
	}
}

