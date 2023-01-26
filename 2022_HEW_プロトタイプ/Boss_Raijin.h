#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

class ExplosionFactory;
class RayFactory;
class EnemyFactory;
class FlashFactory;
class ThunderBladeFactory;
class SwitchBulletFactory;
struct PLAYER;

class Boss_Raijin :public Enemy
{
public:
	enum STATE_ENEMY_RAIJIN
	{		
		SWITCH_BULLET,	// ����e�ƈʒu�����ւ�
		BULLET_T,		// �{�e
		THUNDERBLADE,	// ���̐n
		AVATOR,			// ���g
		IDLE,			// �ҋ@
		ATTACK,			// �ߐڍU��
		MOVE,
		WAIT,			// �ҋ@
		DEAD,			// ���S
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
		0.000f,
		0.125f,
		0.250f,
		0.375f,
		0.500f,
		0.625f,
		0.750f,
		0.875f,
	};

	STATE_ENEMY_RAIJIN m_State = IDLE;
	STATE_ENEMY_RAIJIN m_BeforeState = IDLE;
	STATE_ENEMY_RAIJIN m_LastTimeState = IDLE;

	// �ҋ@�t���[��
	int m_StateCount = 0;
	int m_WaitFrame = 0;
	PLAYER* m_pPlayer = nullptr;
	RayFactory* m_pRayFactory = nullptr;
	EnemyFactory* m_pEnemyFactory = nullptr;
	FlashFactory* m_pFlashFactory = nullptr;
	ThunderBladeFactory* m_pThunderFactory = nullptr;
	SwitchBulletFactory* m_pSwitchBulletFactory = nullptr;
	// �ߐڍU���p�ϐ��錾
	int m_AttackTextureNo = -1;
	D3DXVECTOR2 m_AttackCollisionSize = D3DXVECTOR2(0.0f, 0.0f);
	// �����p�ϐ��錾
	D3DXVECTOR2 m_ThunderBoltPos = D3DXVECTOR2(0.0f, 0.0f);
	// ���e�p�ϐ��錾
	D3DXVECTOR2 m_SwitchStartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_SwitchPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_TargetPos = D3DXVECTOR2(0.0f, 0.0f);
	// ���g�U���p�ϐ��錾
	D3DXVECTOR2 m_BeforeShotPos = D3DXVECTOR2(0.0f, 0.0f);
	// �ړ��p�ϐ��錾
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_StartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_EndPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_MoveDistance = 0.0f;
	int m_BeforeMuki = -1;
	int m_MoveCount = 0;
	float m_NowDistance = 0.0f;


public:
	Boss_Raijin(D3DXVECTOR2 pos, int ID, int textureNo,int muki,bool isDuo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;

	void SwitchBullet();
	void ShotBullet_T();
	void ThunderBlade();
	void Avator();

	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_RAIJIN state, int muki);
	void Move();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Boss_Raijin();
};