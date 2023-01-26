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
#include "SwitchBulletFactory.h"
#include "SwitchBullet.h"

Boss_Raijin::Boss_Raijin(D3DXVECTOR2 pos, int ID, int textureNo,int muki,bool isDuo)
	: Enemy(pos, ID, D3DXVECTOR2(480.0f, 480.0f), D3DXVECTOR2(8.0f, 8.0f), textureNo, Enemy::ENEMY_TYPE::BOSS_RAIJIN,isDuo)
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_CollisionSize = D3DXVECTOR2(240.0f, 300.0f);
	m_HP = 2;
	m_Muki = muki;
	m_WaitFrame = 60;
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
	m_pSwitchBulletFactory = GetSwitchBulletFactory();
}

void Boss_Raijin::Uninit()
{
	if (!m_IsDuo) {
		SetGameScene(GAMESCENE_PICTURE_STAGE03_FINAL);
	}
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
	if (m_IsDie)
	{
		m_IsDie = false;
		m_IsStealth = true;
		m_AnimationPtn = 0;
		if (m_Pos.x >= BLOCK_SIZE * 32.0f - (m_Size.x / 2.0f))
		{
			m_Pos.x = BLOCK_SIZE * 32.0f - (m_Size.x / 2.0f) - 60.0f;
		}
		else if (m_Pos.x <= BLOCK_SIZE * 2.0f + (m_Size.x / 2.0f))
		{
			m_Pos.x = BLOCK_SIZE * 2.0f + (m_Size.x / 2.0f);
		}
		if (m_Muki % 2 == 0)
		{
			m_Muki = 6;
		}
		else
		{
			m_Muki = 7;
		}
		m_WaitFrame = 0;
		m_State = DEAD;
	}

	switch (m_State)
	{
	case Boss_Raijin::IDLE:
		if (m_WaitFrame >= 120)
		{
			m_WaitFrame = 0;
			 m_StateCount = 3;
			int num = Rand_int(4);
			if (m_LastTimeState == num)
			{
				if (num == 3)
				{
					num = 0;
				}
				else
				{
					num++;
				}
			}
			m_LastTimeState = (STATE_ENEMY_RAIJIN)num;
			m_State = (STATE_ENEMY_RAIJIN)num;
			//m_Muki += 2;
			//m_LastTimeState = (STATE_ENEMY_RAIJIN)AVATOR;
			//m_State = (STATE_ENEMY_RAIJIN)AVATOR;
			switch (num)
			{
			case 0:
				m_Muki += 0;
				m_State = SWITCH_BULLET;
				break;
			case 1:
				m_Muki += 4;
				m_State = BULLET_T;
				break;
			case 2:
				m_Muki += 0;
				m_State = THUNDERBLADE;
				break;
			case 3:
				m_Muki += 2;
				m_State = AVATOR;
				break;
			default:
				break;
			}
		}
		else
		{
			m_WaitFrame++;
		}

		break;
	case Boss_Raijin::SWITCH_BULLET:
		SwitchBullet();
		break;
	case Boss_Raijin::BULLET_T:
		ShotBullet_T();
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
		if (m_WaitFrame >= 10 && m_AnimationPtn < 5)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 5)
			{
				m_AnimationPtn = 5;
				if (!m_IsDuo)
				{
					m_IsActive = false;
				}
			}
		}
		else if (m_pEnemyFactory->CheckTogetherDie() && m_AnimationPtn == 5&&m_IsDuo)
		{
			// m_IsActive = false;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	default:
		break;
	}
	if (m_IsInvincible == true)
	{
		m_InvincibleTime++;

		if (m_InvincibleTime >= 120)
		{
			m_InvincibleTime = 0;
			m_IsInvincible = false;
		}
	}
}

void Boss_Raijin::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	if (!m_IsStealth) {
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	if (m_IsInvincible)
	{
		float alpha = (float)(m_InvincibleTime % 30);
		D3DXVECTOR2 basePos = GetBase();
		DrawSpriteColor(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha / 30.0f));
	}
	else
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSpriteColor(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void Boss_Raijin::SwitchBullet()
{
	switch (m_MoveCount)
	{
	case 0:
		if (m_WaitFrame >= 10 && m_AnimationPtn < 7) {
			m_AnimationPtn++;
			m_WaitFrame = 0;
		}
		if (m_WaitFrame == 40)
		{
			if (m_Muki % 2 == 0) {
				// 光るオブジェクト作成
				m_pFlashFactory->Create(D3DXVECTOR2(m_Pos.x - 100, BLOCK_SIZE * 18.0f - 120.0f), D3DXVECTOR2(180.0f, 180.0f));
			}
			else {
				// 光るオブジェクト作成
				m_pFlashFactory->Create(D3DXVECTOR2(m_Pos.x + 100, BLOCK_SIZE * 18.0f - 120.0f), D3DXVECTOR2(180.0f, 180.0f));
			}
		}
		if (m_WaitFrame == 120)
		{
			// 雷弾発射
			m_TargetPos = m_pPlayer->pos;
			m_TargetPos.y=  BLOCK_SIZE * 18.0f - 120.0f;
			if (m_Muki % 2 == 0) {
				m_pSwitchBulletFactory->Create(D3DXVECTOR2(m_Pos.x - 100, BLOCK_SIZE * 18.0f - 120.0f), D3DXVECTOR2(60.0f, 60.0f), m_pPlayer->pos, 1, SwitchBullet::BULLET_MODE::ONCE);
			}
			else {
				m_pSwitchBulletFactory->Create(D3DXVECTOR2(m_Pos.x + 100, BLOCK_SIZE * 18.0f - 120.0f), D3DXVECTOR2(60.0f, 60.0f), m_pPlayer->pos, 0, SwitchBullet::BULLET_MODE::ONCE);

			}
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_Muki -= 0;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 1:
	{
		// 雷弾がプレイヤーの後ろに行くまで待機
		if (m_pSwitchBulletFactory->CheckSwitchFlag()) {
			m_SwitchPos = m_pSwitchBulletFactory->GetSwitchPos();
			m_SwitchStartPos = m_Pos;
			m_MoveCount++;
		}
		break;
	}
	case 2:
		if (m_WaitFrame == 30) {
			m_IsStealth = false;
			// 爆発
			// 光るオブジェクト作成
			m_pFlashFactory->Create(m_Pos, D3DXVECTOR2(300.0f, 300.0f));
		}
		if (m_WaitFrame == 60) {
			// 爆発
			// 光るオブジェクト作成
			m_pFlashFactory->Create(D3DXVECTOR2( m_SwitchPos.x,m_SwitchStartPos.y), D3DXVECTOR2(300.0f, 300.0f));
		}
		if (m_WaitFrame == 120) {
			if (m_Muki % 2 == 0) {
				m_Muki += 1;
			}
			else {
				m_Muki -= 1;
			}
			m_Pos.x = m_SwitchPos.x;
			m_IsStealth = true;
		}
		if (m_WaitFrame == 180)
		{
			if (m_Muki % 2 == 0)
			{
				// 光るオブジェクト作成
				m_pFlashFactory->Create(D3DXVECTOR2(m_SwitchStartPos.x - 100, BLOCK_SIZE * 18.0f - 120.0f), D3DXVECTOR2(180.0f, 180.0f));
			}
			else
			{
				// 光るオブジェクト作成
				m_pFlashFactory->Create(D3DXVECTOR2(m_SwitchStartPos.x + 100, BLOCK_SIZE * 18.0f - 120.0f), D3DXVECTOR2(180.0f, 180.0f));
			}
		}
		if (m_WaitFrame == 240) {
			if (m_Muki % 2 == 0) {
				m_pSwitchBulletFactory->Create(D3DXVECTOR2(m_SwitchStartPos.x - 100, m_TargetPos.y), D3DXVECTOR2(60.0f, 60.0f), m_pPlayer->pos, 0, SwitchBullet::BULLET_MODE::TWICE);
			}
			else {
				m_pSwitchBulletFactory->Create(D3DXVECTOR2(m_SwitchStartPos.x + 100, m_TargetPos.y), D3DXVECTOR2(60.0f, 60.0f), m_pPlayer->pos, 1, SwitchBullet::BULLET_MODE::TWICE);
			}
			// 雷弾発射
			m_MoveCount++;
			m_WaitFrame = 0;
		}
		else {
			m_WaitFrame++;
		}
		break;
	case 3:
		// 待機
		if (m_WaitFrame == 60) {
			m_IsStealth = false;
			// 爆発
			// 光るオブジェクト作成
			m_pFlashFactory->Create(m_Pos, D3DXVECTOR2(300.0f, 300.0f));
		}
		if (m_WaitFrame == 90) {
			// 爆発
			// 光るオブジェクト作成
			m_pFlashFactory->Create(m_SwitchStartPos, D3DXVECTOR2(300.0f, 300.0f));
		}
		if (m_WaitFrame == 150) {
			// 元の座標へ移動
			if (m_Muki % 2 == 0) {
				m_Muki += 1;
			}
			else {
				m_Muki -= 1;
			}
			m_Pos.x = m_SwitchStartPos.x;
			m_IsStealth = true;
		}
		if (m_WaitFrame == 300) {
			m_State = IDLE;
			m_WaitFrame = 0;
			m_MoveCount = 0;
		}
		else {
			m_WaitFrame++;
		}
		break;
	default:
		break;
	}

}

void Boss_Raijin::ShotBullet_T()
{
	switch (m_MoveCount)
	{
	case 0:
		// 待機
		if (m_WaitFrame >= 6)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 5) {
				m_AnimationPtn = 4;
				m_BeforeShotPos = m_Pos;
				m_MoveCount++;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 1:
	{
		// 位置を修正
		D3DXVECTOR2 pos = m_Pos;
		if (m_Muki % 2 == 0) {
			pos.x -= 150;
			pos.y -= 200;
		}
		else {
			pos.x += 150;
			pos.y -= 200;
		}
		// X弾発射
		m_pRayFactory->CreateTRay(pos, m_pPlayer->pos);
		m_Muki -= 4;
		m_AnimationPtn = 0;
		m_MoveCount++;
		break;
	}
	case 2:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
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
		m_AnimationPtn = 0;
		D3DXVECTOR2 pos = D3DXVECTOR2(0.0f, 0.0f);
		if (m_Muki % 2 == 0) {
			pos = D3DXVECTOR2(1300.0f, 300.0f);
		}
		else {
			pos = D3DXVECTOR2(SCREEN_WIDTH-1300.0f, 300.0f);
		}
		SetMove(m_Pos, pos, m_State, m_Muki);
		break;
	}
	case 4:
		if (m_WaitFrame >= 60)
		{
			m_Muki += 4;
			m_WaitFrame = 0;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 5:
		if (m_WaitFrame >= 6)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 5) {
				m_AnimationPtn = 4;
				m_MoveCount++;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 6:
	{
		// 位置を修正
		D3DXVECTOR2 pos = m_Pos;
		if (m_Muki % 2 == 0) {
			pos.x -= 150;
			pos.y -= 200;
		}
		else {
			pos.x += 150;
			pos.y -= 200;
		}
		// X弾発射
		m_pRayFactory->CreateTRay(pos, m_pPlayer->pos);
		m_Muki -= 4;
		m_AnimationPtn = 0;
		m_MoveCount++;
	}
	case 7:
		// 待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 8:
		// 地面へ移動
		m_AnimationPtn = 0;
		SetMove(m_Pos, m_BeforeShotPos, m_State, m_Muki);
		break;
	case 9:
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

void Boss_Raijin::ThunderBlade()
{
	if (m_WaitFrame >= 10 && m_AnimationPtn < 7) {
		m_AnimationPtn++;
		m_WaitFrame = 0;
	}
	if (m_WaitFrame == 40)
	{
		m_ThunderBoltPos = m_pPlayer->pos;
		// 光るオブジェクト作成
		m_pFlashFactory->Create(D3DXVECTOR2(m_ThunderBoltPos.x, BLOCK_SIZE * 17.0f - 600.0f), D3DXVECTOR2(180.0f, 180.0f));
	}
	if (m_WaitFrame == 120)
	{
		// 雷の刃作成
		m_pThunderFactory->Create(D3DXVECTOR2(m_ThunderBoltPos.x, BLOCK_SIZE * 17.0f - 300.0f), D3DXVECTOR2(600.0f, 600.0f),0);
	}
	// 一定時間待機
	if (m_WaitFrame >= 300)
	{
		m_WaitFrame = 0;
		m_AnimationPtn = 0;
		m_Muki -= 0;
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
		if (m_WaitFrame >= 10 && m_AnimationPtn < 6) {
			m_AnimationPtn++;
			m_WaitFrame = 0;
		}
		else if (m_WaitFrame == 120) {
			m_MoveCount++;
		}
		else {
			m_WaitFrame++;
		}
		break;
	case 1:
		m_BeforeShotPos = m_Pos;
		m_AnimationPtn = 0;
		m_Muki -= 2;
		if (m_Muki % 2 == 0) {
			// マップ外へ移動
			SetMove(m_Pos, D3DXVECTOR2(SCREEN_WIDTH + m_Size.x / 2.0f, SCREEN_HEIGHT - m_Size.y / 2.0f - BLOCK_SIZE), m_State, m_Muki);
		}
		else {
			// マップ外へ移動
			SetMove(m_Pos, D3DXVECTOR2(0.0f - m_Size.x / 2.0f, SCREEN_HEIGHT - m_Size.y / 2.0f - BLOCK_SIZE), m_State, m_Muki);
		}
		break;
	case 2:
		// 分身を三体作成
		for (int i = 0; i < 4; i++)
		{
			m_pEnemyFactory->Create_BossAvator(m_Pos, D3DXVECTOR2((i*400.0f)+400.0f, 300.0f),D3DXVECTOR2(8.0f,8.0f), ENEMY_TYPE::RAIJINAVATOR,m_Muki);
		}
		m_MoveCount++;
	case 3:
		// 分身が消えるまで待機
		if (m_pEnemyFactory->CheckAliveRaijinAvator()) {
			m_MoveCount++;
		}
		break;
	case 4:
		// マップ内へ移動
		SetMove(m_Pos, m_BeforeShotPos, m_State, m_Muki);
		break;
	case 5:
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
		m_Muki = 1;
	}
	else
	{
		m_Muki = 0;
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
	//if (m_WaitFrame >= 10)
	//{
	//	m_AnimationPtn++;
	//	if (m_AnimationPtn >= 4)
	//	{
	//		m_AnimationPtn = 0;
	//	}
	//	m_WaitFrame = 0;
	//}
	//{
	//	m_WaitFrame++;
	//}
}

void Boss_Raijin::AfterHitCheckBlockX(DWORD result)
{
	if (m_State == DEAD)
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
	if (m_State == DEAD)
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
