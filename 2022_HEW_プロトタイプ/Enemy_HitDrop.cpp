#include "Enemy_HitDrop.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"
#include "Block.h"
#include "player.h"

Enemy_HitDrop::Enemy_HitDrop(D3DXVECTOR2 pos, int ID) :
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(4.0f, 2.0f))
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_DropPower = ENEMY_DROPPOWER;
	m_HP = 1;
}

void Enemy_HitDrop::Init()
{
	// 敵のテクスチャを読み込み
	m_EnemyTextureNo = LoadTexture((char*)"data/TEXTURE/Enemy_HitDrop.png");
}

void Enemy_HitDrop::Uninit()
{
}

void Enemy_HitDrop::Update()
{
	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();

	// ブロックと敵の当たり判定
	switch (m_State)
	{
	
	case Enemy_HitDrop::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら攻撃準備へ
		if (len < ENEMY_ACTIVERADIUS)
		{
			ChangeSetUp();
		}
		break;
	}
	case Enemy_HitDrop::SETUP:
		// 待機
		if (m_WaitFrame >= ENEMY_WAITFRAME_SETUP)
		{
			m_State = Enemy_HitDrop::JUMP;
			m_WaitFrame = 0;
			// ジャンプアニメーションにする
			m_AnimationPtn++;
		}
		else
		{
			m_WaitFrame++;
		}
		LookPlayer();
		break;
	case Enemy_HitDrop::JUMP:
		// ジャンプ処理
		Jump();
		break;
	case Enemy_HitDrop::DROP:
		m_DropPower += ENEMY_DROPPOWER;
		if (m_IsGround)
		{
			// 準備アニメーションへ
			// m_AnimationPtn = 0;
			m_DropPower = ENEMY_DROPPOWER;
			m_State = Enemy_HitDrop::AFTERDROP;
		}
		break;
	case Enemy_HitDrop::AFTERDROP:
		if (m_WaitFrame >= ENEMY_WAITFRAME_AFTERDROP)
		{
			m_State = Enemy_HitDrop::WAIT;
			m_WaitFrame = 0;
			// 待機アニメーションにする
			m_AnimationPtn = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Enemy_HitDrop::WAIT:
		if (m_WaitFrame >= ENEMY_WAITFRAME_WAIT)
		{
			m_State = Enemy_HitDrop::IDLE;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		LookPlayer();
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

	m_Pos.x += m_Vel.x;

	if (m_State == Enemy_HitDrop::JUMP)
	{
		m_Vel.y += m_JumpPower.y;
	}
	else if (m_State == Enemy_HitDrop::DROP)
	{
		m_Vel.y += m_DropPower;
	}
	else
	{
		m_Vel.y += m_Gravity;
	}

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

	m_Pos.y += m_Vel.y;
	LookPlayer();

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);


}

void Enemy_HitDrop::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

Enemy_HitDrop::~Enemy_HitDrop()
{
}

void Enemy_HitDrop::ChangeSetUp()
{
	m_State = Enemy_HitDrop::SETUP;
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	// プレイヤーと敵の位置から飛ぶ方向と落下予定座標を決定
	if (pVec.x > 0)
	{
		m_JumpPower = D3DXVECTOR2(-ENEMY_JUMPPOWER_X, ENEMY_JUMPPOWER_Y);
		m_JumpAttenuation = D3DXVECTOR2(-ENEMY_JUMPATTENUATION_X, ENEMY_JUMPATTENUATION_Y);
	}
	else
	{
		m_JumpPower = D3DXVECTOR2(ENEMY_JUMPPOWER_X, ENEMY_JUMPPOWER_Y);
		m_JumpAttenuation = D3DXVECTOR2(ENEMY_JUMPATTENUATION_X, ENEMY_JUMPATTENUATION_Y);
	}
	m_DropPosX = pPlayer->pos.x;
	// 準備アニメーションにする
	m_AnimationPtn++;
}

void Enemy_HitDrop::Jump()
{
	// ジャンプ
	m_Vel.x += m_JumpPower.x;
	// 減衰
	m_JumpPower.y += m_JumpAttenuation.y;
	// 移動中にプレイヤーがいた座標まで来たならそこに落下
	// 敵の向きによって変える
	D3DXVECTOR2 enemyPos = m_Pos + m_Vel;
	if (m_Muki == 0)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = Enemy_HitDrop::DROP;
			m_AnimationPtn++;
		}
	}
	else
	{
		if (enemyPos.x <= m_DropPosX)
		{
			m_State = Enemy_HitDrop::DROP;
			m_AnimationPtn++;
		}
	}
	if (m_JumpPower.y >= 0.0f)
	{
		m_State = Enemy_HitDrop::DROP;
		m_AnimationPtn++;
	}
}

void Enemy_HitDrop::LookPlayer()
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


