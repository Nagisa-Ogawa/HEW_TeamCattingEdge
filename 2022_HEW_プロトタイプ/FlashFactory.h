#pragma once

#include <list>
#include "Flash.h"

class FlashFactory
{
private:
	std::list<Flash*> m_pFlashList;
	int m_TextureNo = -1;
public:
	FlashFactory();
	// “G‚ğì¬
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Init();
	// ”š’eHê‚ÌI—¹ˆ—
	void Uninit();
	// ”š’eHê‚ÌXVˆ—
	void Update();
	// ”š’eHê‚Ì•`‰æˆ—
	void Draw();
	~FlashFactory();

};

