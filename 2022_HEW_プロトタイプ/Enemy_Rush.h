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
	const float m_AnimeTable[9] =
	{
		0.00000f,
		0.11111f,
		0.22222f,
		0.33333f,
		0.44444f,
		0.55555f,
		0.66666f,
		0.77777f,
		0.88888f,
	};
	const float M_MukiTable[4] =
	{
		0.00f,	//�������ҋ@
		0.25f,
		0.50f,
		0.75f,
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
	int m_AnimeFrame = 0;
	STATE_ENEMY_RUSH m_State = IDLE;
public:
	Enemy_Rush(D3DXVECTOR2 pos, int ID, int textureNo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Enemy_Rush();
private:
	// ���N���X�ł����g�p���Ȃ��N���X
	void LookPlayer();
	void Chase();
};

