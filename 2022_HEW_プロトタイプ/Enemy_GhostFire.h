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
	const float m_AnimeTable[13] =
	{
		0.00000f,
		0.07692f,
		0.15384f,
		0.23076f,
		0.30768f,
		0.38460f,
		0.46152f,
		0.53844f,
		0.61536f,
		0.69228f,
		0.76920f,
		0.84612f,
		0.92304f,
	};
	const float M_MukiTable[6] =
	{
		0.000000f,
		0.166667f,
		0.333334f,
		0.500000f,
		0.666667f,
		0.833334f
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
	int m_AnimeFrame = 0;


public:
	Enemy_GhostFire(D3DXVECTOR2 pos,int muki ,int ID, int textureNo);
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

