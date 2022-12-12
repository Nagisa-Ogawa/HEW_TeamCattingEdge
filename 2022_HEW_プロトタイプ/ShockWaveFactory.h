#pragma once

#include <list>
#include "main.h"
#include "ShockWave.h"

class ShockWaveFactory
{
private:
	std::list<ShockWave*> m_pShockWaveList;
public:
	ShockWaveFactory();
	// �Ռ��g���쐬
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 moveVec,int muki);
	void Init();
	// �Ռ��g�H��̏I������
	void Uninit();
	// �Ռ��g�H��̍X�V����
	void Update();
	// �Ռ��g�H��̕`�揈��
	void Draw();
	// �Ռ��g�H��̃|�C���^��n��
	~ShockWaveFactory();

	// Get�n�֐�
	std::list<ShockWave*> GetShockWaveList() { return m_pShockWaveList; };

	// �Ռ��g���폜����
	void DeleteShockWave();
};

