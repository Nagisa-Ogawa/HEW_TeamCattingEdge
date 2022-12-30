#pragma once
#include "main.h"
#include <list>
#include "player.h"

class FireBall;

class FireBallFactory
{
private:
	std::list<FireBall*> m_pFireBallList;
	PLAYER* m_pPlayer = nullptr;
	int m_nowID = -1;
public:
	FireBallFactory();
	// 火の玉を作成
	void Create(D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power,int mode);
	void Init();
	// 火の玉工場の終了処理
	void Uninit();
	// 火の玉工場の更新処理
	void Update();
	// 火の玉工場の描画処理
	void Draw();
	// 火の玉工場のポインタを渡す
	~FireBallFactory();

	// 指定された火の玉をリストから削除する
	void DeleteFireBall();

	// 当たり判定系関数
	// プレイヤーと火の玉の当たり判定(プレイヤーへのダメージ)
	void CollisionPlayerToFireBall();
	// 壁と火の玉の当たり判定
	void CollisionBlockToFireBall();
	bool HitCheckBox(D3DXVECTOR2 fireBallPos, D3DXVECTOR2 fireBallSize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
	bool HitCheckBox_Block(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
};

