#pragma once
#include <list>
#include "main.h"
#include "Bomb.h"

class BombFactory
{
private:
	std::list<Bomb*> m_pBombList;
public:
	BombFactory();
	// �G���쐬
	void Create(D3DXVECTOR2 createPos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	void Init();
	// ���e�H��̏I������
	void Uninit();
	// ���e�H��̍X�V����
	void Update();
	// ���e�H��̕`�揈��
	void Draw();
	// ���e�H��̃|�C���^��n��
	BombFactory* GetBombFactory() { return this; };
	~BombFactory();
};
