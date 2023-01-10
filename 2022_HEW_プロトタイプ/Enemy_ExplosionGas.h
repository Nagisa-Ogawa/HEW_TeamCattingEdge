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
class Enemy_ExplosionGas : public Enemy
{
public:
	enum STATE_ENEMY_EXPLOSIONGAS
	{
		IDLE,		// �ҋ@
		SETUP_BOMB,	// ��������
		BOMB,		// ����
		DEAD,		// ���S
	};
private:
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[7] =
	{
		0.0f,
		0.142857f,
		0.285714f,
		0.428571f,
		0.571428f,
		0.714285f,
		0.857142f
	};
	const float M_MukiTable[2] =
	{
		0.0f,
		0.5f,	//�������ҋ@
	};

	// �`�F�C�X�n�ϐ�
	float m_BombRange = 0.0f;	// �������n�߂�͈�
	// �����n�ϐ�
	int m_BombFrame = 0;	// ��������܂ł̃t���[��
	float m_MaxSize = 0.0f;	// �����O�̍ő�T�C�Y
	float m_ChangeSizeValue = 0.0f;// �����O�̃T�C�Y��1�t���[��������̕ω���
	ExplosionFactory* m_pExplosionFactory = nullptr;

	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	int m_WaitAnimeFrame = 0;
	STATE_ENEMY_EXPLOSIONGAS m_State = IDLE;
public:
	Enemy_ExplosionGas(D3DXVECTOR2 pos, int ID, int textureNo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Enemy_ExplosionGas();
private:
	// ���N���X�ł����g�p���Ȃ��N���X
	void SetUp();

};

