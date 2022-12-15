#pragma once

#include "enemy.h"
#include "game.h"

class Boss_Tengu:public Enemy
{
public:
	enum STATE_BOSS_TENGU
	{
		IDLE,
		SETUP,
		JUMP,
		GLID,
		DROP,
		AFTERDROP,
		THROW,
		AFTERTHROW,
		WAIT,
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
	const float M_MukiTable[4] =
	{
		0.0f,	// �E�����̂�������U��
		0.25f,	// �������̂�������U��
		0.50f,	// �E�������e�����U��
		0.75f,	// ���������e�Ȃ��U��
	};

	STATE_BOSS_TENGU m_State = IDLE;

	// �W�����v�n
	float m_ActiveRad_Jump = 0.0f;
	D3DXVECTOR2 m_JumpPowerMax = D3DXVECTOR2(0.0f, 0.0f);	// �G�̃W�����v�̍ő�p���[
	D3DXVECTOR2 m_JumpPower = D3DXVECTOR2(0.0f, 0.0f);	// �G�̃W�����v�̃p���[
	D3DXVECTOR2 m_JumpAttenuation = D3DXVECTOR2(0.0f, 0.0f);	// �W�����v���̌�����
	float m_DropPower = 0.0f;	// �h���b�v���̃p���[
	float m_DropPosX = 0.0f;	// �v���C���[���������W
	float m_ChangeGlidPowerY = 0.0f;	// �W�����v���犊��ֈڍs����Ƃ��̊�l
	float m_ChangeDropPowerY = 0.0f;	// ���󂩂�h���b�v�ֈڍs����Ƃ��̊�l
	float m_AddDropPower = 0.0f;		// �����p���[�ɖ��t���[�������l

	// �Ռ��g�n
	ShockWaveFactory* m_pShockWaveFactory = nullptr;
	D3DXVECTOR2 m_ShockWavePower = D3DXVECTOR2(0.0f, 0.0f);

	// ���e�n
	BombFactory* m_pBombFactory = nullptr;
	float m_ActiveRad_Throw = 0.0f;	// ���e�𓊂��n�߂�͈�
	int m_WaitTime = 0;	// ���e�𓊂�����̑ҋ@����
	int m_CooldownTime = 0;	// ���e�𓊂��I�������̃N�[���^�C������
	float m_ThrowOffset = 0.0f;	// ���e�𓊂���Ƃ��̃{�X����̃I�t�Z�b�g
	float m_ThrowDistance = 0.0f;	// ���e���m�̊Ԋu
	// �ҋ@�t���[��
	int m_WaitFrame = 0;
public:
	Boss_Tengu(D3DXVECTOR2 pos, int ID);
	// �G�l�~�[�̏���������
	void Init() override;
	// �G�l�~�[�̏I������
	void Uninit() override;
	// �G�l�~�[�̍X�V����
	void Update() override;
	// �G�l�~�[�̕`�揈��
	void Draw() override;
	~Boss_Tengu();

	// �W�����v�̏���������֐�
	void ChangeSetUp();
	// �W�����v
	void Jump();
	// ����
	void Glid();
	// �Ռ��g�𔭐�������֐�
	void ShockWave();
	// ���e�𓊂���
	void Throw();
	// �v���C���[�̕��������֐�
	void LookPlayer();

};

