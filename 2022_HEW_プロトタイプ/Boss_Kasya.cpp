#include "Boss_Kasya.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "Block.h"
#include "game.h"
#include "FireBallFactory.h"
#include "FireBall.h"
#include "game.h"
#include "sound.h"

static int	g_SE_fire;		// SEの識別子
static int	g_SE_hasiri;	// SEの識別子
static int	g_SE_teisi;		// SEの識別子
static int	g_SE_dead;	// SEの識別子

Boss_Kasya::Boss_Kasya(D3DXVECTOR2 pos, int ID, int textureNo) : 
	Enemy(pos, ID, D3DXVECTOR2(360.0f, 360.0f), D3DXVECTOR2(6.0f, 10.0f), textureNo,ENEMY_TYPE::BOSS_KASYA)

{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_CollisionSize=D3DXVECTOR2(300.0f, 240.0f);
	m_HP =10;
	m_Muki = 0;
	m_IdleWaitFrame = 120;
	// 移動攻撃用変数初期化
	m_MoveWaitFrame = 60;
	m_MoveFrame = 30;
	m_MoveSpeed = D3DXVECTOR2(10.0f, 8.0f);
	// 火の玉攻撃用変数初期化
	m_pFireBallFactory = GetFireBallFactory();
	m_SetThrowWaitFrame = 30;
	m_OneShotDirection = 1.0f;

	m_pPlayer = GetPlayer();

	m_LanePosXList[0] = 0.0f + BLOCK_SIZE         + (m_Size.x / 2.0f);
	m_LanePosXList[1] = 0.0f + BLOCK_SIZE * 14.0f - (m_Size.x / 2.0f);
	m_LanePosXList[2] = 0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f);

	m_LanePosYList[0] = 0.0f + BLOCK_SIZE * 6.0f  - (m_Size.y / 2.0f);
	m_LanePosYList[1] = 0.0f + BLOCK_SIZE * 12.0f -  (m_Size.y / 2.0f);
	m_LanePosYList[2] = 0.0f + BLOCK_SIZE * 18.0f -  (m_Size.y / 2.0f);

}

void Boss_Kasya::Init()
{
	//音関連の初期化
	g_SE_hasiri= LoadSound((char*)"data/SE/player_dash.wav");
	SetVolume(g_SE_hasiri, 1.5f);
	g_SE_teisi = LoadSound((char*)"data/SE/Kasya_Teisi.wav");
	SetVolume(g_SE_teisi, 0.5f);
	g_SE_fire = LoadSound((char*)"data/SE/Kasya_fire.wav");
	SetVolume(g_SE_fire, 0.5f);
	g_SE_dead = LoadSound((char*)"data/SE/Kasya_dead.wav");
	SetVolume(g_SE_dead, 0.5f);
}

void Boss_Kasya::Uninit()
{
	StopSound(g_SE_hasiri);
	SetGameScene(GAMESCENE_PICTURE_STAGE03);
}

void Boss_Kasya::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	m_OldPos = m_Pos;
	DWORD result = 0;
	// 死亡していたなら死亡状態へ
	if (m_IsDie) {
		m_IsDie = false;
		m_IsEndDead = false;
		m_AnimationPtn = 0;
		if (m_Muki % 2 == 0) {
			m_Muki = 2;
		}
		else {
			m_Muki = 3;
		}
		m_WaitFrame = 0;
		PlaySound(g_SE_dead, 0);
		m_State = DEAD;
	}

	switch (m_State)
	{
	case Boss_Kasya::IDLE:
		if (m_WaitFrame >= m_IdleWaitFrame)
		{
			m_WaitFrame = 0;
			m_MoveCount = 0;
			if (m_LastTimeState!=SETUP_MOVE)
			{
				m_State = SETUP_MOVE;
				m_LastTimeState = SETUP_MOVE;
			}
			else if(m_LastTimeState != SETUP_THROW)
			{
				m_State = SETUP_THROW;
				m_LastTimeState = SETUP_THROW;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		if (m_AnimeFrame >= 10)
		{
			m_AnimationPtn++;
			if (m_AnimationPtn >= 2)
			{
				m_AnimationPtn = 0;
			}
			m_AnimeFrame = 0;
		}
		else
		{
			m_AnimeFrame++;
		}
		break;
	case Boss_Kasya::SETUP_MOVE:
		SetUp_Move();
		break;
	case Boss_Kasya::MOVE_LEFT_RIGHT:
		Move_Left_Right();
		break;
	case Boss_Kasya::MOVE_UP_DOWN:
		Move_Up_Down();
		break;
	case Boss_Kasya::MOVE:
		Move();
		break;
	case Boss_Kasya::STOP:
		Stop();
		break;
	case Boss_Kasya::SETUP_THROW:
		SetUp_Throw();
		break;
	case Boss_Kasya::THROW_ONESHOT:
		OneShot();
		break;
	case Boss_Kasya::THROW_THREESHOT:
		ThreeShot();
		break;
	case Boss_Kasya::WAIT:
		// 一定時間待機
		if (m_WaitFrame >= 60)
		{
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_State = Boss_Kasya::IDLE;
		}
		else
		{
			m_WaitFrame++;
		}
		if (m_AnimeFrame >= 10)
		{
			m_AnimationPtn++;
			if (m_AnimationPtn >= 2)
			{
				m_AnimationPtn = 0;
			}
			m_AnimeFrame = 0;
		}
		else
		{
			m_AnimeFrame++;
		}

		break;
	case Boss_Kasya::DEAD:
		if (m_WaitFrame >= 10 && !m_IsEndDead)
		{
			m_AnimationPtn++;
			m_WaitFrame = 0;
			if (m_AnimationPtn >= 6)
			{
				m_IsEndDead = true;
				m_AnimationPtn = 5;
			}
		}
		else if (m_IsEndDead&&m_WaitFrame >= 120)
		{
			m_IsActive = false;
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

void Boss_Kasya::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
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

void Boss_Kasya::AfterHitCheckBlockX(DWORD result)
{
	if (m_State != Boss_Kasya::MOVE&&m_State != Boss_Kasya::MOVE_LEFT_RIGHT&&m_State != Boss_Kasya::MOVE_UP_DOWN) {
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
}

void Boss_Kasya::AfterHitCheckBlockY(DWORD result)
{
	if (m_State != Boss_Kasya::MOVE&&m_State != Boss_Kasya::MOVE_LEFT_RIGHT&&m_State != Boss_Kasya::MOVE_UP_DOWN) {
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
	}
	else {
		m_Pos += m_Vel;
	}
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

Boss_Kasya::~Boss_Kasya()
{
}


void Boss_Kasya::SetUp_Move()
{
	switch (m_MoveCount)
	{
	case 0:
		m_BeforeMuki = m_Muki;
		// 上下、左右どっちの動きか決める
		if (Rand_int(2) == 0) {
			m_LastTimeState_Move = MOVE_LEFT_RIGHT;
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f), m_LanePosYList[0]));
		}
		else {
			m_LastTimeState_Move = MOVE_UP_DOWN;
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f), m_LanePosYList[2]));
		}
		m_BeforeState = Boss_Kasya::SETUP_MOVE;
		PlaySound(g_SE_hasiri, -1);
		m_IsStop = true;
		break;
	case 1:
	// 規定フレーム待機
		m_Muki = 0;
	if (m_WaitFrame >= m_MoveWaitFrame)
	{
		m_AnimationPtn++;
		m_WaitFrame = 0;
		m_MoveCount = 0;
		if (m_LastTimeState_Move==MOVE_LEFT_RIGHT) {
			m_BeforeMuki = 8;
			m_State = Boss_Kasya::MOVE_LEFT_RIGHT;
		}
		else {
			m_State = Boss_Kasya::MOVE_UP_DOWN;
		}

		PlaySound(g_SE_hasiri, -1);
	}
	else
	{
		m_WaitFrame++;
	}
		break;
	}
}

void Boss_Kasya::Move_Left_Right()
{
	switch (m_MoveCount)
	{
	case 0:
		m_IsStop = true;
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_LanePosXList[0], m_Pos.y), D3DXVECTOR2(-m_MoveSpeed.x, 0.0f));
		break;
	case 1:
		m_IsStop = false;
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		m_BeforeMuki = 7;

		SetMove(m_Pos, D3DXVECTOR2(m_Pos.x, m_Pos.y + BLOCK_SIZE * 6.0f), D3DXVECTOR2(0.0f, m_MoveSpeed.y),true);
		break;
	case 2:
		m_IsStop = true;
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_LanePosXList[2], m_Pos.y), D3DXVECTOR2(m_MoveSpeed.x, 0.0f));
		break;
	case 3:
		m_IsStop = false;
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		m_BeforeMuki = 6;
		SetMove(m_Pos, D3DXVECTOR2(m_Pos.x, m_Pos.y + BLOCK_SIZE * 6.0f), D3DXVECTOR2(0.0f, m_MoveSpeed.y),false);
		break;
	case 4:
		m_IsStop = true;
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_LanePosXList[0], m_Pos.y), D3DXVECTOR2(-m_MoveSpeed.x, 0.0f));
		break;
	case 5:
		m_State = Boss_Kasya::WAIT;
		m_Muki = 1;
		StopSound(g_SE_hasiri);
		break;
	default:
		break;
	}
}

void Boss_Kasya::Move_Up_Down()
{
	switch (m_MoveCount)
	{
	case 0:
		m_BeforeState = Boss_Kasya::MOVE_UP_DOWN;
		m_BeforeMuki = 0;
		SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 28.0f - (m_Size.x / 2.0f), m_LanePosYList[0]));
		break;
	case 1:
		if (m_WaitFrame >= m_MoveFrame)
		{
			m_WaitFrame = 0;
			m_BeforeState = Boss_Kasya::MOVE_UP_DOWN;
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 22.0f - (m_Size.x / 2.0f), m_LanePosYList[2]));
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 2:
		if (m_WaitFrame >= m_MoveFrame)
		{
			m_WaitFrame = 0;
			m_BeforeState = Boss_Kasya::MOVE_UP_DOWN;
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 15.0f - (m_Size.x / 2.0f), m_LanePosYList[0]));
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 3:
		if (m_WaitFrame >= m_MoveFrame)
		{
			m_WaitFrame = 0;
			m_BeforeState = Boss_Kasya::MOVE_UP_DOWN;
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 10.0f - (m_Size.x / 2.0f), m_LanePosYList[2]));
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 4:
		if (m_WaitFrame >= m_MoveFrame)
		{
			m_WaitFrame = 0;
			m_BeforeState = Boss_Kasya::MOVE_UP_DOWN;
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE + (m_Size.x / 2.0f), m_LanePosYList[1]));
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 5:
		m_State = Boss_Kasya::WAIT;
		m_Muki = 1;
		StopSound(g_SE_hasiri);
		break;
	default:
		break;
	}
}

void Boss_Kasya::SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, D3DXVECTOR2 moveVec)
{
	m_State = Boss_Kasya::MOVE;
	m_StartPos = startPos;
	m_EndPos = endPos;
	m_MoveVec = moveVec;
	m_NowDistance = 0.0f;
	D3DXVECTOR2 vec = endPos - startPos;
	m_MoveDistance = D3DXVec2Length(&vec);
	PlaySound(g_SE_hasiri, -1);
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

void Boss_Kasya::SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos)
{
	m_State = Boss_Kasya::MOVE;
	m_StartPos = startPos;
	m_EndPos = endPos;
	m_NowDistance = 0.0f;
	D3DXVECTOR2 vec = endPos - startPos;
	D3DXVec2Normalize(&m_MoveVec,&vec);
	m_MoveVec.x *= 12.0f;
	m_MoveVec.y *= 12.0f;
	m_MoveDistance = D3DXVec2Length(&vec);
	PlaySound(g_SE_hasiri, -1);
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

void Boss_Kasya::SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, D3DXVECTOR2 moveVec, bool isRight)
{
	m_State = Boss_Kasya::MOVE;
	m_StartPos = startPos;
	m_EndPos = endPos;
	m_MoveVec = moveVec;
	m_NowDistance = 0.0f;
	D3DXVECTOR2 vec = endPos - startPos;
	m_MoveDistance = D3DXVec2Length(&vec);
	PlaySound(g_SE_hasiri, -1);
	// 移動方向からアニメーションの向きを決定
	if (isRight)
	{
		m_Muki = 7;
	}
	else
	{
		m_Muki = 6;
	}
}

void Boss_Kasya::Move()
{
	m_Vel += m_MoveVec;
	D3DXVECTOR2 vec = (m_Pos + m_Vel) - m_StartPos;
	float distance = D3DXVec2Length(&vec);
	if (distance >= m_MoveDistance) {
		if (m_IsStop) {
			m_Muki += 2;
			m_State = Boss_Kasya::STOP;
			StopSound(g_SE_hasiri);
			PlaySound(g_SE_teisi, 0);
		}
		else {
			m_Muki = m_BeforeMuki;
			m_State = m_BeforeState;
		}
		m_Pos = m_EndPos;
		m_MoveCount++;
		m_AnimationPtn = 0;
	}
	if (m_AnimeFrame >= 10)
	{
		m_AnimationPtn++;
		if (m_AnimationPtn >= 4) {
			m_AnimationPtn = 0;
		}
		m_AnimeFrame = 0;
	}
	{
		m_AnimeFrame++;
	}
}

void Boss_Kasya::Stop()
{
	if (m_AnimeFrame >= 20)
	{
		m_AnimationPtn++;
		if (m_AnimationPtn >= 2) {
			m_Muki = m_BeforeMuki;
			m_State = m_BeforeState;	
			m_AnimationPtn = 0;
		}
		m_AnimeFrame = 0;
	}
	else{
		m_AnimeFrame++;
	}
}

void Boss_Kasya::SetUp_Throw()
{
	switch (m_MoveCount)
	{
	case 0:
		// どっちの火の玉攻撃か決める
		m_IsStop = true;
		m_BeforeState = Boss_Kasya::SETUP_THROW;
		if (Rand_int(2) == 0) {
			m_LastTimeState_Shot = THROW_ONESHOT;
			m_Muki = 0;
			m_BeforeMuki = m_Muki;
			SetMove(m_Pos, D3DXVECTOR2(m_LanePosXList[2], m_LanePosYList[1]));
		}
		else {
			m_LastTimeState_Shot = THROW_THREESHOT;
			D3DXVECTOR2 pos;
			// プレイヤーの座標から移動地点をセット
			if (m_pPlayer->pos.x > SCREEN_WIDTH/2.0f) {
				pos.x = m_LanePosXList[2];
				m_Muki = 0;
			}
			else {
				pos.x = m_LanePosXList[0];
				m_Muki = 1;
			}
			m_BeforeMuki = m_Muki;
			if (m_pPlayer->pos.y <=BLOCK_SIZE*7.0f) {
				pos.y = m_LanePosYList[0];
			}
			else if (m_pPlayer->pos.y > BLOCK_SIZE*7.0f&&m_pPlayer->pos.y <= BLOCK_SIZE * 13.0f) {
				pos.y = m_LanePosYList[1];
			}
			else if (m_pPlayer->pos.y > BLOCK_SIZE*13.0f&&m_pPlayer->pos.y <= BLOCK_SIZE * 19.0f) {
				pos.y = m_LanePosYList[2];
			}
			SetMove(m_Pos, pos);
		}
		break;
	case 1:

		m_AnimationPtn = 0;
		// 規定フレーム待機
		if (m_WaitFrame >= m_MoveWaitFrame)
		{
			m_WaitFrame = 0;
			m_Muki += 4;
			m_AnimationPtn = 1;
			if (m_LastTimeState_Shot==THROW_ONESHOT) {
				m_State = Boss_Kasya::THROW_ONESHOT;
			}
			else {
				m_State = Boss_Kasya::THROW_THREESHOT;
			}
			
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	}
}

void Boss_Kasya::OneShot()
{
	// 火の玉を生成
	if (m_AnimeFrame >= 20) {
		m_AnimeFrame = 0;
		m_AnimationPtn++;
		if (m_AnimationPtn == 3) {
			D3DXVECTOR2 pos = m_Pos;
			pos.x -= 150.0f;
			pos.y += 15.0f;
			m_pFireBallFactory->Create(pos, 1, D3DXVECTOR2(-6.0f, m_OneShotDirection * 12.0f), FireBall::MODE::KASYA_ONESHOT);
			PlaySound(g_SE_fire, 0);
		}
		if (m_AnimationPtn >= 6) {
			m_OneShotDirection *= -1.0f;
			m_AnimationPtn = 0;
			m_Muki -= 4;
			m_State = WAIT;
		}
	}
	else
	{
		m_AnimeFrame++;
	}
}

void Boss_Kasya::ThreeShot()
{
	// 火の玉を三つ生成
	if (m_AnimeFrame >= 20) {
		m_AnimeFrame = 0;
		m_AnimationPtn++;
		if (m_AnimationPtn >= 2 && m_AnimationPtn < 5) {
			// 火の玉が口から出るように位置を調整
			D3DXVECTOR2 pos = m_Pos;
			pos.y += 15.0f;
			if (m_Muki % 2 == 0) {
				pos.x -= 150.0f;
				m_pFireBallFactory->Create(pos, 1, D3DXVECTOR2(-5.0f - ((float)m_NowShotFireBall*7.5f), -3.0f), FireBall::MODE::KASYA_THREESHOT);
				PlaySound(g_SE_fire, 0);
			}
			else {
				pos.x += 150.0f;
				m_pFireBallFactory->Create(pos, 1, D3DXVECTOR2( 5.0f + ((float)m_NowShotFireBall*7.5f), -3.0f), FireBall::MODE::KASYA_THREESHOT);
				PlaySound(g_SE_fire, 0);
			}
			m_NowShotFireBall++;
		}
		if (m_AnimationPtn >= 6) {
			m_AnimationPtn = 0;
			m_Muki -= 4;
			m_NowShotFireBall = 0;
			m_State = WAIT;
		}
	}
	else
	{
		m_AnimeFrame++;
	}

}
