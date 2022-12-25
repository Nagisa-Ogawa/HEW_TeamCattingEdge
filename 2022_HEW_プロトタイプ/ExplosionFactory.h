#pragma once

#include <list>
#include "Explosion.h"

class ExplosionFactory
{
private:
	std::list<Explosion*> m_pExplosion;
	int m_TextureNo = -1;
public:
	ExplosionFactory();
	// �G���쐬
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Init();
	// ���e�H��̏I������
	void Uninit();
	// ���e�H��̍X�V����
	void Update();
	// ���e�H��̕`�揈��
	void Draw();
	~ExplosionFactory();

	// �v���C���[�Ɣ����̓����蔻��
	void CollisionBlastToPlayer();
};

