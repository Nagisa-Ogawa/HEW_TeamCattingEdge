#pragma once
#include "main.h"
#include "enemy.h"

class Enemy_Rush :public Enemy
{
public:
	enum STATE_ENEMY_RUSH
	{
		IDLE,		// �ʏ펞
		CHASE,		// �ǂ�
		WAIT,		// �ҋ@
		DEAD,		// ���S
	};
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
private:
	// �`�F�C�X�n�ϐ�
	float m_ChaseSpeed = 0.0f;	// �ǂ�������X�s�[�h
	float m_ActiveRange = 0.0f;	// �ǂ������n�߂�͈�
	D3DXVECTOR2 m_ChaseDirection = D3DXVECTOR2(0.0f, 0.0f);
	float m_DistanceMax = 0.0f;	// �ǂ������鋗���̍ő�l
	float m_NowDistance = 0.0f;	// ���݂̈ړ�����
	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	STATE_ENEMY_RUSH m_State = IDLE;
public:
	Enemy_Rush(D3DXVECTOR2 pos, int ID);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Enemy_Rush();
private:
	// ���N���X�ł����g�p���Ȃ��N���X
	void LookPlayer();
	void Chase();
};

