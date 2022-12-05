#pragma once
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DIVIDE_X 4	//���̕�����
#define DIVIDE_Y 2	//�c�̕�����

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)

#define ENEMY_SIZE (120)	//�@�v���C���[�̃T�C�Y

// �G�̃W�����v�p���[
#define ENEMY_JUMPPOWER_X (4.0f)	
#define ENEMY_JUMPPOWER_Y (-6.0f)
// �G�̃W�����v���̌�����
#define ENEMY_JUMPATTENUATION_X (0.2f)
#define ENEMY_JUMPATTENUATION_Y (0.2f)

#define ENEMY_ACTIVERADIUS (400.0f)	// �G�̒T�m�͈͂̔��a
#define ENEMY_WAITFRAME_SETUP (60)	// �U���������̑ҋ@�t���[����
#define ENEMY_WAITFRAME_WAIT (60)	// �U����̑ҋ@�t���[����
#define ENEMY_WAITFRAME_AFTERDROP (30) // �h���b�v��̑ҋ@�t���[����
#define ENEMY_DROPPOWER (0.98f)		// �h���b�v���̉����W��


enum STATE_ENEMY_HITDROP 
{
	IDLE,
	SETUP,
	JUMP,
	DROP,
	AFTERDROP,
	WAIT,
};

//*****************************************************************************
// �q�b�g�h���b�v����G�̃N���X
//*****************************************************************************
class Enemy_HitDrop :public Enemy
{
private:
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[4] =
	{
		0.0f,
		0.25f,
		0.50f,
		0.75
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//�E����
		0.5f,	//������
	};

	// �W�����v�n�ϐ�
	D3DXVECTOR2 m_JumpPower = D3DXVECTOR2(0.0f, 0.0f);	// �G�̃W�����v�̃p���[
	D3DXVECTOR2 m_JumpAttenuation = D3DXVECTOR2(0.0f, 0.0f);	// �W�����v���̌�����
	float m_DropPower = 0.0f;	// �h���b�v���̃p���[
	float m_DropPosX = 0.0f;	// �v���C���[���������W

	// �ҋ@�t���[��
	int m_WaitFrame = 0;
	STATE_ENEMY_HITDROP m_State = IDLE;
public:
	Enemy_HitDrop(D3DXVECTOR2 pos,int ID);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Enemy_HitDrop();
private:
	// ���N���X�ł����g�p���Ȃ��N���X
	void ChangeSetUp();	// �U���������̊֐�
	void Jump();	// �W�����v���̊֐�
	void LookPlayer(); // �v���C���[�̕����������֐�
};

