#include "Enemy_BossAvator.h"
#include "camera.h"
#include "sprite.h"
#include "Block.h"
#include "texture.h"
#include "WindBladeFactory.h"
#include "ThunderBladeFactory.h"
#include "FlashFactory.h"
#include "game.h"
#include "EnemyFactory.h"

Enemy_BossAvator::Enemy_BossAvator(D3DXVECTOR2 pos, int ID, int textureNo, D3DXVECTOR2 targetPos,Enemy_BossAvator::AVATOR_MODE mode)
	: Enemy(pos, ID, D3DXVECTOR2(300.0f, 300.0f), D3DXVECTOR2(6.0f, 10.0f), textureNo, Enemy::ENEMY_TYPE::FUFINAVATOR),m_TargetPos(targetPos),m_mode(mode)
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = 0;
	m_pWindBladeFactory = GetWindBladeFactory();
	m_pThunderBladeFactory = GetThunderBladeFactory();
	m_pFlashFactory = GetFlashFactory();
	m_pEnemyFactory = GetEnemyFactory();
}

void Enemy_BossAvator::Init()
{
}

void Enemy_BossAvator::Uninit()
{
}

void Enemy_BossAvator::Update()
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
	case Enemy_BossAvator::IDLE:
		switch (m_mode)
		{
		case Enemy_BossAvator::FUJIN:
			m_State = WINDBLADE;
			break;
		case Enemy_BossAvator::RAIJIN:
			m_State = THUNDERBLADE;
			break;
		default:
			break;
		}
		break;
	case Enemy_BossAvator::WINDBLADE:
		WindBlade();
		break;
	case Enemy_BossAvator::THUNDERBLADE:
		ThunderBlade();
		break;
	case Enemy_BossAvator::MOVE:
		Move();
		break;
	case Enemy_BossAvator::DEAD:
		if (m_WaitFrame >= 10)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 6)
			{
				m_AnimationPtn = 0;
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
}

void Enemy_BossAvator::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

void Enemy_BossAvator::WindBlade()
{
	switch (m_MoveCount)
	{
	case 0:
		// 決められた場所へ移動
		SetMove(m_Pos, m_TargetPos, m_State, m_Muki);
		break;
	case 1:
		m_IsEndSetUp = true;
		// 他の分身が準備完了しているかチェック
		if (m_pEnemyFactory->CheckFujinAvatorSetEnd())
		{
			m_MoveCount++;
			m_WaitFrame = 0;
		}
	case 2:
		if (m_WaitFrame == 250)
		{
			// 風の刃作成
			m_pWindBladeFactory->Create(D3DXVECTOR2(m_Pos.x-100.0f, m_Pos.y+100.0f),D3DXVECTOR2(120.0f,240.0f), 1);
		}
		// 一定時間待機
		if (m_WaitFrame >= 500)
		{
			m_WaitFrame = 0;
			m_Muki = 0;
			m_IsDie = true;
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

void Enemy_BossAvator::ThunderBlade()
{
	switch (m_MoveCount)
	{
	case 0:
		// 決められた場所へ移動
		SetMove(m_Pos, m_TargetPos, m_State, m_Muki);
		break;
	case 1:
		m_IsEndSetUp = true;
		// 他の分身が準備完了しているかチェック
		if (m_pEnemyFactory->CheckFujinAvatorSetEnd())
		{
			m_MoveCount++;
			m_WaitFrame = 0;
		}
	case 2:
		if (m_WaitFrame == 200) {
			// 光るオブジェクト作成
			m_pFlashFactory->Create(D3DXVECTOR2(m_Pos.x, 200.0f), D3DXVECTOR2(180.0f, 180.0f));
		}
		if (m_WaitFrame == 280)
		{
			// 雷の刃作成
			m_pThunderBladeFactory->Create(D3DXVECTOR2(m_Pos.x, 200.0f + 90.0f + 360.0f), D3DXVECTOR2(90.0f, 720.0f));
		}
		// 一定時間待機
		if (m_WaitFrame >= 500)
		{
			m_WaitFrame = 0;
			m_Muki = 0;
			m_IsDie = true;
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

void Enemy_BossAvator::SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki)
{
	m_BeforeState = m_State;
	m_BeforeMuki = muki;
	m_State = Enemy_BossAvator::MOVE;
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

void Enemy_BossAvator::Move()
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

void Enemy_BossAvator::AfterHitCheckBlockX(DWORD result)
{
	m_Pos.x += m_Vel.x;
}

void Enemy_BossAvator::AfterHitCheckBlockY(DWORD result)
{
	m_Pos.y += m_Vel.y;
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Enemy_BossAvator::~Enemy_BossAvator()
{
}
