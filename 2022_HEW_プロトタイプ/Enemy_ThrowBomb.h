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
	};
private:
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//�E����
		0.5f,	//������
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
	Enemy_ThrowBomb(D3DXVECTOR2 pos, int ID);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Enemy_ThrowBomb();
	// ���e�𓊂���
	void Throw();	
	void LookPlayer();
};

