#include "Boss_Kasya.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "Block.h"
#include "player.h"
#include "game.h"
#include "FireBallFactory.h"
#include "FireBall.h"

Boss_Kasya::Boss_Kasya(D3DXVECTOR2 pos, int ID, int textureNo) : 
	Enemy(pos, ID, D3DXVECTOR2(240.0f, 240.0f), D3DXVECTOR2(6.0f, 6.0f), textureNo)

{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 1;
	m_Muki = 0;
	m_IdleWaitFrame = 60;
	// 攻撃用変数初期化
	m_AttackWaitFrame = 60;
	m_AttackFrame = 60;
	m_AttackTextureNo = LoadTexture((char*)"data/TEXTURE/fade_white.png");
	m_CollisionSize = D3DXVECTOR2(120.0f, 120.0f);
	m_IsAttack = 0;
	// 移動攻撃用変数初期化
	m_MoveWaitFrame = 60;
	m_MoveFrame = 30;
	m_MoveSpeed = D3DXVECTOR2(10.0f, 8.0f);
	// 火の玉攻撃用変数初期化
	m_pFireBallFactory = GetFireBallFactory();
	m_SetThrowWaitFrame = 30;

	m_LanePosXList[0] = 0.0f + BLOCK_SIZE         + (m_Size.x / 2.0f);
	m_LanePosXList[1] = 0.0f + BLOCK_SIZE * 14.0f - (m_Size.x / 2.0f);
	m_LanePosXList[2] = 0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f);

	m_LanePosYList[0] = 0.0f + BLOCK_SIZE * 2.0f  + (m_Size.y / 2.0f);
	m_LanePosYList[1] = 0.0f + BLOCK_SIZE * 8.0f  + (m_Size.y / 2.0f);
	m_LanePosYList[2] = 0.0f + BLOCK_SIZE * 14.0f +  (m_Size.y / 2.0f);

}

void Boss_Kasya::Init()
{
}

void Boss_Kasya::Uninit()
{
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
		m_AnimationPtn = 0;
		if (m_Muki % 2 == 0) {
			m_Muki = 4;
		}
		else {
			m_Muki = 5;
		}
		m_WaitFrame = 0;
		m_State = DEAD;
	}

	switch (m_State)
	{
	case Boss_Kasya::IDLE:
		if (m_WaitFrame >= m_IdleWaitFrame)
		{
			m_AnimationPtn++;
			m_WaitFrame = 0;
			m_MoveCount = 0;
			switch (m_StateCount)
			{
			case 0:
				m_State = SETUP_THROW;
				break;
			case 1:
				m_State = SETUP_THROW;
				break;
			case 2:
				m_State = SETUP_MOVE;
				break;
			case 3:
				m_State = SETUP_MOVE;
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
	case Boss_Kasya::SETUP_ATTACK:
		SetUp_Attack();
		break;
	case Boss_Kasya::ATTACK:
		Attack();
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
			m_Muki = 0;
			m_AnimationPtn = 0;
			m_State = Boss_Kasya::IDLE;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Boss_Kasya::DEAD:
		break;
	default:
		break;
	}

	// 移動攻撃以外は地面との当たり判定をする
	if (m_State != Boss_Kasya::MOVE&&m_State != Boss_Kasya::MOVE_LEFT_RIGHT&&m_State != Boss_Kasya::MOVE_UP_DOWN) {
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

		result = 0;
		m_Vel.y += m_Gravity;
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
	}
	else {
		m_Pos += m_Vel;
	}

	// プレイヤーの方を向く関数
	// LookPlayer();

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);


}

void Boss_Kasya::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	switch (m_IsAttack)
	{
	case 1:
		DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x - 120.0f, basePos.y + m_Pos.y, m_CollisionSize.x, m_CollisionSize.y,
			0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case 2:
		DrawSprite(m_AttackTextureNo, basePos.x + m_Pos.x + 120.0f, basePos.y + m_Pos.y, m_CollisionSize.x, m_CollisionSize.y,
			0.0f, 0.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

Boss_Kasya::~Boss_Kasya()
{
}

void Boss_Kasya::SetUp_Attack()
{
	if (m_WaitFrame >= m_AttackWaitFrame)
	{
		m_AnimationPtn = 0;
		m_WaitFrame = 0;
		PLAYER* pPlayer = GetPlayer();
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		if (pVec.x > 0) {
			m_IsAttack = 1;
		}
		else {
			m_IsAttack = 2;
		}
		m_State = Boss_Kasya::ATTACK;
	}
	else
	{
		m_WaitFrame++;
	}
}

void Boss_Kasya::Attack()
{
	if (m_WaitFrame >= m_AttackFrame)
	{
		m_AnimationPtn = 0;
		m_WaitFrame = 0;
		m_IsAttack = 0;
		m_State = Boss_Kasya::IDLE;
	}
	else
	{
		m_WaitFrame++;
	}
}

void Boss_Kasya::SetUp_Move()
{
	switch (m_MoveCount)
	{
	case 0:
		// 上下、左右どっちの動きか決める
		if (m_StateCount % 2 == 0) {
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f), m_LanePosYList[0]));
		}
		else {
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f), m_LanePosYList[2]));
		}
		m_BeforeState = Boss_Kasya::SETUP_MOVE;
		break;
	case 1:
	// 規定フレーム待機
	if (m_WaitFrame >= m_MoveWaitFrame)
	{
		m_AnimationPtn++;
		m_WaitFrame = 0;
		m_IsAttack = 0;
		m_MoveCount = 0;
		if (m_StateCount % 2 == 0) {
			m_State = Boss_Kasya::MOVE_LEFT_RIGHT;
		}
		else {
			m_State = Boss_Kasya::MOVE_UP_DOWN;
		}
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
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_LanePosXList[0], m_Pos.y), D3DXVECTOR2(-m_MoveSpeed.x, 0.0f));
		break;
	case 1:
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_Pos.x, m_Pos.y + BLOCK_SIZE * 6.0f), D3DXVECTOR2(0.0f, m_MoveSpeed.y));
		break;
	case 2:
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_LanePosXList[2], m_Pos.y), D3DXVECTOR2(m_MoveSpeed.x, 0.0f));
		break;
	case 3:
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_Pos.x, m_Pos.y + BLOCK_SIZE * 6.0f), D3DXVECTOR2(0.0f, m_MoveSpeed.y));
		break;
	case 4:
		m_BeforeState = Boss_Kasya::MOVE_LEFT_RIGHT;
		SetMove(m_Pos, D3DXVECTOR2(m_LanePosXList[0], m_Pos.y), D3DXVECTOR2(-m_MoveSpeed.x, 0.0f));
		break;
	case 5:
		m_State = Boss_Kasya::WAIT;
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
}

void Boss_Kasya::Move()
{
	m_Vel += m_MoveVec;
	D3DXVECTOR2 vec = (m_Pos + m_Vel) - m_StartPos;
	float distance = D3DXVec2Length(&vec);
	if (distance >= m_MoveDistance) {
		m_State = m_BeforeState;
		m_Pos = m_EndPos;
		m_MoveCount++;
	}
}

void Boss_Kasya::SetUp_Throw()
{
	switch (m_MoveCount)
	{
	case 0:
		// 上下、左右どっちの動きか決める
		m_BeforeState = Boss_Kasya::SETUP_THROW;
		SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f), 0.0f + BLOCK_SIZE * 11.0f - (m_Size.y / 2.0f)));
		if (m_StateCount % 2 == 0) {
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f), m_LanePosYList[1]));
		}
		else {
			SetMove(m_Pos, D3DXVECTOR2(0.0f + BLOCK_SIZE * 31.0f - (m_Size.x / 2.0f), m_LanePosYList[1]));
		}
		break;
	case 1:
		m_AnimationPtn = 0;
		// 規定フレーム待機
		if (m_WaitFrame >= m_MoveWaitFrame)
		{
			m_WaitFrame = 0;
			m_Muki = 2;
			m_AnimationPtn = 1;
			if (m_StateCount % 2 == 0) {
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
	m_pFireBallFactory->Create(m_Pos, 1, D3DXVECTOR2(-3.0f, 6.0f), FireBall::MODE::KASYA_ONESHOT);
	m_State = WAIT;
}

void Boss_Kasya::ThreeShot()
{
	// 火の玉を三つ生成
	// 規定フレーム待機
	if (m_WaitFrame >= 20)
	{
		m_WaitFrame = 0;
		// 火の玉を生成
		m_pFireBallFactory->Create(m_Pos, 1, D3DXVECTOR2(-10.0f - ((float)m_NowShotFireBall*2.0f), -6.0f), FireBall::MODE::KASYA_THREESHOT);
		m_NowShotFireBall++;
		if (m_NowShotFireBall >= 3) {
			m_State = WAIT;
			m_NowShotFireBall = 0;
		}
	}
	else
	{
		m_WaitFrame++;
	}
}
