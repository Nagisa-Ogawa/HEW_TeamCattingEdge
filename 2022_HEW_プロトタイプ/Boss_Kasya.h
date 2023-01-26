#pragma once
#include "enemy.h"
#include "player.h"

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
		STOP,
		SETUP_THROW,
		THROW_ONESHOT,
		THROW_THREESHOT,
		WAIT,
		DEAD,
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

	STATE_BOSS_KASYA m_State = IDLE;
	STATE_BOSS_KASYA m_BeforeState = IDLE;

	STATE_BOSS_KASYA m_LastTimeState = IDLE;
	STATE_BOSS_KASYA m_LastTimeState_Move = IDLE;
	STATE_BOSS_KASYA m_LastTimeState_Shot = IDLE;
	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	int m_AnimeFrame = 0;
	int m_IdleWaitFrame = 0;

	float m_LanePosXList[3]{ 0.0f,0.0f,0.0f };
	float m_LanePosYList[3]{ 0.0f,0.0f,0.0f };

	int m_StateCount = 0;
	PLAYER* m_pPlayer = nullptr;

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
	bool m_IsStop = false;
	int m_BeforeMuki = -1;
	// �΂̋ʍU���p�ϐ��錾
	FireBallFactory* m_pFireBallFactory = nullptr;
	int m_SetThrowWaitFrame = 0;
	int m_NowShotFireBall = 0;
	float m_OneShotDirection = 0.0f;

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

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Boss_Kasya();

	void SetUp_Move();
	void Move_Up_Down();
	void Move_Left_Right();
	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, D3DXVECTOR2 moveVec);
	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos);
	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, D3DXVECTOR2 moveVec, bool isRight);
	void Move();
	void Stop();
	void SetUp_Throw();
	void OneShot();
	void ThreeShot();
};

