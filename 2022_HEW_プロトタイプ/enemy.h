#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// #define MAX_ENEMY 100		//�G�̕\���ő吔


//*****************************************************************************
// �G�̊��N���X
//*****************************************************************************
class Enemy 
{
public:
	enum ENEMY_TYPE
	{
		HITDROP,
		SELFDESTRUCT,
		THROWBOMB,
		GHOSTFIRE,
		RUSH,
		EXPLOSIONGAS,
		FUFINAVATOR,
		BOSS_TENGU,
		BOSS_KASYA,
		BOSS_FUJIN,
		BOSS_RAIJIN,
	};
private:
	int m_ID = -99999;	// �G�����ʂ��邽�߂�ID
protected:
	ENEMY_TYPE m_EnemyType;
	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);	// �ʒu���W
	D3DXVECTOR2 m_OldPos = D3DXVECTOR2(0.0f, 0.0);	// 1�t���[���O�̈ʒu���W
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);	// ���x�x�N�g��
	D3DXVECTOR2 m_Dir = D3DXVECTOR2(0.0f, 0.0f);	// �ړ�����
	int m_Muki = -1;			// �G�������Ă������(�E�Ȃ�true,���Ȃ�false)
	float m_Speed = 0.0f;		// �ړ����x
	//bool m_IsUse = false;		// �g�p���Ă��邩�̃t���O
	int m_EnemyTextureNo;		// �G���g�p����e�N�X�`���̔ԍ�
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// �G�̕\���T�C�Y
	D3DXVECTOR2 m_CollisionSize = D3DXVECTOR2(0.0f, 0.0f); // �G�̓����蔻��T�C�Y
	int m_AnimationPtn = 0;		// �G�̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// �G�̃A�j���[�V�����̃J�E���^�[
	float m_Gravity = 0.98f;	// �d��
	bool m_IsGround = false;	// �G�̐ݒu����t���O
	int m_HP;
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;	// �\���t���O
	bool m_IsDie = false;		// ���S������1�t���[���̊Ԃ���true�ɂȂ�t���O
	bool m_IsDead = false;		// ���S�t���O
	int m_DeadAnimeNum = 0;		// ���S���A�j���[�V�����̖���
	int m_DeadAnimeFrame = 0;	// ���S���A�j���[�V�����̐؂�ւ��t���[����

	float m_BlockLength = 99999;//��ԋ߂��u���b�N�̋���
	int m_BlockIndex[2]{ -99999,-99999 };//1��ԋ߂��u���b�N�̓Y����
	float m_BlockHeight = 0.0f;
	bool m_IsEndSetUp = false;
	bool m_IsStealth = true;

public:
	Enemy(D3DXVECTOR2 pos, int ID, D3DXVECTOR2 size, D3DXVECTOR2 divid,int textureNo,ENEMY_TYPE enemyType) :
			m_Pos(pos), m_ID(ID), m_Size(size), m_divid(divid) ,m_EnemyTextureNo(textureNo),m_EnemyType(enemyType)
	{ 
		m_pttern.x = 1.0f / m_divid.x;
		m_pttern.y = 1.0f / m_divid.y;
		m_IsActive = true;
		m_IsGround = false;
		m_IsDead = false;
		m_CollisionSize = size;
	};
	// �G�l�~�[�̏���������
	virtual void Init() = 0;
	// �G�l�~�[�̏I������
	virtual void Uninit() = 0;
	// �G�l�~�[�̍X�V����
	virtual void Update() = 0;
	// �G�l�~�[�̕`�揈��
	virtual void Draw() = 0;

	// �G�l�~�[�̓����蔻���������̏���
	virtual void AfterHitCheckBlockX(DWORD result) = 0;
	virtual void AfterHitCheckBlockY(DWORD result) = 0;

	// �G�l�~�[�̐퓬�n�֐�
	// �v���C���[�̍U���Ƃ̓����蔻��֐�
	void HitCheckPlayerAttack(D3DXVECTOR2 AttackPos);
	// �_���[�W�֐�
	void Damege(int damage);
	// Get�n�֐�
	ENEMY_TYPE GetEnemyType() { return m_EnemyType; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	D3DXVECTOR2 GetCollisionSize() { return m_CollisionSize; };
	D3DXVECTOR2 GetVel() { return m_Vel; };
	bool GetIsActive() { return m_IsActive; };
	bool GetIsDead() { return m_IsDead; };
	int GetBlockIndexX() { return m_BlockIndex[0]; };
	int GetBlockIndexY() { return m_BlockIndex[1]; };
	float GetBlockLength() { return m_BlockLength; };
	float GetBlockHeight() { return m_BlockHeight; };
	bool GetIsEndSetUp() { return m_IsEndSetUp; };
	bool GetIsStealth() { return m_IsStealth; };

	// Set�n�֐�
	void SetBlockIndexX(int num) { m_BlockIndex[0] = num; };
	void SetBlockIndexY(int num) { m_BlockIndex[1] = num; };
	void SetBlockLength(float num) { m_BlockLength = num; };
	void SetBlockHeight(float num) { m_BlockHeight = num; };
	~Enemy() {};
};

//enum ENEMYSTATE
//{
//	chase = 0,
//	wait,
//	Return,
//	NUN_STATE
//};

//struct ENEMY
//{
//	D3DXVECTOR2 pos;
////	D3DXVECTOR2	vel;	//���x�x�N�g��
//	D3DXVECTOR2 dir;	//�ړ�����
//	float		speed;	//�ړ��X�s�[�h
//
//	D3DXVECTOR2 start_pos;
//
//	float		height;	//����
//	int			muki;	//����
//
//	ENEMYSTATE		state;  //��ԊǗ�
//
//	int			frame;	//�L���t���[��
//	int			use;	//���t���O
//};
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//void InitEnemy(void);
//void UninitEnemy(void);
//void UpdateEnemy(void);
//void DrawEnemy(void);
//
//void SetEnemy(void);
//
//ENEMY* GetEnemy(void);
