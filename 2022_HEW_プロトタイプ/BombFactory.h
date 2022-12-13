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
	// “G‚ğì¬
	void CreateContactBomb(D3DXVECTOR2 createPos, D3DXVECTOR2 endPos);
	void CreateInstallationBomb(D3DXVECTOR2 createPos, D3DXVECTOR2 endPos);
	void Init();
	// ”š’eHê‚ÌI—¹ˆ—
	void Uninit();
	// ”š’eHê‚ÌXVˆ—
	void Update();
	// ”š’eHê‚Ì•`‰æˆ—
	void Draw();
	// ”š’eHê‚Ìƒ|ƒCƒ“ƒ^‚ğ“n‚·
	~BombFactory();

	// w’è‚³‚ê‚½”š’e‚ğƒŠƒXƒg‚©‚çíœ‚·‚é
	void DeleteBomb();
};
