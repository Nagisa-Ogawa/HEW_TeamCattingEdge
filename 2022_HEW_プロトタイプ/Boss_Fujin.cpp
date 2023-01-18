#include "Boss_Fujin.h"
#include "player.h"
#include "camera.h"
#include "sprite.h"
#include "Block.h"
#include "texture.h"

Boss_Fujin::Boss_Fujin(D3DXVECTOR2 pos, int ID, int textureNo)
	: Enemy(pos,ID,D3DXVECTOR2(480.0f,480.0f), D3DXVECTOR2(6.0f, 10.0f), textureNo, Enemy::ENEMY_TYPE::BOSS_FUJIN)
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = 0;
	m_pPlayer = GetPlayer();
	// 攻撃用変数
	m_AttackTextureNo=LoadTexture((char*)"data/TEXTURE/fade_white.png");
	m_AttackCollisionSize = D3DXVECTOR2(200.0f, 100.0f);
	// 吸い込み攻撃用変数
	m_InHalePower = D3DXVECTOR2(5.0f, 5.0f);
}

void Boss_Fujin::Init()
{
}

void Boss_Fujin::Uninit()
{
}

void Boss_Fujin::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	m_OldPos = m_Pos;
	DWORD result = 0;
	// 死亡していたなら死亡状態へ
	if (m_IsDie)
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
	switch (m_State)
	{
	case Boss_Fujin::IDLE:
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_Muki += 4;
			m_State = INHALE;
			// m_StateCount = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Boss_Fujin::ATTACK:
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_State = IDLE;
			// m_StateCount = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Boss_Fujin::INHALE:
		InHale();
		break;
	case Boss_Fujin::BULLET_X:
		break;
	case Boss_Fujin::WINDBLADE:
		break;
	case Boss_Fujin::AVATOR:
		break;
	case Boss_Fujin::WIT:
		break;
	case Boss_Fujin::DEAD:
		break;
	default:
		break;
	}
}

void Boss_Fujin::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	if (m_State == Boss_Fujin::ATTACK)
	{
		if (m_Muki % 2 == 0)
		{
			DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x-300.0f, basePos.y + m_Pos.y, m_AttackCollisionSize.x, m_AttackCollisionSize.y,
				0.0f, 0.0f, 1.0f, 1.0f);
		}
		else
		{
			DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x+300.0f, basePos.y + m_Pos.y, m_AttackCollisionSize.x, m_AttackCollisionSize.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
		}
	}
}

void Boss_Fujin::InHale()
{
	// プレイヤーとの距離を計算
	D3DXVECTOR2 vec = m_Pos - m_pPlayer->pos;
	float distance = D3DXVec2Length(&vec);
	// プレイヤーと一定値以上近づいたら終了
	if (distance <= m_AttackDistance)
	{
		m_WaitFrame = 0;
		m_Muki = 0;
		m_State = ATTACK;
	}
	// ベクトルを正規化
	D3DXVec2Normalize(&vec, &vec);
	vec.x *= m_InHalePower.x;
	vec.y *= m_InHalePower.y;
	// プレイヤーを吸い込む
	m_pPlayer->vel += vec;
	// 規定フレーム経過していたなら終了
	if (m_WaitFrame >= 600)
	{
		m_WaitFrame = 0;
		m_Muki = 0;
		m_State = ATTACK;
	}
	else
	{
		m_WaitFrame++;
	}
}

void Boss_Fujin::AfterHitCheckBlockX(DWORD result)
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

	m_Pos.x += m_Vel.x;

	result = 0;
	m_Vel.y += m_Gravity;
}

void Boss_Fujin::AfterHitCheckBlockY(DWORD result)
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

	m_Pos.y += m_Vel.y;
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Boss_Fujin::~Boss_Fujin()
{
}
