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
private:
	int m_ID = -99999;	// �G�����ʂ��邽�߂�ID
protected:
	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);	// �ʒu���W
	D3DXVECTOR2 m_OldPos = D3DXVECTOR2(0.0f, 0.0);	// 1�t���[���O�̈ʒu���W
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);	// ���x�x�N�g��
	D3DXVECTOR2 m_Dir = D3DXVECTOR2(0.0f, 0.0f);	// �ړ�����
	bool m_Muki = true;			// �G�������Ă������(�E�Ȃ�true,���Ȃ�false)
	float m_Speed = 0.0f;		// �ړ����x
	//bool m_IsUse = false;		// �g�p���Ă��邩�̃t���O
	int m_EnemyTextureNo;		// �G���g�p����e�N�X�`���̔ԍ�
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// �G�̕\���T�C�Y
	int m_AnimationPtn = 0;		// �G�̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// �G�̃A�j���[�V�����̃J�E���^�[
	float m_Gravity = 0.98f;	// �d��
	bool m_IsGround = false;	// �G�̐ݒu����t���O
	int m_HP;
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
public:
	Enemy(D3DXVECTOR2 pos, int ID, D3DXVECTOR2 size, D3DXVECTOR2 divid) :
			m_Pos(pos), m_ID(ID), m_Size(size), m_divid(divid) 
	{ 
		m_pttern.x = 1.0f / m_divid.x;
		m_pttern.y = 1.0f / m_divid.y;
		m_IsActive = true;
		m_IsGround = false;
	};
	// �G�l�~�[�̏���������
	virtual void Init() = 0;
	// �G�l�~�[�̏I������
	virtual void Uninit() = 0;
	// �G�l�~�[�̍X�V����
	virtual void Update() = 0;
	// �G�l�~�[�̕`�揈��
	virtual void Draw() = 0;

	// �G�l�~�[�̐퓬�n�֐�
	// �v���C���[�̍U���Ƃ̓����蔻��֐�
	void HitCheckPlayerAttack(D3DXVECTOR2 AttackPos);
	// �_���[�W�֐�
	void Damege(int damage);
	// Get�n�֐�
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	bool GetIsActive() { return m_IsActive; };
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
