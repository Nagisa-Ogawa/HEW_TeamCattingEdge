
#include "ContactBomb.h"
#include "sprite.h"
#include "camera.h"
#include "texture.h"
#include "ExplosionFactory.h"
#include "texture.h"

ContactBomb::ContactBomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos) : Bomb(ID,pos,endPos)
{
	m_NowFrame = 0;
	// �G�̃e�N�X�`����ǂݍ���
	m_TextureNo = LoadTexture((char*)"data/TEXTURE/bomb.png");
	m_BombType = Bomb::CONTACT;
};

void ContactBomb::Init()
{
}

void ContactBomb::Uninit()
{
}

void ContactBomb::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	switch (m_State)
	{
	case ContactBomb::THROW:
		Throw();
		// �����蔻���������ړ�
		if (m_NowFrame < m_ThrowFrame)
		{
			m_NowFrame++;
		}
		else
		{
			m_State = ContactBomb::EXPLOSION;
		}
		// ���e�ƒn�ʂ̓����蔻��
		if (CollisionBombToBlock())
		{
			m_State = ContactBomb::EXPLOSION;
		}
		// ���e�ƃv���C���[�̓����蔻��
		if (HitCheckCircle())
		{
			m_State = ContactBomb::EXPLOSION;
			PlayerDamage(1);
		}
		//�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
		if (m_AnimationCounter > 10)
		{
			//�A�j���[�V�����p�^�[����؂�ւ���
			m_AnimationPtn++;
			//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
			if (m_AnimationPtn >= m_divid.x)
				m_AnimationPtn = 0;

			//�A�j���[�V�����J�E���^�[�̃��Z�b�g
			m_AnimationCounter = 0;
		}
		m_AnimationCounter++;
		break;
	case ContactBomb::EXPLOSION:
		// �������쐬
		m_pExplosionFactory->Create(m_Pos, D3DXVECTOR2(100.0f, 100.0f));
		m_IsActive = false;
		break;
	default:
		break;
	}
	m_Pos = m_NextPos;
}

void ContactBomb::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[0], m_pttern.x, m_pttern.y);
}

ContactBomb::~ContactBomb()
{
}

