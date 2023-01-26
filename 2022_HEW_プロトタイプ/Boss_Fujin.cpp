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

Boss_Fujin::Boss_Fujin(D3DXVECTOR2 pos, int ID, int textureNo,bool isDuo)
	: Enemy(pos,ID,D3DXVECTOR2(480.0f,480.0f), D3DXVECTOR2(6.0f, 12.0f), textureNo, Enemy::ENEMY_TYPE::BOSS_FUJIN,isDuo)
{
	// �G�̃T�C�Y��ݒ�
	m_Gravity = 4.0f;
	m_CollisionSize = D3DXVECTOR2(240.0f, 300.0f);
	m_HP = 2;
	m_Muki = 0;
	m_pPlayer = GetPlayer();
	m_pRayFactory = GetRayFactory();
	m_pWindBladeFactory = GetWindBladeFactory();
	m_pFlashFactory = GetFlashFactory();
	m_pEnemyFactory = GetEnemyFactory();
	m_AttackDistance = 300.0f;
	// �U���p�ϐ�
	m_AttackTextureNo=LoadTexture((char*)"data/TEXTURE/fade_white.png");
	m_AttackCollisionSize = D3DXVECTOR2(200.0f, 360.0f);
	m_AttackSize = m_AttackCollisionSize;
	// �z�����ݍU���p�ϐ�
	m_InHalePower = D3DXVECTOR2(5.0f, 5.0f);
}

void Boss_Fujin::Init()
{
}

void Boss_Fujin::Uninit()
{
	if (m_IsDuo) {
		SetGameScene(GAMESCENE_PICTURE_RESULT);
	}
	else {
		SetGameScene(GAMESCENE_PICTURE_STAGE03_RAIJIN);
	}
}

void Boss_Fujin::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	m_OldPos = m_Pos;
	DWORD result = 0;
	// ���S���Ă����Ȃ玀�S��Ԃ�
	if (m_IsDie)
	{
		m_IsDie = false;
		m_IsAttack = false;
		m_AnimationPtn = 0;
		if (m_Pos.x >= BLOCK_SIZE * 32.0f - (m_Size.x / 2.0f))
		{
			m_Pos.x = BLOCK_SIZE * 32.0f - (m_Size.x / 2.0f) - 60.0f;
		}
		if (m_Muki % 2 == 0)
		{
			m_Muki = 10;
		}
		else
		{
			m_Muki = 11;
		}
		m_WaitFrame = 0;
		m_State = DEAD;
	}
	switch (m_State)
	{
	case Boss_Fujin::IDLE:
		if (m_WaitFrame >= 120)
		{
			m_WaitFrame = 0;
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
			m_LastTimeState = (STATE_ENEMY_FUJIN)num;
			m_State = (STATE_ENEMY_FUJIN)num;
			//m_Muki += 4;
			//m_LastTimeState = (STATE_ENEMY_FUJIN)AVATOR;
			//m_State = (STATE_ENEMY_FUJIN)AVATOR;
			switch (num)
			{
			case 0:
				m_Muki += 8;
				m_State = INHALE;
				break;
			case 1:
				m_Muki += 2;
				m_State = BULLET_X;
				break;
			case 2:
				m_Muki += 6;
				m_State = WINDBLADE;
				break;
			case 3:
				m_Muki += 4;
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
	case Boss_Fujin::ATTACK:
		if (m_Muki % 2 == 0)
		{
			m_AttackPos = D3DXVECTOR2(m_Pos.x - 100.0f, m_Pos.y + 60.0f);
		}
		else
		{
			m_AttackPos = D3DXVECTOR2(m_Pos.x + 100.0f, m_Pos.y + 60.0f);
		}
		if (m_AnimeFrame >= 10 && m_AnimationPtn < 3) {
			m_AnimeFrame = 0;
			m_AnimationPtn++;
		}
		else if (m_AnimeFrame >= 80) {
			m_AnimeFrame = 0;
			m_AnimationPtn = 0;
			m_Muki -= 6;
			m_State = IDLE;
			m_IsAttack = false;
		}
		else {
			m_AnimeFrame++;
		}
		break;
	case Boss_Fujin::INHALE:
		InHale();
		break;
	case Boss_Fujin::BULLET_X:
		ShotBullet_X();
		break;
	case Boss_Fujin::WINDBLADE:
		if (m_AnimeFrame >= 10 && m_AnimationPtn < 3) {
			m_AnimeFrame = 0;
			m_AnimationPtn++;
		}
		else {
			m_AnimeFrame++;
		}

		if (m_WaitFrame == 80)
		{
			// ����I�u�W�F�N�g�쐬
			m_pFlashFactory->Create(D3DXVECTOR2(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 200.0f), D3DXVECTOR2(180.0f, 180.0f));
		}
		if (m_WaitFrame == 160)
		{
			// ���̐n�쐬
			m_pWindBladeFactory->Create(D3DXVECTOR2(SCREEN_WIDTH + 60.0f, SCREEN_HEIGHT - 200.0f),D3DXVECTOR2(240.0f,240.0f), 1);
		}
		// ��莞�ԑҋ@
		if (m_WaitFrame >= 300)
		{
			m_WaitFrame = 0;
			m_AnimationPtn = 0;
			m_Muki -= 6;
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
		// ��莞�ԑҋ@
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
		else if (m_pEnemyFactory->CheckTogetherDie() && m_AnimationPtn == 5 && m_IsDuo)
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

void Boss_Fujin::Draw()
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
	//if (m_State == Boss_Fujin::ATTACK)
	//{
	//	DrawSprite(m_AttackTextureNo, basePos.x + m_AttackPos.x, basePos.y + m_AttackPos.y, m_AttackCollisionSize.x, m_AttackCollisionSize.y,
	//		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	//}
}

void Boss_Fujin::InHale()
{
	// �v���C���[�Ƃ̋������v�Z
	D3DXVECTOR2 vec = m_Pos - m_pPlayer->pos;
	float distance = D3DXVec2Length(&vec);
	// �v���C���[�ƈ��l�ȏ�߂Â�����I��
	if (distance <= m_AttackDistance)
	{
		m_WaitFrame = 0;
		m_Muki -= 8;
		m_Muki += 6;
		m_AnimationPtn = 0;
		m_State = ATTACK;
		m_IsAttack = true;
	}
	// �x�N�g���𐳋K��
	D3DXVec2Normalize(&vec, &vec);
	vec.x *= m_InHalePower.x;
	vec.y *= m_InHalePower.y;
	// �v���C���[���z������
	m_pPlayer->vel += vec;
	// �K��t���[���o�߂��Ă����Ȃ�I��
	if (m_WaitFrame >= 600)
	{
		m_WaitFrame = 0;
		m_Muki -= 8;
		m_Muki += 6;
		m_AnimationPtn = 0;
		m_State = ATTACK;
		m_IsAttack = true;
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
		// �ҋ@
		if (m_WaitFrame >= 10)
		{
			m_WaitFrame = 0;
			m_AnimationPtn ++;
			if (m_AnimationPtn >= 4) {
				m_AnimationPtn = 3;
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
		// X�e����
		m_pRayFactory->CreateXRay(m_Pos, m_pPlayer->pos);
		m_MoveCount++;
		break;
	case 2:
		// �ҋ@
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
		// ���ֈړ�
		m_AnimationPtn = 0;
		m_Muki -= 2;
		D3DXVECTOR2 pos = D3DXVECTOR2(1300.0f, 300.0f);
		SetMove(m_Pos, pos, m_State, m_Muki);
		break;
	}
	case 4:
		if (m_WaitFrame >= 60)
		{
			m_Muki += 2;
			m_WaitFrame = 0;
			m_MoveCount++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 5:
		if (m_WaitFrame >= 10)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 4) {
				m_AnimationPtn = 3;
				m_MoveCount++;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 6:
		// X�e����
		m_pRayFactory->CreateXRay(m_Pos, m_pPlayer->pos);
		m_MoveCount++;
	case 7:
		// �ҋ@
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
		// �n�ʂֈړ�
		m_AnimationPtn = 0;
		m_Muki -=2;
		SetMove(m_Pos, m_BeforeShotPos, m_State, m_Muki);
		break;
	case 9:
		// �ҋ@
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
		if (m_WaitFrame >= 10)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 4) {
				m_AnimationPtn = 3;
				m_MoveCount++;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case 1:
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
	case 2:
		m_BeforeShotPos = m_Pos;
		m_AnimationPtn = 0;
		// �}�b�v�O�ֈړ�
		SetMove(m_Pos, D3DXVECTOR2(SCREEN_WIDTH + m_Size.x / 2.0f, SCREEN_HEIGHT - m_Size.y / 2.0f - BLOCK_SIZE),m_State,m_Muki);
		break;
	case 3:
		// ���g���O�̍쐬
		for (int i = 0; i < 3; i++)
		{
			m_pEnemyFactory->Create_BossAvator(m_Pos, D3DXVECTOR2(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - (i*300.0f) - 250.0f),D3DXVECTOR2(6.0f,12.0f), Enemy::ENEMY_TYPE::FUFINAVATOR,0);
		}
		m_MoveCount++;
	case 4:
		// ���g��������܂őҋ@
		if (m_pEnemyFactory->CheckAliveFujinAvator()) {
			m_MoveCount++;
			m_Muki -= 4;
		}
		break;
	case 5:
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
	case 6:
		// �}�b�v���ֈړ�
		SetMove(m_Pos, m_BeforeShotPos, m_State, m_Muki);
		break;
	case 7:
		// �ҋ@
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
	m_AnimationPtn = 1;
	// �ړ���������A�j���[�V�����̌���������
	if (vec.x > 0)
	{
		m_Muki = 1;
	}
	else
	{
		m_Muki = 0;
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

void Boss_Fujin::AfterHitCheckBlockX(DWORD result)
{
	if (m_State == DEAD)
	{
		//�����蔻�菈��
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
	if (m_State == DEAD)
	{
		//���������邩�H����
		if ((result & HIT_UP) == 0 && m_IsGround == true)
		{
			m_IsGround = false;
		}

		//��������
		if (m_IsGround == false)
		{
			if (result & HIT_UP)
			{
				m_IsGround = true;
				m_Pos.y = GetBlockHeight() - (m_Size.y / 2);
				m_Vel.y = 0.0f;
			}
		}
		else // �ŏI�I�ɒn�ʂɐG��Ă���
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
