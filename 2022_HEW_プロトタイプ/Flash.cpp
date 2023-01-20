#include "Flash.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"

Flash::Flash(D3DXVECTOR2 pos, D3DXVECTOR2 size, int textureNo):
	m_Pos(pos), m_Size(size), m_TextureNo(textureNo)
{
	m_divid = D3DXVECTOR2(7.0f, 1.0f);
	m_pttern = D3DXVECTOR2(1.0f / m_divid.x, 1.0f / m_divid.y);
	m_IsActive = true;
}

void Flash::Init()
{
}

void Flash::Uninit()
{
}

void Flash::Update()
{
	if (!m_IsActive)
	{
		return;
	}
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

void Flash::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y,  m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[0], m_pttern.x, m_pttern.y);
}

Flash::~Flash()
{
}
