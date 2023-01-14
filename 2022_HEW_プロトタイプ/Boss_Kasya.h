#pragma once
#include "enemy.h"

class FireBallFactory;

class Boss_Kasya : public Enemy
{
public:
	enum STATE_BOSS_KASYA
	{
		IDLE,
		SETUP_MOVE,
		MOVE_LEFT_RIGHT,
		MOVE_UP_DOWN,
		MOVE,
		SETUP_THROW,
		THROW_ONESHOT,
		THROW_THREESHOT,
		WAIT,
		DEAD,
	};
private:
	const float m_AnimeTable[8] =
	{
		0.000f,
		0.125f,
		0.250f,
		0.375f,
		0.500f,
		0.625f,
		0.750f,
		0.875f,
	};
	const float M_MukiTable[8] =
	{
		0.000f,	// �ҋ@�i���j
		0.125f,	// �ҋ@�i�E�j
		0.250f,	// ����i���j
		0.375f,	// ����i�E�j
		0.500f,	// �u���X�i���j
		0.625f,	// �u���X�i�E�j
		0.750f,	// �ړ��i���j
		0.875f,	// �ړ��i�E�j
	};

	STATE_BOSS_KASYA m_State = IDLE;
	STATE_BOSS_KASYA m_BeforeState = IDLE;
	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	int m_AnimeFrame = 0;
	int m_IdleWaitFrame = 0;

	float m_LanePosXList[3]{ 0.0f,0.0f,0.0f };
	float m_LanePosYList[3]{ 0.0f,0.0f,0.0f };

	int m_StateCount = 0;

	// �ړ��U���p�ϐ��錾
	int m_MoveWaitFrame = 0;
	int m_MoveFrame = 0;
	bool m_IsDrop = false;
	int m_MoveCount = 0;
	D3DXVECTOR2 m_MoveSpeed = D3DXVECTOR2(0.0f, 0.0f);
	// �ړ��p�ϐ��錾
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_StartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_EndPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_MoveDistance = 0.0f;
	float m_NowDistance = 0.0f;

	// �΂̋ʍU���p�ϐ��錾
	FireBallFactory* m_pFireBallFactory = nullptr;
	int m_SetThrowWaitFrame = 0;
	int m_NowShotFireBall = 0;

public:
	Boss_Kasya(D3DXVECTOR2 pos, int ID, int textureNo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Boss_Kasya();

	void SetUp_Move();
	void Move_Up_Down();
	void Move_Left_Right();
	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, D3DXVECTOR2 moveVec);
	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos);
	void Move();
	void SetUp_Throw();
	void OneShot();
	void ThreeShot();
};

