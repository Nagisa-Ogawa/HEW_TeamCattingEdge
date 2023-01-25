#include "ShockWave.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"

ShockWave::ShockWave(D3DXVECTOR2 pos, D3DXVECTOR2 moveVec,int muki):m_MoveVec(moveVec)
{
	m_Size = D3DXVECTOR2(120.0f, 120.0f);
	m_Pos = D3DXVECTOR2(pos.x, pos.y - m_Size.y / 2);
	m_divid = D3DXVECTOR2(2.0f, 4.0f);
	m_pttern = D3DXVECTOR2(1.0f / m_divid.x, 1.0f / m_divid.y);
	m_IsActive = true;
	// �G�̃e�N�X�`����ǂݍ���
	m_TextureNo = LoadTexture((char*)"data/TEXTURE/WindBlade.png");
	m_pCamera = GetCamera();
	if (muki == 0)
	{
		m_Muki = 1;
	}
	else
	{
		m_Muki = 0;
	}
}

void ShockWave::Init()
{
}

void ShockWave::Uninit()
{
}

void ShockWave::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	// ���܂��������ɐi��
	m_Vel += m_MoveVec;
	m_Pos += m_Vel;
	// ��ʊO�Ȃ�pCamer
	if (m_Pos.x - m_pCamera->pos.x <= -500.0f || m_Pos.x - m_pCamera->pos.x >= SCREEN_WIDTH + 500.0f)
	{
		m_IsActive = false;
	}
	if (m_WaitFrame >= 10)
	{
		m_WaitFrame = 0;
		m_AnimationPtn++;
		if (m_AnimationPtn >= 2)
		{
			m_AnimationPtn = 0;
		}
	}
	else
	{
		m_WaitFrame++;
	}


	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

void ShockWave::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

ShockWave::~ShockWave()
{
}
