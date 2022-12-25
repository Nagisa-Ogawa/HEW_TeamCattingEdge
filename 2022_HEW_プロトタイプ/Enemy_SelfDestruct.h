#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

class ExplosionFactory;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class Enemy_SelfDestruct:public Enemy
{
public:
	enum STATE_ENEMY_SELFDESTRUCT
	{
		IDLE,		// �ҋ@
		CHASE,		// �ǂ�
		SETUP_BOMB,	// ��������
		BOMB,		// ����
		DEAD,		// ���S
	};
private:
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[6] =
	{
		0.00000f,	//�������ҋ@
		0.16667f,	//�E�����ҋ@
		0.33334f,	//�������ڋ�
		0.50000f,	//�E�����ҋ@
		0.66667f,	//����������
		0.83334f,	//�E��������
	};

	// �`�F�C�X�n�ϐ�
	float m_ChaseSpeed = 0.0f;	// �ǂ�������X�s�[�h
	float m_ActiveRange = 0.0f;	// �ǂ������n�߂�͈�
	float m_BombRange = 0.0f;	// �������n�߂�͈�
	// �����n�ϐ�
	int m_BombFrame = 0;	// ��������܂ł̃t���[��
	float m_MaxSize = 0.0f;	// �����O�̍ő�T�C�Y
	float m_ChangeSizeValue = 0.0f;// �����O�̃T�C�Y��1�t���[��������̕ω���
	ExplosionFactory* m_pExplosionFactory = nullptr;

	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	int m_WaitAnimeFrame = 0;
	STATE_ENEMY_SELFDESTRUCT m_State = IDLE;
public:
	Enemy_SelfDestruct(D3DXVECTOR2 pos, int ID, int textureNo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Enemy_SelfDestruct();
private:
	// ���N���X�ł����g�p���Ȃ��N���X
	void LookPlayer();
	void SetUp();
	void Chase();
};


