
#include "InstallationBomb.h"
#include "sprite.h"
#include "camera.h"
#include "texture.h"
#include "ExplosionFactory.h"
#include "texture.h"
#include "Block.h"
#include "game.h"
#include "ShockWaveFactory.h"
#include "BombFactory.h"
#include "sound.h"

static int	g_SE_bomb;		// SE�̎��ʎq

InstallationBomb::InstallationBomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos) : Bomb(ID,pos, endPos)
{
	m_NowFrame = 0;
	m_Gravity = 4.0f;
	// �G�̃e�N�X�`����ǂݍ���
	m_TextureNo = LoadTexture((char*)"data/TEXTURE/InstallationBomb.png");
	m_BombType = Bomb::INSTALLATION;
	m_pShockWaveFactory = GetShockWaveFactory();
	m_IsGround = false;

	//���֘A�̏�����
	g_SE_bomb = LoadSound((char*)"data/SE/Tengu_bomb.wav");
	SetVolume(g_SE_bomb, 0.3f);
}

void InstallationBomb::Init()
{
}

void InstallationBomb::Uninit()
{
}

void InstallationBomb::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();

	switch (m_State)
	{
	case InstallationBomb::THROW:
		Throw();
		// �����蔻���������ړ�
		if (m_NowFrame < m_ThrowFrame)
		{
			m_NowFrame++;
		}
		else
		{
			m_State = InstallationBomb::SET;
		}
		// ���e�ƒn�ʂ̓����蔻��
		if (CollisionBombToBlock())
		{
			m_State = InstallationBomb::SET;
		}
		break;
	case InstallationBomb::SET:
		// �n�ʂƂ̓����蔻��
		CollisionBombToShockWave();
		break;
	case InstallationBomb::EXPLOSION:
		// �������쐬
		m_pExplosionFactory->Create(m_Pos, D3DXVECTOR2(100.0f, 100.0f));
		PlaySound(g_SE_bomb, 0);

		m_IsActive = false;
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

	if(m_State==InstallationBomb::SET)
	{
		m_Vel.y += m_Gravity;
	}

	result = 0;
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

	if (m_State == InstallationBomb::THROW)
	{
		m_Pos = m_NextPos;
	}
	else if (m_State == InstallationBomb::SET)
	{
		m_Pos.y += m_Vel.y;
	}

	//�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	if (m_AnimationCounter > 10)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		m_AnimationPtn++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (m_AnimationPtn >= m_divid.x)
		{
			m_AnimationPtn = 0;
		}
		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		m_AnimationCounter = 0;
	}
	m_AnimationCounter++;

}

void InstallationBomb::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[0], m_pttern.x, m_pttern.y);
}

InstallationBomb::~InstallationBomb()
{
}

void InstallationBomb::CollisionBombToShockWave()
{
	for (ShockWave* shockWave : m_pShockWaveFactory->GetShockWaveList())
	{
		// �n�ʂɃZ�b�g����Ă��Ȃ��Ȃ�X�L�b�v
		if (m_State != STATE_INSTALLATIONBOMB::SET)
		{
			continue;
		}
		if (HitCheckSquare(shockWave->GetPos(), shockWave->GetSize(), m_Pos, m_Size))
		{
			// ���������Ȃ甚��������
			m_State = STATE_INSTALLATIONBOMB::EXPLOSION;
		}
	}
}

bool InstallationBomb::HitCheckSquare(D3DXVECTOR2 posA, D3DXVECTOR2 sizeA, D3DXVECTOR2 posB, D3DXVECTOR2 sizeB)
{
	float box1Xmin = posA.x - sizeA.x / 2;
	float box1Xmax = posA.x + sizeA.x / 2;
	float box1Ymin = posA.y - sizeA.y / 2;
	float box1Ymax = posA.y + sizeA.y / 2;

	float box2Xmin = posB.x - sizeB.x / 2;
	float box2Xmax = posB.x + sizeB.x / 2;
	float box2Ymin = posB.y - sizeB.y / 2;
	float box2Ymax = posB.y + sizeB.y / 2;

	if (box1Xmin < box2Xmax)
	{
		if (box1Xmax > box2Xmin)
		{
			if (box1Ymin < box2Ymax)
			{
				if (box1Ymax > box2Ymin)
				{
					return true;
				}
			}
		}
	}
	return false;

}
