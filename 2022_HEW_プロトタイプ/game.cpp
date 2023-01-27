
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
#include "WindBladeFactory.h"
#include "FlashFactory.h"
#include "ThunderBladeFactory.h"
#include "RayFactory.h"
#include "SwitchBulletFactory.h"
#include "UI.h"
#include "fade.h"
#include "Game_illustration.h"

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
static WindBladeFactory windBladeFactory;
static FlashFactory flashFactory;
static RayFactory rayFactory;
static ThunderBladeFactory thunderBladeFactory;
static SwitchBulletFactory switchBulletFactory;
static UI ui(GetPlayer());

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	g_Scene = g_SceneNext = GAMESCENE_STAGE_TENGU;

	switch (g_Scene)
	{
	case GAMESCENE_STAGE_TENGU:
		InitGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		InitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		InitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		InitGameStage();
		break;
	case GAMESCENE_BASS_FUJIN:
		InitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		InitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		InitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_RESULT:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	default:
		break;
	}
}

void UninitGame(void)
{
	switch (g_Scene)
	{
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
	case GAMESCENE_BASS_FUJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		UninitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		UninitGameIllust();
		break;
	default:
		break;
	}
}

void UpdateGame(void)
{
	switch (g_Scene)
	{
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
	case GAMESCENE_BASS_FUJIN:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		UpdateGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		UpdateGameIllust();
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
	case GAMESCENE_BASS_FUJIN:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		DrawGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		DrawGameIllust();
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

	windBladeFactory.Init();

	flashFactory.Init();

	thunderBladeFactory.Init();

	switchBulletFactory.Init();

	rayFactory.Init();

	//UI
	ui.InitUI();

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

	rayFactory.Uninit();

	switchBulletFactory.Init();

	thunderBladeFactory.Uninit();

	flashFactory.Uninit();

	windBladeFactory.Uninit();

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

	windBladeFactory.Update();

	flashFactory.Update();

	thunderBladeFactory.Update();

	switchBulletFactory.Update();

	rayFactory.Update();

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

	windBladeFactory.Draw();

	flashFactory.Draw();

	thunderBladeFactory.Draw();

	switchBulletFactory.Draw();

	rayFactory.Draw();

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
	case GAMESCENE_BASS_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_FUJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		UninitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		UninitGameIllust();
		break;
	default:
		break;
	}

	switch (g_SceneNext)
	{
	case GAMESCENE_STAGE_TENGU:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_FUJIN:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_RESULT:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		InitGameIllust(g_SceneNext, g_Scene);
	default:
		break;
	}

	//現在のシーンを更新する
	g_Scene = g_SceneNext;
}

void SetGameScene(GAMESCENE scene)
{
	g_SceneNext = scene;
}

GAMESCENE * GetNowScene()
{
	return &g_Scene;
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

WindBladeFactory * GetWindBladeFactory()
{
	return &windBladeFactory;
}

FlashFactory * GetFlashFactory()
{
	return &flashFactory;
}

RayFactory * GetRayFactory()
{
	return &rayFactory;
}

ThunderBladeFactory * GetThunderBladeFactory()
{
	return &thunderBladeFactory;
}

SwitchBulletFactory * GetSwitchBulletFactory()
{
	return &switchBulletFactory;
}
