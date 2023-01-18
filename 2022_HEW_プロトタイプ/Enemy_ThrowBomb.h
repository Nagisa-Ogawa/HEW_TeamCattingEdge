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
		DEAD,
	};
protected:
	const float m_AnimeTable[3] =
	{
		0.0f,
		0.333f,
		0.666f,
	};
	const float M_MukiTable[4] =
	{
		0.00f,	//左向き
		0.25f,	//右向き
		0.50f,	//死亡時左向き
		0.75f,	//死亡時右向き
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
	Enemy_ThrowBomb(D3DXVECTOR2 pos, int ID, int textureNo);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	virtual void Uninit() override;
	// エネミーの更新処理
	virtual void Update() override;
	// エネミーの描画処理
	void Draw() override;

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Enemy_ThrowBomb();
	// 爆弾を投げる
	void Throw();	
	void LookPlayer();
};

