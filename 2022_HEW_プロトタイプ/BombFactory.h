#pragma once
#include "main.h"
#include <list>

class Bomb;

class BombFactory
{
private:
	std::list<Bomb*> m_pBombList;
	int m_nowID = -1;
public:
	BombFactory();
	// �G���쐬
	void CreateContactBomb(D3DXVECTOR2 createPos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	void CreateInstallationBomb(D3DXVECTOR2 createPos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	void Init();
	// ���e�H��̏I������
	void Uninit();
	// ���e�H��̍X�V����
	void Update();
	// ���e�H��̕`�揈��
	void Draw();
	// ���e�H��̃|�C���^��n��
	~BombFactory();

	// �w�肳�ꂽ���e�����X�g����폜����
	void DeleteBomb();
};
