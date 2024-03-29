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
	// Õgðì¬
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 moveVec,int muki);
	void Init();
	// ÕgHêÌI¹
	void Uninit();
	// ÕgHêÌXV
	void Update();
	// ÕgHêÌ`æ
	void Draw();
	// ÕgHêÌ|C^ðn·
	~ShockWaveFactory();

	// GetnÖ
	std::list<ShockWave*> GetShockWaveList() { return m_pShockWaveList; };

	// Õgðí·é
	void DeleteShockWave();
};

