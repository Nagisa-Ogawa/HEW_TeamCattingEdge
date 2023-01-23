#pragma once
#include "enemy.h"

class WindBladeFactory;
class EnemyFactory;

class Enemy_BossAvator :public Enemy
{
public:
	enum STATE_ENEMY_FUJINAVATOR
	{
		IDLE,		// �ҋ@
		WINDBLADE,	// ���̐n
		MOVE,
		DEAD,		// ���S
	};
private:
	const float m_AnimeTable[6] =
	{
		0.000000f,
		0.166667f,
		0.333334f,
		0.500000f,
		0.666667f,
		0.833334f
	};
	const float M_MukiTable[10] =
	{
		0.0f,	// �ҋ@�i���j
		0.1f,	// �ҋ@�i�E�j
		0.2f,	// ����i���j
		0.3f,	// ����i�E�j
		0.4f,	// �u���X�i���j
		0.5f,	// �u���X�i�E�j
		0.6f,	// �ړ��i���j
		0.7f,	// �ړ��i�E�j
		0.8f,	// ��~�i���j
		0.9f,	// ��~�i�E�j
	};

	STATE_ENEMY_FUJINAVATOR m_State = IDLE;
	STATE_ENEMY_FUJINAVATOR m_BeforeState = IDLE;
	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	WindBladeFactory* m_pWindBladeFactory = nullptr;
	EnemyFactory* m_pEnemyFactory = nullptr;
	// �ړ��p�ϐ��錾
	float m_NowDistance = 0.0f;
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_StartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_EndPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_MoveDistance = 0.0f;
	int m_BeforeMuki = -1;
	int m_MoveCount = 0;
	D3DXVECTOR2 m_TargetPos = D3DXVECTOR2(0.0f, 0.0f);
public:
	Enemy_BossAvator(D3DXVECTOR2 pos, int ID, int textureNo,D3DXVECTOR2 targetPos);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;

	void WindBlade();
	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki);
	void Move();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;


	~Enemy_BossAvator();

};

