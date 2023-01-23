#pragma once

#include "main.h"

class EnemyFactory;
class BombFactory;
class ExplosionFactory;
class ShockWaveFactory;
class FireBallFactory;
class WindBladeFactory;
class FlashFactory;
class RayFactory;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum GAMESCENE {

	GAMESCENE_NONE,
	GAMESCENE_STAGE_TENGU,		//天狗ステージ
	GAMESCENE_BASS_TENGU,		//天狗ボスステージ
	GAMESCENE_STAGE_KASYA,		//火車ステージ
	GAMESCENE_BASS_KASYA,		//火車ボスステージ
	GAMESCENE_BASS_FUJINRAIJIN,
	GAMESCENE_BASS_FINAL,

	GAMESCENE_STAGE_NUM,
	
	GAMESCENE_PICTURE_STAGE01,		//ステージ１（天狗）の挿絵
	GAMESCENE_PICTURE_STAGE01_BOSS, //ステージ１（天狗）の挿絵
	GAMESCENE_PICTURE_STAGE02,		//ステージ２（火車）の挿絵
	GAMESCENE_PICTURE_STAGE02_BOSS, //ステージ２（火車）の挿絵
	GAMESCENE_PICTURE_STAGE03,		//ステージ３（ラスボス）の挿絵
	GAMESCENE_PICTURE_STAGE03_BOSS, //ステージ３（ラスボス）の挿絵
	GAMESCENE_PICTURE_OVERGAME,		//ゲームオーバー
	GAMESCENE_PICTURE_RESULT,		//リザルト

	GAMESCENE_NUM,	//終端コード
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

EnemyFactory* GetEnemyFactory();
BombFactory* GetBombFactory();
ExplosionFactory* GetExplosionFactory();
ShockWaveFactory* GetShockWaveFactory();
FireBallFactory* GetFireBallFactory();
WindBladeFactory* GetWindBladeFactory();
FlashFactory* GetFlashFactory();
RayFactory*	GetRayFactory();

void SetGameScene(GAMESCENE scene);