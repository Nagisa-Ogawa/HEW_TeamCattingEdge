#pragma once
#include "main.h"

struct CAMERA_2D;

class ThunderBlade
{
public:
	enum STATE_THUNDERBLADE
	{
		IDLE,
		THUNDERBOLT,
		DEAD,
	};
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[13] =
	{
		0.00000f,
		0.07692f,
		0.15384f,
		0.23076f,
		0.30768f,
		0.38460f,
		0.46152f,
		0.53844f,
		0.61536f,
		0.69228f,
		0.76920f,
		0.84612f,
		0.92304f,
	};
	const float M_MukiTable[1] =
	{
		0.00f,
	};
private:
	CAMERA_2D* m_pCamera = nullptr;
	int m_ID = -1;
	STATE_THUNDERBLADE m_State = IDLE;
	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_Vel;
	int m_TextureNo = -1;
	int m_WaitFrame = 0;
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// �΂̋ʂ̕\���T�C�Y
	D3DXVECTOR2 m_CollisionSize = D3DXVECTOR2(0.0f, 0.0f);
	int m_AnimationPtn = 0;		// �A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// �A�j���[�V�����̃J�E���^�[
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	int m_Muki = 0;
	bool m_IsActive = false;
	bool m_IsCanHit = false;
	int m_ThunderBoltFrame = 0;
	int m_HitStartFrame = 0;
public:
	ThunderBlade(D3DXVECTOR2 pos, D3DXVECTOR2 size, int textureNo);
	// �΂̋ʂ̏���������
	void Init();
	// �΂̋ʂ̏I������
	void Uninit();
	// �΂̋ʂ̍X�V����
	void Update();
	// �΂̋ʂ̕`�揈��
	void Draw();
	~ThunderBlade();

	void ThunderBolt();

	// Get�n�֐�
	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	D3DXVECTOR2 GetCollisionSize() { return m_CollisionSize; };
	bool GetIsCanHit() { return m_IsCanHit; };

};

