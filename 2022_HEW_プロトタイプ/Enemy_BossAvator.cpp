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
#include "sound.h"

static int	g_SE_thunder;		// SEの識別子
static int	g_SE_ult;		// SEの識別子
static int g_SE_Fujin_decision;
static int g_SE_Raijin_decision;

static int g_Texture_warning;


Enemy_BossAvator::Enemy_BossAvator(D3DXVECTOR2 pos, int ID, int textureNo, D3DXVECTOR2 targetPos,D3DXVECTOR2 divid, Enemy::ENEMY_TYPE mode,int muki)
	: Enemy(pos, ID, D3DXVECTOR2(300.0f, 300.0f), divid, textureNo, mode),m_TargetPos(targetPos)
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = muki;
	m_pWindBladeFactory = GetWindBladeFactory();
	m_pThunderBladeFactory = GetThunderBladeFactory();
	m_pFlashFactory = GetFlashFactory();
	m_pEnemyFactory = GetEnemyFactory();

	// 音関連の初期化
	g_SE_thunder = LoadSound((char*)"data/SE/Raijin_thunder.wav");
	SetVolume(g_SE_thunder, 0.5f);
	g_SE_ult = LoadSound((char*)"data/SE/Fujin_ult.wav");
	SetVolume(g_SE_ult, 1.3f);

	g_Texture_warning = LoadTexture((char*)"data/TEXTURE/warning.png");

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
		switch (m_EnemyType)
		{
		case ENEMY_TYPE::FUFINAVATOR:
			if (m_Muki % 2 == 0)
			{
				m_Muki = 10;
			}
			else
			{
				m_Muki = 11;
			}
			break;
		case ENEMY_TYPE::RAIJINAVATOR:
			if (m_Muki % 2 == 0)
			{
				m_Muki = 6;
			}
			else
			{
				m_Muki = 7;
			}
			break;
		default:
			break;
		}
		m_WaitFrame = 0;
		m_State = DEAD;
	}
	switch (m_State)
	{
	case Enemy_BossAvator::IDLE:
		switch (m_EnemyType)
		{
		case ENEMY_TYPE::FUFINAVATOR:
			m_State = WINDBLADE;
			break;
		case ENEMY_TYPE::RAIJINAVATOR:
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
		if (m_EnemyType == ENEMY_TYPE::FUFINAVATOR) {
			Move_Fujin();
		}
		else {
			Move_Raijin();
		}
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
	if (m_EnemyType == ENEMY_TYPE::FUFINAVATOR) {
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeFujinTable[m_AnimationPtn], M_MukiFujinTable[m_Muki], m_pttern.x, m_pttern.y);
	}
	else {
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeRaijinTable[m_AnimationPtn], M_MukiRaijinTable[m_Muki], m_pttern.x, m_pttern.y);
	}

	if (m_IsDead == false)
	{
		DrawSprite(g_Texture_warning, basePos.x + m_Pos.x, basePos.y + m_Pos.y - 120.0f, 120.0f, 120.0f,
			0.0f, 0.0f, 1.0f, 1.0f);
	}
}

void Enemy_BossAvator::WindBlade()
{
	switch (m_MoveCount)
	{
	case 0:
		// 決められた場所へ移動
		SetMove_Fujin(m_Pos, m_TargetPos, m_State, m_Muki);
		break;
	case 1:
		m_IsEndSetUp = true;
		// 他の分身が準備完了しているかチェック
		if (m_pEnemyFactory->CheckFujinAvatorSetEnd())
		{
			m_MoveCount++;
			m_WaitFrame = 0;
			m_Muki += 6;
		}
	case 2:
		if (m_WaitFrame >= 60) {
			m_WaitFrame = 0;
			m_MoveCount++;
		}
		else {
			m_WaitFrame++;
		}
		break;
	case 3:
		if (m_WaitFrame >= 10) {
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 4) {
				m_AnimationPtn = 3;
				m_MoveCount++;
			}
		}
		else {
			m_WaitFrame++;
		}
		break;
	case 4:
		if (m_WaitFrame >= 60) {
			m_WaitFrame = 0;
			m_MoveCount++;
		}
		else {
			m_WaitFrame++;
		}
		break;
	case 5:
		if (m_WaitFrame == 120)
		{
			for (int i = 0; i < 3; i++)
			{
				// 風の刃作成
				m_pWindBladeFactory->Create(D3DXVECTOR2(m_Pos.x - 100.0f, m_Pos.y +(i*100)-100), D3DXVECTOR2(100.0f, 100.0f), 1,m_IsDuo);
				PlaySound(g_SE_ult, 0);
			}
		}
		// 一定時間待機
		if (m_WaitFrame >= 300)
		{
			m_WaitFrame = 0;
			m_Muki -= 6;
			m_AnimationPtn = 0;
			m_IsDie = true;
			m_IsDead = true;
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
		SetMove_Raijin(m_Pos, m_TargetPos, m_State, m_Muki);
		break;
	case 1:
		m_IsEndSetUp = true;
		// 他の分身が準備完了しているかチェック
		if (m_pEnemyFactory->CheckRaijinAvatorSetEnd())
		{
			m_MoveCount++;
			m_WaitFrame = 0;
		}
		break;
	case 2:
		if (m_WaitFrame >= 60) {
			m_WaitFrame = 0;
			m_MoveCount++;
		}
		else {
			m_WaitFrame++;
		}
		break;
	case 3:
		if (m_WaitFrame >= 10 && m_AnimationPtn < 7) {
			m_AnimationPtn++;
			m_WaitFrame = 0;
		}
		//if (m_WaitFrame == 40)
		//{
		//	// 光るオブジェクト作成
		//	m_pFlashFactory->Create(D3DXVECTOR2(m_Pos.x, 200.0f), D3DXVECTOR2(180.0f, 180.0f));
		//}
		if (m_WaitFrame == 80)
		{
			// 雷の刃作成
			m_pThunderBladeFactory->Create(D3DXVECTOR2(m_Pos.x, BLOCK_SIZE * 17.0f - 300.0f), D3DXVECTOR2(700.0f, 600.0f),1,m_IsDuo);
			PlaySound(g_SE_thunder, 0);
		}
		// 一定時間待機
		if (m_WaitFrame >= 300)
		{
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_Muki -= 0;
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

void Enemy_BossAvator::SetMove_Fujin(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki)
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
	m_AnimationPtn = 1;
	// 移動方向からアニメーションの向きを決定
	if (vec.x > 0)
	{
		m_Muki = 1;
	}
	else
	{
		m_Muki = 0;
	}
}

void Enemy_BossAvator::SetMove_Raijin(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki)
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
		m_Muki = 1;
	}
	else
	{
		m_Muki = 0;
	}
}

void Enemy_BossAvator::Move_Fujin()
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
		if (m_AnimationPtn >= 3)
		{
			m_AnimationPtn = 1;
		}
		m_WaitFrame = 0;
	}
	{
		m_WaitFrame++;
	}
}

void Enemy_BossAvator::Move_Raijin()
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
