#pragma once
#include <list>
#include "main.h"

class ThunderBlade;
struct CAMERA_2D;
struct PLAYER;

class ThunderBladeFactory
{
private:
	std::list<ThunderBlade*> m_pThunderBladeList;
	PLAYER* m_pPlayer = nullptr;
	CAMERA_2D* m_pCamera = nullptr;
	int m_nowID = -1;

	int m_ThunderBladeNo = -1;
	int m_BigThunderBladeNo = -1;

public:
	ThunderBladeFactory();
	// エネミー工場の初期化処理
	void Init();
	// エネミー工場の終了処理
	void Uninit();
	// エネミー工場の更新処理
	void Update();
	// エネミー工場の描画処理
	void Draw();

	// Create関数
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 size,int mode);

	// プレイヤーと風の刃の当たり判定
	void CollisionThunderBladeToPlayer();
	bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
		D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
	~ThunderBladeFactory();
};

