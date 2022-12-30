#pragma once
#include "main.h"
#include <list>
#include "player.h"

class FireBall;

class FireBallFactory
{
private:
	std::list<FireBall*> m_pFireBallList;
	PLAYER* m_pPlayer = nullptr;
	int m_nowID = -1;
public:
	FireBallFactory();
	// �΂̋ʂ��쐬
	void Create(D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power,int mode);
	void Init();
	// �΂̋ʍH��̏I������
	void Uninit();
	// �΂̋ʍH��̍X�V����
	void Update();
	// �΂̋ʍH��̕`�揈��
	void Draw();
	// �΂̋ʍH��̃|�C���^��n��
	~FireBallFactory();

	// �w�肳�ꂽ�΂̋ʂ����X�g����폜����
	void DeleteFireBall();

	// �����蔻��n�֐�
	// �v���C���[�Ɖ΂̋ʂ̓����蔻��(�v���C���[�ւ̃_���[�W)
	void CollisionPlayerToFireBall();
	// �ǂƉ΂̋ʂ̓����蔻��
	void CollisionBlockToFireBall();
	bool HitCheckBox(D3DXVECTOR2 fireBallPos, D3DXVECTOR2 fireBallSize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
	bool HitCheckBox_Block(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
};

