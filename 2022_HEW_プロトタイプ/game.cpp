
/*==============================================================================

   ゲーム画面 [game.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "polygon.h"
#include "player.h"
#include "goal.h"
#include "bg.h"
#include "Block.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "number.h"
#include "camera.h"
#include "inputx.h"
#include "EnemyFactory.h"
#include "BombFactory.h"
#include "ExplosionFactory.h"
#include "ShockWaveFactory.h"
#include "FireBallFactory.h"
#include "UI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_BGMNo;
static int g_TextureCloud = 0;

static EnemyFactory enemyFactory;
static BombFactory bombFactory;
static ExplosionFactory explosionFactory;
static ShockWaveFactory shockWaveFactory;
static FireBallFactory fireBallFactory;
static UI ui(GetPlayer());

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	// プレイヤーの初期化
	InitPlayer();

	// ゴールの初期化
	InitGoal();

	// 背景の初期化
	InitBG();

	// ステージの初期化
	InitBlock();

	// 弾の初期化
	InitBullet();

	// エネミー工場の初期化
	enemyFactory.Init();

	// 爆弾工場の初期化
	bombFactory.Init();

	// 爆発工場の初期化
	explosionFactory.Init();

	// 衝撃波工場の初期化
	shockWaveFactory.Init();

	// 火の玉工場の初期化
	fireBallFactory.Init();

	//数値表示の初期化
	InitNumber();

	//カメラの初期化
	InitCamera();

	//ゲーム用BGMの読み込み
	g_BGMNo = LoadSound((char*)"data/BGM/sample001.wav");

	//BGM再生(二つ目の引数はループ回数 負の値を指定すると無限ループ)
	//PlaySound(g_BGMNo, -1);
}

void UninitGame(void)
{
	//カメラの終了処理
	UninitCamera();

	//数値表示の終了処理
	UninitNumber();

	// 衝撃波工場の終了処理
	shockWaveFactory.Uninit();

	// 爆発工場の終了処理
	explosionFactory.Uninit();

	// 爆発工場の終了処理
	bombFactory.Uninit();

	//エネミー工場の終了処理
	enemyFactory.Uninit();

	// 火の玉工場の終了処理
	fireBallFactory.Uninit();

	// 弾の終了処理
	UninitBullet();

	// ステージの終了処理
	UninitBlock();

	// 背景の終了処理
	UninitBG();

	// ゴールの終了処理
	UninitGoal();

	// プレイヤーの終了処理
	UninitPlayer();
}

void UpdateGame(void)
{

	// プレイヤーの更新処理
	UpdatePlayer();

	// 敵の更新処理
	enemyFactory.Update();

	// 爆弾工場の更新処理
	bombFactory.Update();

	// 爆弾工場の更新処理
	explosionFactory.Update();

	// 衝撃波工場の更新処理
	shockWaveFactory.Update();

	// 火の玉工場の更新処理
	fireBallFactory.Update();

	// ステージの更新処理
	UpdateBlock();

	// 背景の更新
	UpdateBG();

	//カメラの更新処理
	UpdateCamera();

	//数値表示の更新処理
	UpdateNumber();

	ui.UpdateUI();

}

void DrawGame(void)
{
	// 背景の描画
	DrawBG();

	// ステージの描画処理
	DrawBlock();

	// 衝撃波工場の描画処理
	shockWaveFactory.Draw();

	// 爆弾工場の描画処理
	bombFactory.Draw();

	// 爆発工場の描画処理
	explosionFactory.Draw();

	// 敵の描画処理
	enemyFactory.Draw();

	// 火の玉工場の描画処理
	fireBallFactory.Draw();

	// プレイヤーの描画処理
	DrawPlayer();

	// ゴールの描画処理
	DrawGoal();

	// カメラの描画処理
	DrawCamera();

	// 弾の描画処理
	DrawBullet();

	//ここからUIの描画
	//ゲーム関係の描画が全て終わってからUIの描画をする

	//数値表示の描画処理
	//DrawNumber();
}

EnemyFactory* GetEnemyFactory()
{
	return &enemyFactory;
}

BombFactory* GetBombFactory()
{
	return &bombFactory;
}

ExplosionFactory* GetExplosionFactory()
{
	return &explosionFactory;
}

ShockWaveFactory * GetShockWaveFactory()
{
	return  &shockWaveFactory;
}

FireBallFactory * GetFireBallFactory()
{
	return &fireBallFactory;
}

