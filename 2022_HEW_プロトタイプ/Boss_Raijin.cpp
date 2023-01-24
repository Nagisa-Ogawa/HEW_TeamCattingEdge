#include "Boss_Raijin.h"
#include "player.h"
#include "camera.h"
#include "sprite.h"
#include "Block.h"
#include "texture.h"
#include "game.h"
#include "RayFactory.h"
#include "FlashFactory.h"
#include "ThunderBladeFactory.h"
#include "EnemyFactory.h"
#include "Enemy_BossAvator.h"

Boss_Raijin::Boss_Raijin(D3DXVECTOR2 pos, int ID, int textureNo)
	: Enemy(pos, ID, D3DXVECTOR2(480.0f, 480.0f), D3DXVECTOR2(6.0f, 10.0f), textureNo, Enemy::ENEMY_TYPE::BOSS_RAIJIN)
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = 0;
	// 攻撃用変数
	m_AttackTextureNo = LoadTexture((char*)"data/TEXTURE/fade_white.png");
	m_AttackCollisionSize = D3DXVECTOR2(200.0f, 100.0f);
}

void Boss_Raijin::Init()
{
	m_pPlayer = GetPlayer();
	m_pRayFactory = GetRayFactory();
	m_pFlashFactory = GetFlashFactory();
	m_pThunderFactory = GetThunderBladeFactory();
	m_pEnemyFactory = GetEnemyFactory();
}

void Boss_Raijin::Uninit()
{
}

void Boss_Raijin::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	m_OldPos = m_Pos;
	DWORD result = 0;
	// 死亡していたなら死亡状態へ
	/*if (m_IsDie)
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
*/
	switch (m_State)
	{
	case Boss_Raijin::IDLE:
		m_WaitFrame++;
		if (m_WaitFrame >= 120)
		{
			m_Muki += 2;
			m_State = AVATOR;
			m_WaitFrame = 0;
		}

		break;
	case Boss_Raijin::ATTACK:
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
	case Boss_Raijin::SWITCH_BULLET:
		break;
	case Boss_Raijin::BULLET_T:
		m_pRayFactory->CreateXRay(m_Pos, m_pPlayer->pos);

		m_State = IDLE;
		break;
	case Boss_Raijin::THUNDERBLADE:
		ThunderBlade();
		break;
	case Boss_Raijin::AVATOR:
		Avator();
		break;
	case Boss_Raijin::MOVE:
		Move();
		break;
	case Boss_Raijin::WAIT:
		break;
	case Boss_Raijin::DEAD:
		break;
	default:
		break;
	}
}

void Boss_Raijin::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	if (m_State == Boss_Raijin::ATTACK)
	{
		if (m_Muki % 2 == 0)
		{
			DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x - 300.0f, basePos.y + m_Pos.y, m_AttackCollisionSize.x, m_AttackCollisionSize.y,
				0.0f, 0.0f, 1.0f, 1.0f);
		}
		else
		{
			DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x + 300.0f, basePos.y + m_Pos.y, m_AttackCollisionSize.x, m_AttackCollisionSize.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
		}
	}
}

void Boss_Raijin::ThunderBlade()
{
	if (m_WaitFrame == 10)
	{
		m_ThunderBoltPos = m_pPlayer->pos;
		// 光るオブジェクト作成
		m_pFlashFactory->Create(D3DXVECTOR2(m_ThunderBoltPos.x, 200.0f), D3DXVECTOR2(180.0f, 180.0f));
	}
	if (m_WaitFrame == 90)
	{
		// 雷の刃作成
		m_pThunderFactory->Create(D3DXVECTOR2(m_ThunderBoltPos.x, 200.0f+90.0f+360.0f), D3DXVECTOR2(90.0f, 720.0f));
	}
	// 一定時間待機
	if (m_WaitFrame >= 120)
	{
		m_WaitFrame = 0;
		m_Muki = 0;
		m_State = IDLE;
	}
	else
	{
		m_WaitFrame++;
	}
}

void Boss_Raijin::Avator()
{
	switch (m_MoveCount)
	{
	case 0:
		m_BeforeShotPos = m_Pos;
		// マップ外へ移動
		SetMove(m_Pos, D3DXVECTOR2(SCREEN_WIDTH + m_Size.x / 2.0f, SCREEN_HEIGHT - m_Size.y / 2.0f - BLOCK_SIZE), m_State, m_Muki);
		break;
	case 1:
		// 分身を三体作成
		for (int i = 0; i < 4; i++)
		{
			m_pEnemyFactory->Create_BossAvator(m_Pos, D3DXVECTOR2((i*400.0f)+400.0f, 150.0f),Enemy_BossAvator::AVATOR_MODE::RAIJIN);
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

void Boss_Raijin::SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, Boss_Raijin::STATE_ENEMY_RAIJIN state, int muki)
{
	m_BeforeState = m_State;
	m_BeforeMuki = muki;
	m_State = Boss_Raijin::MOVE;
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

void Boss_Raijin::Move()
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

void Boss_Raijin::AfterHitCheckBlockX(DWORD result)
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
		if (m_State != BULLET_T && m_State != AVATOR) {
			m_Vel.y += m_Gravity;
		}
	}

	m_Pos.x += m_Vel.x;

	result = 0;
}

void Boss_Raijin::AfterHitCheckBlockY(DWORD result)
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

Boss_Raijin::~Boss_Raijin()
{
}
