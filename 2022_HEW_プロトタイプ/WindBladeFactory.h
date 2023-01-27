#pragma once
#include <list>
#include "main.h"

class WindBlade;
struct CAMERA_2D;
struct PLAYER;

class WindBladeFactory
{
private:
	std::list<WindBlade*> m_pWindBladeList;
	PLAYER* m_pPlayer = nullptr;
	CAMERA_2D* m_pCamera = nullptr;
	int m_nowID = -1;

	int m_WindBladeNo = -1;

public:
	WindBladeFactory();
	// エネミー工場の初期化処理
	void Init();
	// エネミー工場の終了処理
	void Uninit();
	// エネミー工場の更新処理
	void Update();
	// エネミー工場の描画処理
	void Draw();

	// Create関数
	void Create(D3DXVECTOR2 pos,D3DXVECTOR2 size, int muki,bool isDuo);

	// プレイヤーと風の刃の当たり判定
	void CollisionWindBladeToPlayer();
	bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
		D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
	~WindBladeFactory();
};

