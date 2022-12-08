
#include <vector>
#include "Bomb.h"
#include "sprite.h"
#include "camera.h"


void Bomb::Init()
{
}

void Bomb::Uninit()
{
}

void Bomb::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	switch (m_State)
	{
	case Bomb::THROW:
		Throw();
		// �����蔻���������ړ�
		if (m_NowFrame < m_ThrowFrame)
		{
			m_NowFrame++;
		}
		else
		{
			m_State = Bomb::EXPLOSION;
			m_IsActive = false;
		}
		// ���e�ƒn�ʂ̓����蔻��
		CollisionBombToBlock();
		// ���e�ƃv���C���[�̓����蔻��
		if (HitCheckCircle())
		{
			m_State = Bomb::EXPLOSION;
			m_IsActive = false;
			PlayerDamage(10);
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
	case Bomb::EXPLOSION:
		// �����̃A�j���[�V����
		// �����̃T�C�Y��ύX
		// �����ƃv���C���[�̓����蔻��
		break;
	default:
		break;
	}
	m_Pos = m_NextPos;
}

void Bomb::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[0], m_pttern.x, m_pttern.y);
}

Bomb::~Bomb()
{
}

void Bomb::Throw()
{
	double t = (double)m_NowFrame / (double)m_ThrowFrame;	// ���݂̌o�ߊ���
	// �G���~�[�g�Ȑ����쐬
	m_NextPos.x = (2.0f*m_StartPos.x + (-2.0f)*m_EndPos.x + m_StartVec.x + m_EndVec.x)*(t*t*t) +
					((-3.0f)*m_StartPos.x + 3.0f*m_EndPos.x + (-2.0f)*m_StartVec.x + (-1.0f)*m_EndVec.x)*(t*t) +
						(m_StartVec.x*t) + m_StartPos.x;
	m_NextPos.y = (2.0f*m_StartPos.y + (-2.0f)*m_EndPos.y + m_StartVec.y + m_EndVec.y)*(t*t*t) +
					((-3.0f)*m_StartPos.y + 3.0f*m_EndPos.y + (-2.0f)*m_StartVec.y + (-1.0f)*m_EndVec.y)*(t*t) +
						(m_StartVec.y*t) + m_StartPos.y;
}


bool Bomb::HitCheckCircle()
{
	// �v���C���[�Ɣ��e�̃x�N�g�����v�Z
	D3DXVECTOR2 vec = m_Pos - m_pPlayer->pos;
	// �x�N�g���̒������v�Z
	float len = D3DXVec2Length(&vec);
	// �v���C���[�Ɣ��e�̓����蔻��̔��a�𑫂������l����
	// �x�N�g���̕����Z���Ȃ瓖�����Ă���
	if (len <= m_CollisionRad + m_pPlayer->size / 2.0f)
	{
		return true;
	}
	return false;
}

bool Bomb::HitCheckBlock(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize)
{
	float box1Xmin = m_Pos.x - m_Size.x / 2;
	float box1Xmax = m_Pos.x + m_Size.x/2;
	float box1Ymin = m_Pos.y - m_Size.y / 2;
	float box1Ymax = m_Pos.y + m_Size.y / 2;

	float box2Xmin = blockPos.x;
	float box2Xmax = blockPos.x + blockSize.x;
	float box2Ymin = blockPos.y;
	float box2Ymax = blockPos.y + blockSize.y;

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

void Bomb::CollisionBombToBlock()
{
	std::vector<std::vector<int>> blocks = GetPPBlocks();
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (blocks[y][x] == 0)
			{
				continue;
			}
			if (HitCheckBlock(D3DXVECTOR2(x*BLOCK_SIZE, y*BLOCK_SIZE), D3DXVECTOR2(BLOCK_SIZE, BLOCK_SIZE)))
			{
				m_State = Bomb::EXPLOSION;
				m_IsActive = false;
			}
		}
	}
}
