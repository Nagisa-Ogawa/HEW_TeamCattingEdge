#pragma once

#include <list>
#include "enemy.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY_HITDORP 10		//�G�̕\���ő吔

//*****************************************************************************
// �q�b�g�h���b�v����G�̃N���X
//*****************************************************************************
class EnemyFactory
{
private:
	std::list<Enemy*> m_pEnemyList;
	PLAYER* m_pPlayer = nullptr;
	int m_nowID = -1;
public:
	EnemyFactory();
	// �G�l�~�[�H��̏���������
	void Init();
	// �G�l�~�[�H��̏I������
	void Uninit();
	// �G�l�~�[�H��̍X�V����
	void Update();
	// �G�l�~�[�H��̕`�揈��
	void Draw();

	void Create_HitDrop(D3DXVECTOR2 pos);
	void Create_SelfDestruction(D3DXVECTOR2 pos);
	void Create_ThrowBomb(D3DXVECTOR2 pos);
	void Create_Boss(D3DXVECTOR2 pos);

	// �}�b�v��񂩂�G�l�~�[���쐬
	void SetEnemy();

	// �����蔻��n�֐�
	// �v���C���[�ƃG�l�~�[�̓����蔻��(�v���C���[�ւ̃_���[�W)
	void CollisionPlayerToEnemy();
	// �v���C���[�ƃ{�X�̓����蔻��(�v���C���[�ւ̃_���[�W)
	// void CollisionPlayerToBoss();
	// �v���C���[�U���ƃG�l�~�[�̓����蔻��(�G�l�~�[�ւ̃_���[�W�j
	void CollisoinAttacktoEnemy(D3DXVECTOR2 AttackPos);

	// �G�l�~�[���폜����
	void DeleteEnemy();

	~EnemyFactory();
};

