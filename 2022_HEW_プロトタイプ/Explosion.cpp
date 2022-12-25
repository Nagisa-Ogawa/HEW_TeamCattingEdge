#include "Explosion.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"

Explosion::Explosion(D3DXVECTOR2 pos,D3DXVECTOR2 size,float collisionRad, float lastRad,int textureNo):
			m_Pos(pos),m_Size(size),m_CollisionRad(collisionRad),m_LastRad(lastRad),m_TextureNo(textureNo)
{
	m_divid = D3DXVECTOR2(7.0f, 1.0f);
	m_pttern = D3DXVECTOR2(1.0f / m_divid.x, 1.0f / m_divid.y);
	m_IsActive = true;
	m_CollisionRad = m_Size.x / 2.0f;
	// �����̗L���t���[�����Z�b�g
	m_BlastFrame = 42;
	// 1�t���[���ɕω����锚���͈̔͂��v�Z
	m_ChangeBlastSize = (m_LastRad - m_CollisionRad) / (float)m_BlastFrame;
};

void Explosion::Init()
{
}

void Explosion::Uninit()
{
}

void Explosion::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	// �����̓����蔻��𑝂₷
	m_CollisionRad += m_ChangeBlastSize;
	//�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	if (m_AnimationCounter > 6)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		m_AnimationPtn++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (m_AnimationPtn >= m_divid.x)
		{
			m_IsActive = false;
		}
		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		m_AnimationCounter = 0;
	}
	m_AnimationCounter++;
}

void Explosion::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_CollisionRad*2, m_CollisionRad * 2,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[0], m_pttern.x, m_pttern.y);
}

Explosion::~Explosion()
{
}
