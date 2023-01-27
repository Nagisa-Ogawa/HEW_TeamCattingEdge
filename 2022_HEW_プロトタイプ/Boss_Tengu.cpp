#include "Boss_Tengu.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "Block.h"
#include "player.h"
#include "BombFactory.h"
#include "ShockWaveFactory.h"
#include "game.h"
#include "sound.h"

#define TENGU_WAITFRAME_SETUP (120)
#define TENGU_WAITFRAME_AFTERDROP (20)
#define TENGU_WAITFRAME_WAIT (60)
#define TENGU_WAITFRAME_AFTERTHROW (60)

static int	g_SE_throw;		// SE�̎��ʎq
static int	g_SE_jump;		// SE�̎��ʎq
static int	g_SE_tyakuti;		// SE�̎��ʎq
static int	g_SE_dead;	// SE�̎��ʎq

Boss_Tengu::Boss_Tengu(D3DXVECTOR2 pos, int ID, int textureNo) :
	Enemy(pos, ID, D3DXVECTOR2(480.0f, 480.0f), D3DXVECTOR2(6.0f, 6.0f), textureNo, ENEMY_TYPE::BOSS_TENGU)
{
	// �G�̃T�C�Y��ݒ�
	m_Gravity = 4.0f;
	m_HP = 5;
	m_CollisionSize = D3DXVECTOR2(300.0f, 480.0f);
	// �q�b�g�h���b�v�n�ϐ�������
	m_DropPower = 0.98f;
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
	m_DeadAnimeNum = 3;
	m_DeadAnimeFrame = 10;
}

void Boss_Tengu::Init()
{
	//���֘A�̏�����
	g_SE_jump = LoadSound((char*)"data/SE/Tengu_jump.wav");
	SetVolume(g_SE_jump, 0.5f);
	g_SE_throw = LoadSound((char*)"data/SE/Tengu_throw.wav");
	SetVolume(g_SE_throw, 0.5f);
	g_SE_tyakuti = LoadSound((char*)"data/SE/Tengu_Tyakuti.wav");
	SetVolume(g_SE_tyakuti, 0.5f);
}

void Boss_Tengu::Uninit()
{
	SetGameScene(GAMESCENE_PICTURE_STAGE02);
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

	// ���S���Ă����Ȃ玀�S��Ԃ�
	if (m_IsDie)
	{
		m_IsDie = false;
		m_AnimationPtn = 0;
		if (m_Muki % 2 == 0)
		{
			m_Muki = 4;
		}
		else
		{
			m_Muki = 5;
		}
		m_WaitFrame = 0;
		m_State = DEAD;
		PlaySound(g_SE_dead, 0);
	}

	switch (m_State)
	{
	case Boss_Tengu::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// �v���C���[���͈͓��ɓ������Ȃ�U��������
		if (len < m_ActiveRad_Throw&&m_BeforeState!=THROW_INSTALLATION)
		{
			m_State = Boss_Tengu::THROW_INSTALLATION;
			m_BeforeState = Boss_Tengu::THROW_INSTALLATION;
			m_ActiveRad_Throw = 0.0f;
		}
		else if (m_BeforeState!=JUMP&&m_IsGround)
		{
			m_BeforeState = Boss_Tengu::JUMP;
			ChangeSetUp();
			m_ActiveRad_Jump = 0.0f;
		}
		else if(m_BeforeState!=THROW_CONTACT)
		{
			m_State = Boss_Tengu::THROW_CONTACT;
			m_BeforeState = Boss_Tengu::THROW_CONTACT;
		}

		break;
	}
	case Boss_Tengu::SETUP:
		// �ҋ@
		if (m_WaitFrame >= TENGU_WAITFRAME_SETUP)
		{
			m_State = Boss_Tengu::JUMP;
			m_WaitFrame = 0;
			PlaySound(g_SE_jump, 0);
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
			//SE
			PlaySound(g_SE_tyakuti, 0);
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
	case Boss_Tengu::THROW_INSTALLATION:
		Throw_Installation();
		m_ActiveRad_Jump = 600.0f;
		break;
	case Boss_Tengu::THROW_CONTACT:
		Throw_Contact();
		break;
	case Boss_Tengu::AFTERTHROW:
		if (m_WaitFrame >= TENGU_WAITFRAME_AFTERTHROW)
		{
			m_Muki -= 2;
			m_AnimationPtn = 0;
			m_State = Boss_Tengu::WAIT;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
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
	case Boss_Tengu::DEAD:
		if (m_WaitFrame >= m_DeadAnimeFrame)
		{
			m_AnimationPtn++;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		if (m_AnimationPtn > m_DeadAnimeNum - 1)
		{
			m_IsActive = false;
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

void Boss_Tengu::Draw()
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

void Boss_Tengu::AfterHitCheckBlockX(DWORD result)
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

	m_Pos.x += m_Vel.x;

	if (m_State == Boss_Tengu::JUMP || m_State == Boss_Tengu::GLID)
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
}

void Boss_Tengu::AfterHitCheckBlockY(DWORD result)
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

	m_Pos.y += m_Vel.y;
	// �v���C���[�̕��������֐�
	LookPlayer();

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
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
	D3DXVECTOR2 pPos = GetPlayer()->pos;
	// �W�����v
	m_Vel.x += m_JumpPower.x;
	// ����
	m_JumpPower.y += m_JumpAttenuation.y;
	// �ړ����Ƀv���C���[���������W�܂ŗ����Ȃ炻���ɗ���
	// �ړ����Ƀv���C���[�������Ȃ痎��
	// �G�̌����ɂ���ĕς���
	D3DXVECTOR2 enemyPos = m_Pos + m_Vel;
	if (m_Muki == 1)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn += 2;
		}
		if (enemyPos.x >= pPos.x)
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
		if (enemyPos.x <= pPos.x)
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
	D3DXVECTOR2 pPos = GetPlayer()->pos;
	// �W�����v
	m_Vel.x += m_JumpPower.x;
	// ����
	m_JumpPower.y += m_JumpAttenuation.y;
	// �ړ����Ƀv���C���[���������W�܂ŗ����Ȃ炻���ɗ���
	// �G�̌����ɂ���ĕς���
	D3DXVECTOR2 enemyPos = m_Pos + m_Vel;
	if (m_Muki == 1)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn++;
		}
		if (enemyPos.x >= pPos.x)
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
			m_AnimationPtn++;
		}
		if (enemyPos.x <= pPos.x)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn += 2;
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

void Boss_Tengu::Throw_Installation()
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
		m_pBombFactory->CreateInstallationBomb(m_Pos, D3DXVECTOR2(m_Pos.x + (i*distance) + offset, pPlayer->pos.y));
	}
	// �����ɂ���ăA�j���[�V������ς���
	if (m_Muki == 0)
	{
		m_Muki = 2;
	}
	else
	{
		m_Muki = 3;
	}
	m_AnimationPtn = 1;
	//SE�����
	PlaySound(g_SE_throw, 0);
	m_State = Boss_Tengu::AFTERTHROW;
}

void Boss_Tengu::Throw_Contact()
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
	m_pBombFactory->CreateContactBomb_Boss(m_Pos, pPlayer->pos);
	// �����ɂ���ăA�j���[�V������ς���
	if (m_Muki == 0)
	{
		m_Muki = 2;
	}
	else
	{
		m_Muki = 3;
	}
	m_AnimationPtn = 1;
	//SE�����
	PlaySound(g_SE_throw, 0);
	m_State = Boss_Tengu::AFTERTHROW;
}

void Boss_Tengu::LookPlayer()
{
	// ������ς��Ă͂����Ȃ���ԂȂ�ς��Ȃ�
	if (m_State == Boss_Tengu::JUMP || m_State == Boss_Tengu::GLID || m_State == Boss_Tengu::DROP ||
		m_State == Boss_Tengu::SETUP || m_State == Boss_Tengu::THROW_INSTALLATION || m_State == Boss_Tengu::AFTERTHROW || m_State == Boss_Tengu::DEAD)
	{
		return;
	}
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	if (pVec.x > 0)
	{
		m_Muki = 0;
	}
	else
	{
		m_Muki = 1;
	}
}
