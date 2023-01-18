#pragma once

#include "main.h"
#include "enemy.h"
#include "game.h"
#include "BombFactory.h"

class Enemy_ThrowBomb : public Enemy
{
public:
	enum STATE_ENEMY_THROWBOMB
	{
		IDLE,
		THROW,
		WAIT,
		COOLDOWN,
		DEAD,
	};
protected:
	const float m_AnimeTable[3] =
	{
		0.0f,
		0.333f,
		0.666f,
	};
	const float M_MukiTable[4] =
	{
		0.00f,	//������
		0.25f,	//�E����
		0.50f,	//���S��������
		0.75f,	//���S���E����
	};

	BombFactory* m_pBombFactory = nullptr;
	STATE_ENEMY_THROWBOMB m_State = IDLE;

	// ���e�n
	float m_ActiveRange = 0.0f;	// ���e�𓊂��n�߂�͈�
	int m_WaitTime = 0;	// ���e�𓊂�����̑ҋ@����
	int m_CooldownTime = 0;	// ���e�𓊂��I�������̃N�[���^�C������
	D3DXVECTOR2 m_ThrowPower;	// ����1�ɂ�������p���[

	// �ҋ@�t���[��
	int m_WaitFrame = 0;
public:
	Enemy_ThrowBomb(D3DXVECTOR2 pos, int ID, int textureNo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	virtual void Uninit() override;
	// �G�l�~�[�̍X�V����
	virtual void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Enemy_ThrowBomb();
	// ���e�𓊂���
	void Throw();	
	void LookPlayer();
};

