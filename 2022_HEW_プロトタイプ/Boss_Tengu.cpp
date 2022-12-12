#include "Boss_Tengu.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "Block.h"
#include "player.h"
#include "BombFactory.h"
#include "ShockWaveFactory.h"

#define TENGU_WAITFRAME_SETUP (120)
#define TENGU_WAITFRAME_AFTERDROP (20)
#define TENGU_WAITFRAME_WAIT (60)

Boss_Tengu::Boss_Tengu(D3DXVECTOR2 pos, int ID):Enemy(pos,ID,D3DXVECTOR2(480.0f,480.0f),D3DXVECTOR2(6.0f,4.0f))
{
	// �G�̃T�C�Y��ݒ�
	m_Gravity = 4.0f;
	m_HP = 10;
	// �q�b�g�h���b�v�n�ϐ�������
	m_DropPower = 0.98f;
	m_ActiveRad_Jump = 600.0f;
	m_JumpPowerMax = D3DXVECTOR2(5.0f, -15.0f);
	m_JumpAttenuation = D3DXVECTOR2(0.2f, 0.2f);
	m_ChangeGlidPowerY = -4.0f;
	m_ChangeDropPowerY = 3.0f;
	m_AddDropPower = 0.98f;
	// �Ռ��g�n�ϐ�������
	m_pShockWaveFactory = GetShockWaveFactory();
	m_ShockWavePower = D3DXVECTOR2(6.0f, 0.0f);
	// ���e�n�ϐ�������
	m_ActiveRad_Throw = 800.0f;
	m_pBombFactory = GetBombFactory();
	m_ThrowDistance = 200.0f;
	m_ThrowOffset = 300.0f;
}

void Boss_Tengu::Init()
{
	// �G�̃e�N�X�`����ǂݍ���
	m_EnemyTextureNo = LoadTexture((char*)"data/TEXTURE/Boss_Tengu.png");
}

void Boss_Tengu::Uninit()
{
}

void Boss_Tengu::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();

	switch (m_State)
	{
	case Boss_Tengu::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// �v���C���[���͈͓��ɓ������Ȃ�U��������
		if (len < m_ActiveRad_Jump&&m_IsGround)
		{
			ChangeSetUp();
			m_ActiveRad_Jump = 0.0f;
		}
		if (len < m_ActiveRad_Throw)
		{
			m_State = Boss_Tengu::THROW;
			m_ActiveRad_Throw = 0.0f;
		}
		break;
	}
	case Boss_Tengu::SETUP:
		// �ҋ@
		if (m_WaitFrame >= TENGU_WAITFRAME_SETUP)
		{
			m_State = Boss_Tengu::JUMP;
			m_WaitFrame = 0;
			// �W�����v�A�j���[�V�����ɂ���
			m_AnimationPtn++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Boss_Tengu::JUMP:
		// �W�����v����
		Jump();
		break;
	case Boss_Tengu::GLID:
		Glid();
	case Boss_Tengu::DROP:
		m_DropPower += m_AddDropPower;
		if (m_IsGround)
		{
			// �Ռ��g�����E�ɍ쐬
			ShockWave();
			// ������̑ҋ@�A�j���[�V������
			m_DropPower = m_AddDropPower;
			m_State = Boss_Tengu::AFTERDROP;
		}
		break;
	case Boss_Tengu::AFTERDROP:
		if (m_WaitFrame >= TENGU_WAITFRAME_AFTERDROP)
		{
			m_State = Boss_Tengu::WAIT;
			m_WaitFrame = 0;
			// �ҋ@�A�j���[�V�����ɂ���
			m_AnimationPtn = 0;
			m_ActiveRad_Throw = 800.0f;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Boss_Tengu::THROW:
		Throw();
		m_ActiveRad_Jump = 600.0f;
		break;
	case Boss_Tengu::WAIT:
		if (m_WaitFrame >= TENGU_WAITFRAME_WAIT)
		{
			m_State = Boss_Tengu::IDLE;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
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

	m_Pos.x += m_Vel.x;

	if (m_State == Boss_Tengu::JUMP||m_State==Boss_Tengu::GLID)
	{
		m_Vel.y += m_JumpPower.y;
	}
	else if (m_State == Boss_Tengu::DROP)
	{
		m_Vel.y += m_DropPower;
	}
	else
	{
		m_Vel.y += m_Gravity;
	}

	result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
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

	m_Pos.y += m_Vel.y;
	// �v���C���[�̕��������֐�
	LookPlayer();

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);

	////�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	//if (m_AnimationCounter > 20)
	//{
	//	//�A�j���[�V�����p�^�[����؂�ւ���
	//	m_AnimationPtn++;
	//	//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
	//	if (m_AnimationPtn >= m_divid.x)
	//	{
	//		m_AnimationPtn = 0;
	//	}
	//	//�A�j���[�V�����J�E���^�[�̃��Z�b�g
	//	m_AnimationCounter = 0;
	//}
	//m_AnimationCounter++;
}

void Boss_Tengu::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

Boss_Tengu::~Boss_Tengu()
{
}

void Boss_Tengu::ChangeSetUp()
{
	m_State = Boss_Tengu::SETUP;
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	// �v���C���[�ƓG�̈ʒu�����ԕ����Ɨ����\����W������
	if (pVec.x > 0)
	{
		m_JumpPower = m_JumpPowerMax;
		m_JumpPower.x *= -1.0f;
		m_JumpAttenuation.x *= -1.0f;
	}
	else
	{
		m_JumpPower = m_JumpPowerMax;
	}
	m_DropPosX = pPlayer->pos.x;
	// �����A�j���[�V�����ɂ���
	m_AnimationPtn++;
}

void Boss_Tengu::Jump()
{
	// �W�����v
	m_Vel.x += m_JumpPower.x;
	// ����
	m_JumpPower.y += m_JumpAttenuation.y;
	// �ړ����Ƀv���C���[���������W�܂ŗ����Ȃ炻���ɗ���
	// �G�̌����ɂ���ĕς���
	D3DXVECTOR2 enemyPos = m_Pos + m_Vel;
	if (m_Muki == 0)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn += 2;
		}
	}
	else
	{
		if (enemyPos.x <= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn += 2;
		}
	}
	// Y�����̈ړ��ʂ��キ�Ȃ�n�߂��犊���
	if (m_JumpPower.y >= m_ChangeGlidPowerY)
	{
		m_State = Boss_Tengu::GLID;
		m_AnimationPtn++;
	}
}

void Boss_Tengu::Glid()
{
	// �W�����v
	m_Vel.x += m_JumpPower.x;
	// ����
	m_JumpPower.y += m_JumpAttenuation.y;
	// �ړ����Ƀv���C���[���������W�܂ŗ����Ȃ炻���ɗ���
	// �G�̌����ɂ���ĕς���
	D3DXVECTOR2 enemyPos = m_Pos + m_Vel;
	if (m_Muki == 0)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn++;
		}
	}
	else
	{
		if (enemyPos.x <= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn++;
		}
	}
	// ���ɗ����n�߂���h���b�v��
	if (m_JumpPower.y >= m_ChangeDropPowerY)
	{
		m_State = Boss_Tengu::DROP;
		m_AnimationPtn++;
	}
}

void Boss_Tengu::ShockWave()
{
	// ��x�u���b�N�Ƃ̓����蔻����m�F
	HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
	// �G��Ă���u���b�N�̂�����ɏՌ��g�̈ʒu������
	D3DXVECTOR2 pos;
	pos.x = m_Pos.x;
	pos.y = GetBlockHeight();
	// ���E�ɏՌ��g���쐬
	m_pShockWaveFactory->Create(pos, m_ShockWavePower, 0);
	m_pShockWaveFactory->Create(pos, D3DXVECTOR2(m_ShockWavePower.x*-1, m_ShockWavePower.y), 1);
}

void Boss_Tengu::Throw()
{
	// ���e�𓊂���ꏊ������
	PLAYER* pPlayer = GetPlayer();
	// �n�_�ƏI�_�ƊJ�n�x�N�g���ƏI���x�N�g��������
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	// ��������ړ��x�N�g���̗ʂ�ω�
	D3DXVECTOR2 startVec = D3DXVECTOR2(0.0f, -800.0f);
	D3DXVECTOR2 endVec = D3DXVECTOR2(0.0f, 800.0f);
	float distance = m_ThrowDistance;
	float offset = m_ThrowOffset;
	if (pVec.x < 0)
	{
		startVec.x = -5.0f;
		endVec.x = -5.0f;
	}
	else
	{
		startVec.x = 5.0f;
		endVec.x = 5.0f;
		distance *= -1.0f;
		offset *= -1.0f;
	}
	// 3���e�𐶐�
	for (int i = 0; i < 3; i++)
	{
		m_pBombFactory->CreateInstallationBomb(m_Pos, D3DXVECTOR2(m_Pos.x+(i*distance)+offset , pPlayer->pos.y), startVec, endVec);
	}
	m_State = Boss_Tengu::WAIT;
}

void Boss_Tengu::LookPlayer()
{
	// ������ς��Ă͂����Ȃ���ԂȂ�ς��Ȃ�
	if (m_State== Boss_Tengu::JUMP || m_State == Boss_Tengu::GLID || m_State == Boss_Tengu::DROP||m_State==Boss_Tengu::SETUP)
	{
		return;
	}
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
