#pragma once
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DIVIDE_X 4	//横の分割数
#define DIVIDE_Y 2	//縦の分割数

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)

#define ENEMY_SIZE (120)	//　プレイヤーのサイズ

// 敵のジャンプパワー
#define ENEMY_JUMPPOWER_X (4.0f)	
#define ENEMY_JUMPPOWER_Y (-6.0f)
// 敵のジャンプ中の減衰力
#define ENEMY_JUMPATTENUATION_X (0.2f)
#define ENEMY_JUMPATTENUATION_Y (0.2f)

#define ENEMY_ACTIVERADIUS (400.0f)	// 敵の探知範囲の半径
#define ENEMY_WAITFRAME_SETUP (60)	// 攻撃準備時の待機フレーム数
#define ENEMY_WAITFRAME_WAIT (60)	// 攻撃後の待機フレーム数
#define ENEMY_WAITFRAME_AFTERDROP (30) // ドロップ後の待機フレーム数
#define ENEMY_DROPPOWER (0.98f)		// ドロップ中の加速係数


enum STATE_ENEMY_HITDROP 
{
	IDLE,
	SETUP,
	JUMP,
	DROP,
	AFTERDROP,
	WAIT,
};

//*****************************************************************************
// ヒットドロップする敵のクラス
//*****************************************************************************
class Enemy_HitDrop :public Enemy
{
private:
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[4] =
	{
		0.0f,
		0.25f,
		0.50f,
		0.75
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//右向き
		0.5f,	//左向き
	};

	// ジャンプ系変数
	D3DXVECTOR2 m_JumpPower = D3DXVECTOR2(0.0f, 0.0f);	// 敵のジャンプのパワー
	D3DXVECTOR2 m_JumpAttenuation = D3DXVECTOR2(0.0f, 0.0f);	// ジャンプ中の減衰力
	float m_DropPower = 0.0f;	// ドロップ中のパワー
	float m_DropPosX = 0.0f;	// プレイヤーがいた座標

	// 待機フレーム
	int m_WaitFrame = 0;
	STATE_ENEMY_HITDROP m_State = IDLE;
public:
	Enemy_HitDrop(D3DXVECTOR2 pos,int ID);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;
	~Enemy_HitDrop();
private:
	// 自クラスでしか使用しないクラス
	void ChangeSetUp();	// 攻撃準備時の関数
	void Jump();	// ジャンプ時の関数
	void LookPlayer(); // プレイヤーの方向を向く関数
};

