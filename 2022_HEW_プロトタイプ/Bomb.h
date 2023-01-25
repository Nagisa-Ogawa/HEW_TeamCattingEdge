#pragma once
#include "main.h"
#include "player.h"

class ExplosionFactory;

class Bomb
{
public:
	// �A�j���[�V������UV�l�̎w��Ɏg���z��
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[1] =
	{
		0.0f,	//�E����
	};
	enum BOMB_TYPE
	{
		CONTACT,
		INSTALLATION,
		BOSS_CONTACT,
	};
protected:
	int m_ID = -1;

	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_StartPos;
	D3DXVECTOR2 m_EndPos;
	D3DXVECTOR2 m_StartVec;
	D3DXVECTOR2 m_EndVec;
	D3DXVECTOR2 m_NextPos;

	D3DXVECTOR2 m_ThrowPower = D3DXVECTOR2(0.0f, 0.0f);

	BOMB_TYPE m_BombType;	// ���e�̎��

	int	m_ThrowFrame;	// ���e�̈ړ��t���[��

	PLAYER* m_pPlayer = nullptr;	// �v���C���[�̃|�C���^
	ExplosionFactory* m_pExplosionFactory = nullptr;

	float m_CollisionRad = 0.0f;	// ���e�̓����蔻��p�̔��a

	int m_NowFrame;
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// ���e�̕\���T�C�Y
	int m_AnimationPtn = 0;		// ���e�̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// ���e�̃A�j���[�V�����̃J�E���^�[
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
public:
	Bomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos,BOMB_TYPE type);
	// ���e�̏���������
	virtual void Init() = 0;
	// ���e�̏I������
	virtual void Uninit() = 0;
	// ���e�̍X�V����
	virtual void Update() = 0;
	// ���e�̕`�揈��
	virtual void Draw() = 0;
	~Bomb();

	// Get�n�֐�
	BOMB_TYPE GetBombType() { return m_BombType; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	int GetID() { return m_ID; };
	bool GetIsActive() { return m_IsActive; };

	// �������ɔ��e�𓊂���֐�
	void Throw();
	// �~�Ɖ~�̓����蔻��
	bool HitCheckCircle();
	// �l�p�Ǝl�p�̓����蔻��
	bool HitCheckBlock(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize);
	// �u���b�N�Ƃ̓����蔻��
	bool CollisionBombToBlock();

};

