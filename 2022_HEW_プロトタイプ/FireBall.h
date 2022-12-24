#pragma once

#include "main.h"

class FireBallFactory;

class FireBall
{
public:
	enum FIRE
	{
		MOVE,
		BURNSOUT,
	};
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[1] =
	{
		0.0f,
	};
	const float M_MukiTable[1] =
	{
		0.0f,	//�E����
	};

private:
	int m_ID = -1;
	FIRE m_State = MOVE;

	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_StartPos;
	D3DXVECTOR2 m_Vel;
	D3DXVECTOR2 m_ThrowPower = D3DXVECTOR2(0.0f, 0.0f);
	FireBallFactory* m_pFireBallFactory = nullptr;
	float m_DistanceMax = 0.0f;

	int m_TextureNo = -1;
	int m_WaitFrame = 0;
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// �΂̋ʂ̕\���T�C�Y
	int m_AnimationPtn = 0;		// �΂̋ʂ̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// �΂̋ʂ̃A�j���[�V�����̃J�E���^�[
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	int m_Muki = 0;
	bool m_IsActive = false;

public:
	FireBall(int ID,D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power);
	// �΂̋ʂ̏���������
	void Init();
	// �΂̋ʂ̏I������
	void Uninit();
	// �΂̋ʂ̍X�V����
	void Update();
	// �΂̋ʂ̕`�揈��
	void Draw();
	~FireBall();

	// Get�n�֐�
	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };

	void Move();
	void Hit();
};

