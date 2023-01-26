#pragma once
#include "main.h"


class DamageFloor
{
public:
	const float m_AnimeTable[4] =
	{
		0.0f,
		0.25f,
		0.50f,
		0.75f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,
		0.5f,	//�������ҋ@
	};
private:
	int m_ID = -99999;	// ���ʂ��邽�߂�ID
	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);	// �ʒu���W
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// �\���T�C�Y
	int m_AnimationPtn = 0;		// �A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// �A�j���[�V�����̃J�E���^�[
	int m_Muki = 0;
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;	// �\���t���O
	int m_TextureNo;		// �g�p����e�N�X�`���̔ԍ�
	int m_WaitFrame = 0;
public:
	DamageFloor(D3DXVECTOR2 pos, int textureNo);
	// �G�l�~�[�̏���������
	void Init();
	// �G�l�~�[�̏I������
	void Uninit();
	// �G�l�~�[�̍X�V����
	void Update();
	// �G�l�~�[�̕`�揈��
	void Draw();

	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	~DamageFloor();

};

