#pragma once

#include "main.h"

class Flash
{
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[7] =
	{
		0.0f,
		0.142857f,
		0.285714f,
		0.428571f,
		0.571428f,
		0.714285f,
		0.857142f
	};
	const float M_MukiTable[1] =
	{
		0.0f,	//�E����
	};
	enum STATE_FLASH
	{
		THROW,
		FLASH,
	};
private:
	D3DXVECTOR2 m_Pos;


	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// ���e�̕\���T�C�Y
	int m_AnimationPtn = 0;		// ���e�̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// ���e�̃A�j���[�V�����̃J�E���^�[
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
	STATE_FLASH m_State;
	int m_TextureNo = 0;


public:
	Flash(D3DXVECTOR2 pos, D3DXVECTOR2 size, int textureNo);
	//����������
	void Init();
	// �I������
	void Uninit();
	// �X�V����
	void Update();
	// �`�揈��
	void Draw();

	// Get�n�֐�
	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	~Flash();
};
