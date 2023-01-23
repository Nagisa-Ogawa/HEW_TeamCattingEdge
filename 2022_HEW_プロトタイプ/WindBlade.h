#pragma once
#include "main.h"

struct CAMERA_2D;

class WindBlade
{
public:
	enum STATE_WINDBLADE
	{
		IDLE,
		MOVE,
		DEAD,
	};
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[2] =
	{
		0.00f,
		0.50f,
	};
private:
	CAMERA_2D* m_pCamera = nullptr;
	int m_ID = -1;
	STATE_WINDBLADE m_State = IDLE;
	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_Vel;
	D3DXVECTOR2 m_Power = D3DXVECTOR2(0.0f, 0.0f);
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
	WindBlade(D3DXVECTOR2 pos,D3DXVECTOR2 size, int muki,int textureNo);
	// �΂̋ʂ̏���������
	void Init();
	// �΂̋ʂ̏I������
	void Uninit();
	// �΂̋ʂ̍X�V����
	void Update();
	// �΂̋ʂ̕`�揈��
	void Draw();
	~WindBlade();

	// Get�n�֐�
	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };

};

