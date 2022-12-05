#include "Enemy_HitDrop.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"
#include "Block.h"
#include "player.h"

Enemy_HitDrop::Enemy_HitDrop(D3DXVECTOR2 pos,int ID):Enemy(pos,ID)
{
	// �G�̃T�C�Y��ݒ�
	m_Size = D3DXVECTOR2(ENEMY_SIZE, ENEMY_SIZE);
	m_Gravity = 4.0f;
	m_DropPower = ENEMY_DROPPOWER;
}

void Enemy_HitDrop::Init()
{
	// �G�̃e�N�X�`����ǂݍ���
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

	// �u���b�N�ƓG�̓����蔻��
	switch (m_State)
	{
	
	case IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// �v���C���[���͈͓��ɓ������Ȃ�U��������
		if (len < ENEMY_ACTIVERADIUS)
		{
			ChangeSetUp();
		}
		m_Vel.y += m_Gravity;
		LookPlayer();
		break;
	}
	case SETUP:
		// �ҋ@
		if (m_WaitFrame >= ENEMY_WAITFRAME_SETUP)
		{
			m_State = JUMP;
			m_WaitFrame = 0;
			// �W�����v�A�j���[�V�����ɂ���
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
		// �W�����v����
		Jump();
		break;
	case DROP:
		m_Vel.y += m_DropPower;
		m_DropPower += ENEMY_DROPPOWER;
		if (m_IsGround)
		{
			// �����A�j���[�V������
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
			// �ҋ@�A�j���[�V�����ɂ���
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

	result = HitChackEnemy_Block(m_Pos,m_Size,m_Vel);

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

	m_Pos += m_Vel;

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);


	////�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	//if (m_AnimationCounter > 50)
	//{
	//	//�A�j���[�V�����p�^�[����؂�ւ���
	//	m_AnimationPtn++;
	//	//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
	//	if (m_AnimationPtn >= 4)
	//		m_AnimationPtn = 0;

	//	//�A�j���[�V�����J�E���^�[�̃��Z�b�g
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
	// �v���C���[�ƓG�̈ʒu�����ԕ����Ɨ����\����W������
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
	// �����A�j���[�V�����ɂ���
	m_AnimationPtn++;
}

void Enemy_HitDrop::Jump()
{
	// �W�����v
	m_Vel.x += m_JumpPower.x;
	m_Vel.y += m_JumpPower.y;
	// ����
	m_JumpPower.y += m_JumpAttenuation.y;
	// �ړ����Ƀv���C���[���������W�܂ŗ����Ȃ炻���ɗ���
	// �G�̌����ɂ���ĕς���
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


