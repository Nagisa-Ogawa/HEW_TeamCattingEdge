#include "Enemy_FujinAvator.h"
#include "camera.h"
#include "sprite.h"
#include "Block.h"
#include "texture.h"
#include "WindBladeFactory.h"
#include "game.h"
#include "EnemyFactory.h"

Enemy_FujinAvator::Enemy_FujinAvator(D3DXVECTOR2 pos, int ID, int textureNo, D3DXVECTOR2 targetPos)
	: Enemy(pos, ID, D3DXVECTOR2(360.0f, 360.0f), D3DXVECTOR2(6.0f, 10.0f), textureNo, Enemy::ENEMY_TYPE::FUFINAVATOR),m_TargetPos(targetPos)
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = 0;
	m_pWindBladeFactory = GetWindBladeFactory();
	m_pEnemyFactory = GetEnemyFactory();
}

void Enemy_FujinAvator::Init()
{
}

void Enemy_FujinAvator::Uninit()
{
}

void Enemy_FujinAvator::Update()
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
	case Enemy_FujinAvator::IDLE:
		m_State = WINDBLADE;
		break;
	case Enemy_FujinAvator::WINDBLADE:
		WindBlade();
		break;
	case Enemy_FujinAvator::MOVE:
		Move();
		break;
	case Enemy_FujinAvator::DEAD:
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

void Enemy_FujinAvator::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

void Enemy_FujinAvator::WindBlade()
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
			m_pWindBladeFactory->Create(D3DXVECTOR2(m_Pos.x-100.0f, m_Pos.y), 1);
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

void Enemy_FujinAvator::SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki)
{
	m_BeforeState = m_State;
	m_BeforeMuki = muki;
	m_State = Enemy_FujinAvator::MOVE;
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

void Enemy_FujinAvator::Move()
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

void Enemy_FujinAvator::AfterHitCheckBlockX(DWORD result)
{
	m_Pos.x += m_Vel.x;
}

void Enemy_FujinAvator::AfterHitCheckBlockY(DWORD result)
{
	m_Pos.y += m_Vel.y;
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Enemy_FujinAvator::~Enemy_FujinAvator()
{
}
