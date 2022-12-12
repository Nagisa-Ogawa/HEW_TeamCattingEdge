#pragma once

#include "main.h"
#include "enemy.h"
#include "game.h"
#include "BombFactory.h"

class Enemy_ThrowBomb : public Enemy
{
public:
	enum STATE_ENEMY_THROWBOMB
	{
		IDLE,
		THROW,
		WAIT,
		COOLDOWN,
	};
private:
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//右向き
		0.5f,	//左向き
	};

	BombFactory* m_pBombFactory = nullptr;
	STATE_ENEMY_THROWBOMB m_State = IDLE;

	// 爆弾系
	float m_ActiveRange = 0.0f;	// 爆弾を投げ始める範囲
	int m_WaitTime = 0;	// 爆弾を投げた後の待機時間
	int m_CooldownTime = 0;	// 爆弾を投げ終わった後のクールタイム時間
	D3DXVECTOR2 m_ThrowPower;	// 距離1につき投げるパワー

	// 待機フレーム
	int m_WaitFrame = 0;
public:
	Enemy_ThrowBomb(D3DXVECTOR2 pos, int ID);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;
	~Enemy_ThrowBomb();
	// 爆弾を投げる
	void Throw();	
	void LookPlayer();
};

