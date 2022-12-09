#pragma once
#include "main.h"
#include "enemy.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// マクロ定義
//*****************************************************************************
class Enemy_SelfDestruct:public Enemy
{
public:
	enum STATE_ENEMY_SELFDESTRUCT
	{
		IDLE,		// 待機
		CHASE,		// 追う
		SETUP_BOMB,	// 爆発準備
		BOMB,		// 爆発
	};
private:
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[3] =
	{
		0.0f,
		0.333f,
		0.666f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//右向き
		0.5f,	//左向き
	};

	// チェイス系変数
	float m_ChaseSpeed = 0.0f;	// 追いかけるスピード
	float m_ActiveRange = 0.0f;	// 追いかけ始める範囲
	float m_BombRange = 0.0f;	// 爆発を始める範囲
	// 爆発系変数
	int m_BombFrame = 0;	// 爆発するまでのフレーム
	float m_MaxSize = 0.0f;	// 爆発前の最大サイズ
	float m_ChangeSizeValue = 0.0f;// 爆発前のサイズの1フレーム当たりの変化量
	ExplosionFactory* m_pExplosionFactory = nullptr;

	// 待機フレーム
	int m_WaitFrame = 0;
	STATE_ENEMY_SELFDESTRUCT m_State = IDLE;
public:
	Enemy_SelfDestruct(D3DXVECTOR2 pos, int ID);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;
	~Enemy_SelfDestruct();
private:
	// 自クラスでしか使用しないクラス
	void LookPlayer();
	void SetUp();
	void Chase();
};


