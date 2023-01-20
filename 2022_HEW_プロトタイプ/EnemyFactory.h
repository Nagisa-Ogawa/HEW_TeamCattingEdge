#pragma once

#include <list>
#include <vector>
#include "enemy.h"
#include "player.h"
#include "camera.h"

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
	CAMERA_2D* m_pCamera = nullptr;
	int m_nowID = -1;
	DWORD m_Result = 0;

	// �G�̃e�N�X�`���ԍ�
	int m_EnemyHitDropNo = -1;
	int m_EnemySelfDestructNo = -1;
	int m_EnemyThrowBombNo = -1;
	int m_EnemyTenguNo = -1;

	int m_EnemyGhostFireNo = -1;
	int m_EnemyRushNo = -1;
	int m_EnemyExplosionGasNo = -1;
	int m_EnemyKasyaNo = -1;

	int m_EnemyFujin = -1;
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

	// Create�֐�
	void Create_HitDrop(D3DXVECTOR2 pos);
	void Create_SelfDestruction(D3DXVECTOR2 pos);
	void Create_ThrowBomb(D3DXVECTOR2 pos);
	void Create_SubThrowBomb(D3DXVECTOR2 pos);
	void Create_GhostFire(D3DXVECTOR2 pos, int muki);
	void Create_Rush(D3DXVECTOR2 pos);
	void Create_ExplosionGas(D3DXVECTOR2 pos);
	void Create_FujinAvator(D3DXVECTOR2 pos, D3DXVECTOR2 targetPos);

	// BOSS�nCreate�֐�
	void Create_Boss_Tengu(D3DXVECTOR2 pos);
	void Create_Boss_Kasya(D3DXVECTOR2 pos);
	void Create_Boss_Fujin(D3DXVECTOR2 pos);

	std::list<Enemy*>* GetEnemyList() { return &m_pEnemyList; };
	// �}�b�v��񂩂�G�l�~�[���쐬
	void SetEnemy();

	// �����蔻��n�֐�
	// �v���C���[�ƃu���b�N�̓����蔻��
	void CollisionWallToEnemy();
	void HitCheckWallToEnemy(std::vector<std::vector<int>>* g_Stage);
	// �v���C���[�ƃG�l�~�[�̓����蔻��(�v���C���[�ւ̃_���[�W)
	void CollisionPlayerToEnemy();
	// �v���C���[�ƃ{�X�̓����蔻��(�v���C���[�ւ̃_���[�W)
	// void CollisionPlayerToBoss();
	// �v���C���[�U���ƃG�l�~�[�̓����蔻��(�G�l�~�[�ւ̃_���[�W�j
	void CollisoinAttacktoEnemy(D3DXVECTOR2 AttackPos);

	bool CheckFujinAvatorSetEnd();

	// �G�l�~�[���폜����
	void DeleteEnemy();

	~EnemyFactory();
};

