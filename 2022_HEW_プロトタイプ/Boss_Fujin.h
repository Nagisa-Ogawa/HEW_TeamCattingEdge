#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

class ExplosionFactory;
struct PLAYER;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class Boss_Fujin:public Enemy
{
public:
	enum STATE_ENEMY_FUJIN
	{
		IDLE,		// �ҋ@
		ATTACK,		// �ߐڍU��
		INHALE,	// �z������
		BULLET_X,	// X�e
		WINDBLADE,	// ���̉�����
		AVATOR,		// ���g
		WIT,		// �ҋ@
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

	STATE_ENEMY_FUJIN m_State = IDLE;
	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	PLAYER* m_pPlayer = nullptr;
	// �ߐڍU���p�ϐ��錾
	int m_AttackTextureNo = -1;
	D3DXVECTOR2 m_AttackCollisionSize = D3DXVECTOR2(0.0f, 0.0f);
	// �z�����ݍU���p�ϐ��錾
	D3DXVECTOR2 m_InHalePower = D3DXVECTOR2(0.0f, 0.0f);
	float m_NowDistance = 0.0f;
	float m_AttackDistance = 0.0f;

public:
	Boss_Fujin(D3DXVECTOR2 pos, int ID, int textureNo);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;

	void InHale();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Boss_Fujin();
};

