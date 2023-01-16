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
	// エネミー工場の初期化処理
	void Init();
	// エネミー工場の終了処理
	void Uninit();
	// エネミー工場の更新処理
	void Update();
	// エネミー工場の描画処理
	void Draw();
	~DamageFloorFactory();

	void Create(D3DXVECTOR2 pos);
};

