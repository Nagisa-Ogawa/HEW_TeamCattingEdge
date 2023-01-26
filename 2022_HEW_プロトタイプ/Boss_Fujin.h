#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

class ExplosionFactory;
class WindBladeFactory;
class RayFactory;
class FlashFactory;
class EnemyFactory;
struct PLAYER;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class Boss_Fujin:public Enemy
{
public:
	enum STATE_ENEMY_FUJIN
	{
		INHALE,		// �z������
		BULLET_X,	// X�e
		WINDBLADE,	// ���̐n
		AVATOR,		// ���g
		IDLE,		// �ҋ@
		ATTACK,		// �ߐڍU��
		MOVE,
		WAIT,		// �ҋ@
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
	const float M_MukiTable[12] =
	{
		0.000000f,	// �ړ��i���j
		0.083333f,	// �ړ��i�E�j
		0.166666f,	// �ߐځi���j
		0.249999f,	// �ߐځi�E�j
		0.333332f,	// ����i�E�j
		0.416665f,	// �u���X�i���j
		0.500000f,	// �u���X�i�E�j
		0.583333f,	// �ړ��i���j
		0.666666f,	// �ړ��i�E�j
		0.749999f,	// ��~�i���j
		0.833332f,	// ��~�i�E�j
		0.916665f,
	};

	STATE_ENEMY_FUJIN m_State = IDLE;
	STATE_ENEMY_FUJIN m_BeforeState = IDLE;
	STATE_ENEMY_FUJIN m_LastTimeState = IDLE;
	int m_StateCount = 0;

	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	int m_AnimeFrame = 0;
	PLAYER* m_pPlayer = nullptr;
	WindBladeFactory* m_pWindBladeFactory = nullptr;
	FlashFactory* m_pFlashFactory = nullptr;
	EnemyFactory* m_pEnemyFactory = nullptr;
	RayFactory* m_pRayFactory = nullptr;
	// �ߐڍU���p�ϐ��錾
	int m_AttackTextureNo = -1;
	D3DXVECTOR2 m_AttackCollisionSize = D3DXVECTOR2(0.0f, 0.0f);
	// �z�����ݍU���p�ϐ��錾
	D3DXVECTOR2 m_InHalePower = D3DXVECTOR2(0.0f, 0.0f);
	float m_NowDistance = 0.0f;
	float m_AttackDistance = 0.0f;
	// X�e�U���p�ϐ��錾
	D3DXVECTOR2 m_BeforeShotPos = D3DXVECTOR2(0.0f, 0.0f);
	// �ړ��p�ϐ��錾
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_StartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_EndPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_MoveDistance = 0.0f;
	int m_BeforeMuki = -1;
	int m_MoveCount = 0;

public:
	Boss_Fujin(D3DXVECTOR2 pos, int ID, int textureNo,bool isDuo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;

	void InHale();
	void ShotBullet_X();
	void Avator();

	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJIN state,int muki);
	void Move();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Boss_Fujin();
};

