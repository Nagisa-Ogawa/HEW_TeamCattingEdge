#pragma once
#include "main.h"
#include <list>

class DamageFloor;

class DamageFloorFactory
{
private:
	std::list<DamageFloor*> m_pDamageFloorList;
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
};

