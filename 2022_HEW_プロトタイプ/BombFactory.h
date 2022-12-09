#pragma once
#include "main.h"
#include <list>

class Bomb;

class BombFactory
{
private:
	std::list<Bomb*> m_pBombList;
public:
	BombFactory();
	// “G‚ğì¬
	void Create(D3DXVECTOR2 createPos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	void Init();
	// ”š’eHê‚ÌI—¹ˆ—
	void Uninit();
	// ”š’eHê‚ÌXVˆ—
	void Update();
	// ”š’eHê‚Ì•`‰æˆ—
	void Draw();
	// ”š’eHê‚Ìƒ|ƒCƒ“ƒ^‚ğ“n‚·
	~BombFactory();
};
