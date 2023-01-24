#pragma once
#include <list>
#include "main.h"

class SwitchBullet;
struct CAMERA_2D;
struct PLAYER;

class SwitchBulletFactory
{
private:
	std::list<SwitchBullet*> m_pSwitchBulletList;
	PLAYER* m_pPlayer = nullptr;
	CAMERA_2D* m_pCamera = nullptr;
	int m_nowID = -1;

	int m_SwitchBulletNo = -1;

public:
	SwitchBulletFactory();
	// �G�l�~�[�H��̏���������
	void Init();
	// �G�l�~�[�H��̏I������
	void Uninit();
	// �G�l�~�[�H��̍X�V����
	void Update();
	// �G�l�~�[�H��̕`�揈��
	void Draw();

	// Create�֐�
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 size,D3DXVECTOR2 targetPos,int muki,int mode);

	// �v���C���[�ƕ��̐n�̓����蔻��
	void CollisionSwitchBulletToPlayer();
	void CollisionSwitchBulletToBlock();
	bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
		D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
	bool HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height,
		D3DXVECTOR2 box2pos, float box2width, float box2height);

	void DeleteSwitchBullet();

	// �e�ƃ{�X��؂�ւ���t���O���`�F�b�N����֐�
	bool CheckSwitchFlag();
	D3DXVECTOR2 GetSwitchPos();
	~SwitchBulletFactory();
};

