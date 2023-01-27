#pragma once
#include <list>
#include "main.h"

class WindBlade;
struct CAMERA_2D;
struct PLAYER;

class WindBladeFactory
{
private:
	std::list<WindBlade*> m_pWindBladeList;
	PLAYER* m_pPlayer = nullptr;
	CAMERA_2D* m_pCamera = nullptr;
	int m_nowID = -1;

	int m_WindBladeNo = -1;

public:
	WindBladeFactory();
	// �G�l�~�[�H��̏���������
	void Init();
	// �G�l�~�[�H��̏I������
	void Uninit();
	// �G�l�~�[�H��̍X�V����
	void Update();
	// �G�l�~�[�H��̕`�揈��
	void Draw();

	// Create�֐�
	void Create(D3DXVECTOR2 pos,D3DXVECTOR2 size, int muki,bool isDuo);

	// �v���C���[�ƕ��̐n�̓����蔻��
	void CollisionWindBladeToPlayer();
	bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
		D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
	~WindBladeFactory();
};

