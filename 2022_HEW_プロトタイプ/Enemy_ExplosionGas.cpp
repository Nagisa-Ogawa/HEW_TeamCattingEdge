#include "Enemy_ExplosionGas.h"
#include "texture.h"
#include "camera.h"
#include "sprite.h"
#include "player.h"
#include "Block.h"
#include "game.h"
#include "ExplosionFactory.h"

Enemy_ExplosionGas::Enemy_ExplosionGas(D3DXVECTOR2 pos, int ID):
	Enemy(pos, ID, D3DXVECTOR2(120.0f, 120.0f), D3DXVECTOR2(1.0f, 1.0f))
{
	// �G�̃T�C�Y��ݒ�
	m_HP = 1;
	m_BombFrame = 100;
	m_BombRange = 300.0f;
	m_MaxSize = 30.0f;
	m_ChangeSizeValue = m_MaxSize / m_BombFrame;
	m_pExplosionFactory = GetExplosionFactory();
	m_DeadAnimeNum = 3;
	m_DeadAnimeFrame = 10;
}

void Enemy_ExplosionGas::Init()
{
	// �G�̃e�N�X�`����ǂݍ���
	m_EnemyTextureNo = LoadTexture((char*)"data/TEXTURE/ExplosionGas.png");
}

void Enemy_ExplosionGas::Uninit()
{
}

void Enemy_ExplosionGas::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();

	// ���S���Ă����Ȃ玀�S��Ԃ�
	if (m_IsDie) {
		m_IsDie = false;
		m_AnimationPtn = 0;
		m_Muki += 2;
		m_WaitFrame = 0;
		m_State = DEAD;
	}

	switch (m_State)
	{
	case Enemy_ExplosionGas::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// �v���C���[���͈͓��ɓ������Ȃ甚��
		if (len < m_BombRange)
		{
			m_State = Enemy_ExplosionGas::SETUP_BOMB;
		}
		break;
	}
	case Enemy_ExplosionGas::SETUP_BOMB:
		SetUp();
		break;
	case Enemy_ExplosionGas::BOMB:
		// ����
		// �������쐬
		m_pExplosionFactory->Create(m_Pos, D3DXVECTOR2(200.0f, 200.0f));
		m_IsDead = true;
		m_IsDie = true;
		// m_State = Enemy_SelfDestruct::IDLE;
		break;
	case Enemy_ExplosionGas::DEAD:
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
	m_Vel.y += m_Gravity;

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

	m_Pos += m_Vel;
	if (m_WaitAnimeFrame >= 10)
	{
		m_WaitAnimeFrame = 0;
		m_AnimationPtn++;
		if (m_AnimationPtn == 2) {
			m_AnimationPtn = 0;
		}
	}
	else
	{
		m_WaitAnimeFrame++;
	}
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);

}

void Enemy_ExplosionGas::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

Enemy_ExplosionGas::~Enemy_ExplosionGas()
{
}

void Enemy_ExplosionGas::SetUp()
{
	// �������邩�ǂ������`�F�b�N
	if (m_WaitFrame >= m_BombFrame)
	{
		// ������
		m_State = Enemy_ExplosionGas::BOMB;
		// �T�C�Y��߂�
		m_Size = D3DXVECTOR2(120.0f, 120.0f);
		m_WaitFrame = 0;
	}
	else
	{
		// �T�C�Y��ς���
		m_Size.x += m_ChangeSizeValue;
		m_Size.y += m_ChangeSizeValue;
		m_WaitFrame++;
	}
}
