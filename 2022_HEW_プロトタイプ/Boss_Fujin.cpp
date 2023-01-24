#include "Boss_Fujin.h"
#include "player.h"
#include "camera.h"
#include "sprite.h"
#include "Block.h"
#include "texture.h"
#include "WindBladeFactory.h"
#include "FlashFactory.h"
#include "game.h"
#include "EnemyFactory.h"
#include "RayFactory.h"
#include "Enemy_BossAvator.h"

Boss_Fujin::Boss_Fujin(D3DXVECTOR2 pos, int ID, int textureNo)
	: Enemy(pos,ID,D3DXVECTOR2(480.0f,480.0f), D3DXVECTOR2(6.0f, 12.0f), textureNo, Enemy::ENEMY_TYPE::BOSS_FUJIN)
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = 0;
	m_pPlayer = GetPlayer();
	m_pRayFactory = GetRayFactory();
	m_pWindBladeFactory = GetWindBladeFactory();
	m_pFlashFactory = GetFlashFactory();
	m_pEnemyFactory = GetEnemyFactory();
	m_AttackDistance = 300.0f;
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
			m_StateCount = 1;
			switch (m_StateCount)
			{
			case 0:
				m_State = WINDBLADE;
				break;
			case 1:
				m_Muki += 8;
				m_State = INHALE;
				break;
			case 2:
				m_Muki += 4;
				m_State = BULLET_X;
				break;
			case 3:
				m_Muki += 2;
				m_State = WINDBLADE;
				break;
			case 4:
				m_State = AVATOR;
				break;
			default:
				break;
			}
			m_StateCount++;
			if (m_StateCount >= 5) {
				m_StateCount = 0;
			}
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
		ShotBullet_X();
		break;
	case Boss_Fujin::WINDBLADE:
		if (m_WaitFrame == 10)
		{
			// 光るオブジェクト作成
			m_pFlashFactory->Create(D3DXVECTOR2(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 200.0f), D3DXVECTOR2(180.0f, 180.0f));
		}
		if (m_WaitFrame == 90)
		{
			// 風の刃作成
			m_pWindBladeFactory->Create(D3DXVECTOR2(SCREEN_WIDTH + 60.0f, SCREEN_HEIGHT - 200.0f),D3DXVECTOR2(180.0f,360.0f), 1);
		}
		// 一定時間待機
		if (m_WaitFrame >= 120)
		{
			m_WaitFrame = 0;
			m_Muki = 0;
			m_State = WAIT;
		}
		else
		{
			m_WaitFrame++;
		}

		break;
	case Boss_Fujin::AVATOR:
		Avator();
		break;
	case Boss_Fujin::MOVE:
		Move();
		break;
	case Boss_Fujin::WAIT:
		// 一定時間待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_State = Boss_Fujin::IDLE;
		}
		else
		{
			m_WaitFrame++;
		}
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
		m_Muki -= 8;
		m_AnimationPtn = 0;
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
		m_Muki -= 8;
		m_AnimationPtn = 0;
		m_State = ATTACK;
	}
	else
	{
		m_WaitFrame++;
	}
	if (m_AnimeFrame >= 10) {
		m_AnimeFrame = 0;
		m_AnimationPtn++;
		if (m_AnimationPtn >= 5) {
			m_AnimationPtn = 2;
		}
	}
	else {
		m_AnimeFrame++;
	}
}

void Boss_Fujin::ShotBullet_X()
{
	switch (m_MoveCount)
	{
	case 0:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_AnimationPtn += 2;
			m_BeforeShotPos = m_Pos;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 1:
		// X弾発射
		m_pRayFactory->CreateXRay(m_Pos, m_pPlayer->pos);
		m_MoveCount++;
		break;
	case 2:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 3:
	{
		// 上空へ移動
		D3DXVECTOR2 pos = D3DXVECTOR2(1300.0f, 300.0f);
		SetMove(m_Pos, pos, m_State, m_Muki);
		break;
	}
	case 4:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_AnimationPtn += 2;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 5:
		// X弾発射
		m_pRayFactory->CreateXRay(m_Pos, m_pPlayer->pos);
		m_MoveCount++;
	case 6:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 7:
		// 地面へ移動
		m_Muki = 0;
		SetMove(m_Pos, m_BeforeShotPos, m_State, m_Muki);
		break;
	case 8:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_MoveCount = 0;
			m_State = IDLE;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	default:
		break;
	}
}

void Boss_Fujin::Avator()
{
	switch (m_MoveCount)
	{
	case 0:
		m_BeforeShotPos = m_Pos;
		// マップ外へ移動
		SetMove(m_Pos, D3DXVECTOR2(SCREEN_WIDTH + m_Size.x / 2.0f, SCREEN_HEIGHT - m_Size.y / 2.0f - BLOCK_SIZE),m_State,m_Muki);
		break;
	case 1:
		// 分身を三体作成
		for (int i = 0; i < 3; i++)
		{
			m_pEnemyFactory->Create_BossAvator(m_Pos, D3DXVECTOR2(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - (i*300.0f) - 250.0f),Enemy_BossAvator::AVATOR_MODE::FUJIN);
		}
		m_MoveCount++;
	case 2:
		// 分身が消えるまで待機
		if (m_pEnemyFactory->CheckAliveFujinAvator()) {
			m_MoveCount++;
		}
		break;
	case 3:
		// マップ内へ移動
		SetMove(m_Pos, m_BeforeShotPos, m_State, m_Muki);
		break;
	case 4:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_MoveCount = 0;
			m_State = IDLE;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	default:
		break;
	}
}

void Boss_Fujin::SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJIN state, int muki)
{
	m_BeforeState = m_State;
	m_BeforeMuki = muki;
	m_State = Boss_Fujin::MOVE;
	m_StartPos = startPos;
	m_EndPos = endPos;
	m_NowDistance = 0.0f;
	D3DXVECTOR2 vec = endPos - startPos;
	D3DXVec2Normalize(&m_MoveVec, &vec);
	m_MoveVec.x *= 12.0f;
	m_MoveVec.y *= 12.0f;
	m_MoveDistance = D3DXVec2Length(&vec);
	// 移動方向からアニメーションの向きを決定
	if (vec.x > 0)
	{
		m_Muki = 7;
	}
	else
	{
		m_Muki = 6;
	}
}

void Boss_Fujin::Move()
{
	m_Vel += m_MoveVec;
	D3DXVECTOR2 vec = (m_Pos + m_Vel) - m_StartPos;
	float distance = D3DXVec2Length(&vec);
	if (distance >= m_MoveDistance)
	{
		m_Muki = m_BeforeMuki;
		m_State = m_BeforeState;
		m_Pos = m_EndPos;
		m_MoveCount++;
		m_AnimationPtn = 0;
	}
	if (m_WaitFrame >= 10)
	{
		m_AnimationPtn++;
		if (m_AnimationPtn >= 4)
		{
			m_AnimationPtn = 0;
		}
		m_WaitFrame = 0;
	}
	{
		m_WaitFrame++;
	}
}

void Boss_Fujin::AfterHitCheckBlockX(DWORD result)
{
	if (m_State != MOVE)
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
		if (m_State != BULLET_X&&m_State!=AVATOR) {
			m_Vel.y += m_Gravity;
		}
	}

	m_Pos.x += m_Vel.x;

	result = 0;
}

void Boss_Fujin::AfterHitCheckBlockY(DWORD result)
{
	if (m_State != MOVE)
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
	}

	m_Pos.y += m_Vel.y;
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Boss_Fujin::~Boss_Fujin()
{
}
