#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

class ExplosionFactory;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class Enemy_ExplosionGas : public Enemy
{
public:
	enum STATE_ENEMY_EXPLOSIONGAS
	{
		IDLE,		// 待機
		SETUP_BOMB,	// 爆発準備
		BOMB,		// 爆発
		DEAD,		// 死亡
	};
private:
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[7] =
	{
		0.0f,
		0.142857f,
		0.285714f,
		0.428571f,
		0.571428f,
		0.714285f,
		0.857142f
	};
	const float M_MukiTable[2] =
	{
		0.0f,
		0.5f,	//左向き待機
	};

	// チェイス系変数
	float m_BombRange = 0.0f;	// 爆発を始める範囲
	// 爆発系変数
	int m_BombFrame = 0;	// 爆発するまでのフレーム
	float m_MaxSize = 0.0f;	// 爆発前の最大サイズ
	float m_ChangeSizeValue = 0.0f;// 爆発前のサイズの1フレーム当たりの変化量
	ExplosionFactory* m_pExplosionFactory = nullptr;

	// 待機フレーム
	int m_WaitFrame = 0;
	int m_WaitAnimeFrame = 0;
	STATE_ENEMY_EXPLOSIONGAS m_State = IDLE;
public:
	Enemy_ExplosionGas(D3DXVECTOR2 pos, int ID, int textureNo);
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

	~Enemy_ExplosionGas();
private:
	// 自クラスでしか使用しないクラス
	void SetUp();

};

