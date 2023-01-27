#pragma once
#include "main.h"
#include "enemy.h"

class Enemy_Rush :public Enemy
{
public:
	enum STATE_ENEMY_RUSH
	{
		IDLE,		// 通常時
		CHASE,		// 追う
		WAIT,		// 待機
		DEAD,		// 死亡
	};
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[9] =
	{
		0.00000f,
		0.11111f,
		0.22222f,
		0.33333f,
		0.44444f,
		0.55555f,
		0.66666f,
		0.77777f,
		0.88888f,
	};
	const float M_MukiTable[4] =
	{
		0.00f,	//左向き待機
		0.25f,
		0.50f,
		0.75f,
	};
private:
	// チェイス系変数
	float m_ChaseSpeed = 0.0f;	// 追いかけるスピード
	float m_ActiveRange = 0.0f;	// 追いかけ始める範囲
	D3DXVECTOR2 m_ChaseDirection = D3DXVECTOR2(0.0f, 0.0f);
	float m_DistanceMax = 0.0f;	// 追いかける距離の最大値
	float m_NowDistance = 0.0f;	// 現在の移動距離
	// 待機フレーム
	int m_WaitFrame = 0;
	int m_AnimeFrame = 0;
	STATE_ENEMY_RUSH m_State = IDLE;
public:
	Enemy_Rush(D3DXVECTOR2 pos, int ID, int textureNo);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Enemy_Rush();
private:
	// 自クラスでしか使用しないクラス
	void LookPlayer();
	void Chase();
};

