#pragma once
#include "enemy.h"

class FireBallFactory;

class Boss_Kasya : public Enemy
{
public:
	enum STATE_BOSS_KASYA
	{
		IDLE,
		SETUP_ATTACK,
		ATTACK,
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
	const float m_AnimeTable[6] =
	{
		0.000000f,
		0.166667f,
		0.333334f,
		0.500000f,
		0.666667f,
		0.833334f
	};
	const float M_MukiTable[6] =
	{
		0.000000f,	// �E�����̂�������
		0.166667f,	// �������̂�������
		0.333334f,	// �E�������e����
		0.500000f,	// ���������e����
		0.666667f,	// �E�������S
		0.833334f,	// ���������S
	};

	STATE_BOSS_KASYA m_State = IDLE;
	STATE_BOSS_KASYA m_BeforeState = IDLE;
	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	int m_IdleWaitFrame = 0;

	float m_LanePosXList[3]{ 0.0f,0.0f,0.0f };
	float m_LanePosYList[3]{ 0.0f,0.0f,0.0f };

	int m_StateCount = 0;

	// �U���p�ϐ��錾
	int m_AttackWaitFrame = 0;
	int m_AttackFrame = 0;
	int m_IsAttack = 0;
	int m_AttackTextureNo = -1;
	D3DXVECTOR2 m_CollisionSize = D3DXVECTOR2(0.0f, 0.0f);

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

	void SetUp_Attack();
	void Attack();
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

