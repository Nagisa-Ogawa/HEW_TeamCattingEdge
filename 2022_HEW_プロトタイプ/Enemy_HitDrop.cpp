#include "Enemy_HitDrop.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"
#include "Block.h"
#include "player.h"

Enemy_HitDrop::Enemy_HitDrop(D3DXVECTOR2 pos,int ID):Enemy(pos,ID)
{
	// 敵のサイズを設定
	m_Size = D3DXVECTOR2(ENEMY_SIZE, ENEMY_SIZE);
	m_Gravity = 4.0f;
	m_DropPower = ENEMY_DROPPOWER;
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
	
	case IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら攻撃準備へ
		if (len < ENEMY_ACTIVERADIUS)
		{
			ChangeSetUp();
		}
		m_Vel.y += m_Gravity;
		LookPlayer();
		break;
	}
	case SETUP:
		// 待機
		if (m_WaitFrame >= ENEMY_WAITFRAME_SETUP)
		{
			m_State = JUMP;
			m_WaitFrame = 0;
			// ジャンプアニメーションにする
			m_AnimationPtn++;
		}
		else
		{
			m_WaitFrame++;
		}
		LookPlayer();
		m_Vel.y += m_Gravity;
		break;
	case JUMP:
		// ジャンプ処理
		Jump();
		break;
	case DROP:
		m_Vel.y += m_DropPower;
		m_DropPower += ENEMY_DROPPOWER;
		if (m_IsGround)
		{
			// 準備アニメーションへ
			// m_AnimationPtn = 0;
			m_DropPower = ENEMY_DROPPOWER;
			m_State = AFTERDROP;
		}
		break;
	case AFTERDROP:
		if (m_WaitFrame >= ENEMY_WAITFRAME_AFTERDROP)
		{
			m_State = WAIT;
			m_WaitFrame = 0;
			// 待機アニメーションにする
			m_AnimationPtn = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		m_Vel.y += m_Gravity;
		break;
	case WAIT:
		if (m_WaitFrame >= ENEMY_WAITFRAME_WAIT)
		{
			m_State = IDLE;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		m_Vel.y += m_Gravity;
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

	result = HitChackEnemy_Block(m_Pos,m_Size,m_Vel);

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


	////アニメーションカウンターをカウントアップして、ウエイト値を超えたら
	//if (m_AnimationCounter > 50)
	//{
	//	//アニメーションパターンを切り替える
	//	m_AnimationPtn++;
	//	//最後のアニメーションパターンを表示したらリセットする
	//	if (m_AnimationPtn >= 4)
	//		m_AnimationPtn = 0;

	//	//アニメーションカウンターのリセット
	//	m_AnimationCounter = 0;
	//}
	//m_AnimationCounter++;
}

void Enemy_HitDrop::Draw()
{
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], PATTERN_WIDTH, PATTERN_HEIGHT);
}

Enemy_HitDrop::~Enemy_HitDrop()
{
}

void Enemy_HitDrop::ChangeSetUp()
{
	m_State = SETUP;
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
	m_Vel.y += m_JumpPower.y;
	// 減衰
	m_JumpPower.y += m_JumpAttenuation.y;
	// 移動中にプレイヤーがいた座標まで来たならそこに落下
	// 敵の向きによって変える
	D3DXVECTOR2 enemyPos = m_Pos += m_Vel;
	if (m_Muki == 0)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = DROP;
			m_AnimationPtn++;
		}
	}
	else
	{
		if (enemyPos.x <= m_DropPosX)
		{
			m_State = DROP;
			m_AnimationPtn++;
		}
	}
	if (m_JumpPower.y >= 0.0f)
	{
		m_State = DROP;
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


