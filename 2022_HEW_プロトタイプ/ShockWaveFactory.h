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
	// ÕŒ‚”g‚ğì¬
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 moveVec,int muki);
	void Init();
	// ÕŒ‚”gHê‚ÌI—¹ˆ—
	void Uninit();
	// ÕŒ‚”gHê‚ÌXVˆ—
	void Update();
	// ÕŒ‚”gHê‚Ì•`‰æˆ—
	void Draw();
	// ÕŒ‚”gHê‚Ìƒ|ƒCƒ“ƒ^‚ğ“n‚·
	~ShockWaveFactory();

	// GetŒnŠÖ”
	std::list<ShockWave*> GetShockWaveList() { return m_pShockWaveList; };

	// ÕŒ‚”g‚ğíœ‚·‚é
	void DeleteShockWave();
};

