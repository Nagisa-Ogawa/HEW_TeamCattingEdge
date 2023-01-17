#pragma once
#include "main.h"
#include <list>
#include "camera.h"

class DamageFloor;
class EnemyFactory;

class DamageFloorFactory
{
private:
	std::list<DamageFloor*> m_pDamageFloorList;
	EnemyFactory* m_pEnemyFactory = nullptr;
	CAMERA_2D* m_pCamera = nullptr;
	int m_DamageFloorTextureNo = -1;
public:
	DamageFloorFactory();
	// �G�l�~�[�H��̏���������
	void Init();
	// �G�l�~�[�H��̏I������
	void Uninit();
	// �G�l�~�[�H��̍X�V����
	void Update();
	// �G�l�~�[�H��̕`�揈��
	void Draw();
	~DamageFloorFactory();

	void Create(D3DXVECTOR2 pos);
	void Set();

	// �G�ƃ_���[�W���̓����蔻��
	void CollisionDamageFloorToEnemy();
	// �v���C���[�ƃ_���[�W���̓����蔻��
	void CollisionDamageFloorToPlayer();
};

