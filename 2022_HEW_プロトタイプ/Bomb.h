#pragma once

#include "game.h"
// #include "main.h"
#include "player.h"

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
	enum STATE_BOMB
	{
		THROW,
		EXPLOSION,
	};
private:
	
	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_StartPos;
	D3DXVECTOR2 m_EndPos;
	D3DXVECTOR2 m_StartVec;
	D3DXVECTOR2 m_EndVec;
	D3DXVECTOR2 m_NextPos;

	PLAYER* m_pPlayer = nullptr;	// �v���C���[�̃|�C���^
	ExplosionFactory* m_pExplosionFactory = nullptr;

	float m_CollisionRad = 0.0f;	// ���e�̓����蔻��p�̔��a

	int	m_ThrowFrame;
	int m_NowFrame;
	int m_TextureNo;		// ���e���g�p����e�N�X�`���̔ԍ�
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// ���e�̕\���T�C�Y
	int m_AnimationPtn = 0;		// ���e�̃A�j���[�V�����̃p�^�[���ԍ�
	int m_AnimationCounter = 0;	// ���e�̃A�j���[�V�����̃J�E���^�[
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
	STATE_BOMB m_State = STATE_BOMB::THROW;
public:
	Bomb(D3DXVECTOR2 pos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	// ���e�̏���������
	void Init();
	// ���e�̏I������
	void Uninit();
	// ���e�̍X�V����
	void Update();
	// ���e�̕`�揈��
	void Draw();
	~Bomb();

	void Throw();
	bool HitCheckCircle();
	bool HitCheckBlock(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize);
	void CollisionBombToBlock();
};

