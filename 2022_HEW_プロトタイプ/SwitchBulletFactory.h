#pragma once
#include <list>
#include "main.h"

class SwitchBullet;
struct CAMERA_2D;
struct PLAYER;

class SwitchBulletFactory
{
private:
	std::list<SwitchBullet*> m_pSwitchBulletList;
	PLAYER* m_pPlayer = nullptr;
	CAMERA_2D* m_pCamera = nullptr;
	int m_nowID = -1;

	int m_SwitchBulletNo = -1;

public:
	SwitchBulletFactory();
	// エネミー工場の初期化処理
	void Init();
	// エネミー工場の終了処理
	void Uninit();
	// エネミー工場の更新処理
	void Update();
	// エネミー工場の描画処理
	void Draw();

	// Create関数
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 size,D3DXVECTOR2 targetPos,int muki,int mode);

	// プレイヤーと風の刃の当たり判定
	void CollisionSwitchBulletToPlayer();
	void CollisionSwitchBulletToBlock();
	bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
		D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);
	bool HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height,
		D3DXVECTOR2 box2pos, float box2width, float box2height);

	void DeleteSwitchBullet();

	// 弾とボスを切り替えるフラグをチェックする関数
	bool CheckSwitchFlag();
	D3DXVECTOR2 GetSwitchPos();
	~SwitchBulletFactory();
};

