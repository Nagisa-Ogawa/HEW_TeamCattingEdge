#pragma once

#include "main.h"

class Explosion
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
	enum STATE_EXPLOSION
	{
		THROW,
		EXPLOSION,
	};
private:
	D3DXVECTOR2 m_Pos;


	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// ���e�̕\���T�C�Y
	int m_AnimationPtn = 0;		// ���e�̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// ���e�̃A�j���[�V�����̃J�E���^�[
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
	STATE_EXPLOSION m_State;
	int m_TextureNo = 0;
	int m_Damage = 0;

	// �����֘A
	float m_CollisionRad = 0.0f;	// ���݂̔����̓����蔻��p�~�̔��a
	float m_LastRad = 0.0f;			// �ŏI�I�Ȕ����̔��a
	int m_BlastFrame = 0;	// �����̗L���t���[��
	float m_ChangeBlastSize = 0.0f;	// ������1�t���[���ł̕ω��l

public:
	Explosion(D3DXVECTOR2 pos,D3DXVECTOR2 size,float collisionRad,float lastRad,int textureNo,int damage);
	// �����̏���������
	void Init();
	// �����̏I������
	void Uninit();
	// �����̍X�V����
	void Update();
	// �����̕`�揈��
	void Draw();

	// Get�n�֐�
	float GetCollsionRad() { return m_CollisionRad; };
	bool GetIsActive() { return m_IsActive; };
	int GetDamage() { return m_Damage; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	~Explosion();
};

