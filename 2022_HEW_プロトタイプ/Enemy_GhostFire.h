#pragma once

#include "main.h"
#include "enemy.h"
#include "game.h"
#include "FireBallFactory.h"

class Enemy_GhostFire : public Enemy
{
public:
	enum STATE_ENEMY_GHOSTFIRE
	{
		IDLE,
		THROW,
		WAIT,
		DEAD,
	};
private:
	const float m_AnimeTable[1] =
	{
		0.0f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//������
		0.5f,
	};

	STATE_ENEMY_GHOSTFIRE m_State = IDLE;
	FireBallFactory* m_pFireBallFactory = nullptr;

	// �΂̋ʌn�ϐ�
	int m_WaitIdleTime = 0;	// �΂̋ʂ𓊂���O�̑ҋ@����
	int m_WaitTime = 0;	// �΂̋ʂ𓊂�����̑ҋ@����
	int m_WaitNextThrowTime = 0; // �񔭖ڂ̉΂̋ʂ𓊂���܂ł̑ҋ@����
	bool m_IsThrow = true;
	int m_ThrowNum = 0;
	// �ҋ@�t���[��
	int m_WaitFrame = 0;


public:
	Enemy_GhostFire(D3DXVECTOR2 pos,int muki ,int ID);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Enemy_GhostFire();
	// �΂̋ʂ𓊂���
	void Throw();
};

