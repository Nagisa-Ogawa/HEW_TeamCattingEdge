#pragma once

#include "main.h"

struct CAMERA_2D;

class ShockWave
{
public:
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[4] =
	{
		0.0f,	//�E����
		0.25f,
		0.50f,
		0.75f,
	};
	enum STATE_SHOCKWAVE
	{
		MOVE,
	};
private:

	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	int m_Muki = -1;	// �Ռ��g�̌���
	CAMERA_2D* m_pCamera = nullptr;

	int m_TextureNo = -1;	// ���e���g�p����e�N�X�`���̔ԍ�

	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// ���e�̕\���T�C�Y
	int m_AnimationPtn = 0;		// ���e�̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// ���e�̃A�j���[�V�����̃J�E���^�[
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
	int m_WaitFrame = 0;

	STATE_SHOCKWAVE m_State = STATE_SHOCKWAVE::MOVE;
public:
	ShockWave(D3DXVECTOR2 pos,D3DXVECTOR2 moveVec,int muki);
	// ���e�̏���������
	void Init();
	// ���e�̏I������
	void Uninit();
	// ���e�̍X�V����
	void Update();
	// ���e�̕`�揈��
	void Draw();
	~ShockWave();

	// Get�n�֐�
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	bool GetIsActive() { return m_IsActive; };
};

