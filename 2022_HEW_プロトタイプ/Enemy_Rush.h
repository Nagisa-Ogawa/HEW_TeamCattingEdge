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
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[6] =
	{
		0.00000f,	//左向き待機
		0.16667f,	//右向き待機
		0.33334f,	//左向き接近
		0.50000f,	//右向き待機
		0.66667f,	//左向き爆発
		0.83334f,	//右向き爆発
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
	STATE_ENEMY_RUSH m_State = IDLE;
public:
	Enemy_Rush(D3DXVECTOR2 pos, int ID);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;
	~Enemy_Rush();
private:
	// 自クラスでしか使用しないクラス
	void LookPlayer();
	void Chase();
};

