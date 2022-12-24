#pragma once

#include "main.h"
#include "enemy.h"
#include "game.h"
#include "FireBallFactory.h"

class Enemy_GhostFire : public Enemy
{
public:
	enum STATE_ENEMY_GHOSTFIRE
	{
		IDLE,
		THROW,
		WAIT,
		DEAD,
	};
private:
	const float m_AnimeTable[1] =
	{
		0.0f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//左向き
		0.5f,
	};

	STATE_ENEMY_GHOSTFIRE m_State = IDLE;
	FireBallFactory* m_pFireBallFactory = nullptr;

	// 火の玉系変数
	int m_WaitIdleTime = 0;	// 火の玉を投げる前の待機時間
	int m_WaitTime = 0;	// 火の玉を投げた後の待機時間
	int m_WaitNextThrowTime = 0; // 二発目の火の玉を投げるまでの待機時間
	bool m_IsThrow = true;
	int m_ThrowNum = 0;
	// 待機フレーム
	int m_WaitFrame = 0;


public:
	Enemy_GhostFire(D3DXVECTOR2 pos,int muki ,int ID);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;
	~Enemy_GhostFire();
	// 火の玉を投げる
	void Throw();
};

