
/*==============================================================================

   ゲーム画面 [game.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "game.h"
#include "player.h"
#include "bg.h"
#include "Block.h"
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
#include "DamageFloorFactory.h"
#include "UI.h"
#include "gameover.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static GAMESCENE g_Scene;
static GAMESCENE g_SceneNext;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGameStage(void);
void UninitGameStage(void);
void UpdateGameStage(void);
void DrawGameStage(void);
void ChangeGameScene(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static EnemyFactory enemyFactory;
static BombFactory bombFactory;
static ExplosionFactory explosionFactory;
static ShockWaveFactory shockWaveFactory;
static FireBallFactory fireBallFactory;
static DamageFloorFactory damageFloorFactory;
static UI ui(GetPlayer());

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	g_Scene = g_SceneNext = GAMESCENE_STAGE_KASYA;

	InitGameStage();
}

void UninitGame(void)
{
	switch (g_Scene)
	{
	case GAMESCENE_NONE:
		break;
	case GAMESCENE_STAGE_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		UninitGameStage();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}
}

void UpdateGame(void)
{
	switch (g_Scene)
	{
	case GAMESCENE_NONE:
		break;
	case GAMESCENE_STAGE_TENGU:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		UpdateGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		UpdateGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		UpdateGameStage();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UpdateGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}

	//遷移先シーンが設定されていたら
	if (g_Scene != g_SceneNext)
	{
		//シーンの切り替えを行う
		ChangeGameScene();
	}
}

void DrawGame(void)
{
	switch (g_Scene)
	{
	case GAMESCENE_NONE:
		break;
	case GAMESCENE_STAGE_TENGU:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		DrawGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		DrawGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		DrawGameStage();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		DrawGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}
}

//=============================================================================
// ゲーム本編
//=============================================================================
void InitGameStage(void)
{
	// プレイヤーの初期化
	InitPlayer(g_Scene);

	// 背景の初期化
	InitBG();

	// ステージの初期化
	InitBlock(g_Scene);

	// エネミー工場の初期化
	enemyFactory.Init();

	damageFloorFactory.Init();

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
}

void UninitGameStage(void)
{
	//カメラの終了処理
	UninitCamera();

	//数値表示の終了処理
	UninitNumber();

	// 火の玉工場の終了処理
	fireBallFactory.Uninit();

	// 衝撃波工場の終了処理
	shockWaveFactory.Uninit();

	// 爆発工場の終了処理
	explosionFactory.Uninit();

	// 爆発工場の終了処理
	bombFactory.Uninit();

	damageFloorFactory.Uninit();

	//エネミー工場の終了処理
	enemyFactory.Uninit();


	// ステージの終了処理
	UninitBlock();

	// 背景の終了処理
	UninitBG();

	// プレイヤーの終了処理
	UninitPlayer();
}

void UpdateGameStage(void)
{
	// プレイヤーの更新処理
	UpdatePlayer();

	// ステージの更新処理
	UpdateBlock();


	// 背景の更新
	UpdateBG();

	//カメラの更新処理
	UpdateCamera();

	//数値表示の更新処理
	UpdateNumber();

	// 敵の更新処理
	enemyFactory.Update();

	damageFloorFactory.Update();

	// 爆弾工場の更新処理
	bombFactory.Update();

	// 爆弾工場の更新処理
	explosionFactory.Update();

	// 衝撃波工場の更新処理
	shockWaveFactory.Update();

	// 火の玉工場の更新処理
	fireBallFactory.Update();

	//UIのアップデート
	ui.UpdateUI();
}

void DrawGameStage(void)
{
	// 背景の描画
	DrawBG();

	// ステージの描画処理
	DrawBlock();

	damageFloorFactory.Draw();

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

	// カメラの描画処理
	DrawCamera();

	//ここからUIの描画
	//ゲーム関係の描画が全て終わってからUIの描画をする
	ui.DrawUI();

	//数値表示の描画処理
	//DrawNumber();
}

void ChangeGameScene(void)
{
	//現在動作しているシーンを終了させる
	switch (g_Scene)
	{
	case GAMESCENE_STAGE_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}

	//現在のシーンを更新する
	g_Scene = g_SceneNext;

	//更新されたシーンを初期化する
	switch (g_SceneNext)
	{
	case GAMESCENE_STAGE_TENGU:
		InitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		InitGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		InitGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}
}

void SetGameScene(GAMESCENE scene)
{
	g_SceneNext = scene;
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
