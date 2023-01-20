#pragma once

#include "main.h"

class EnemyFactory;
class BombFactory;
class ExplosionFactory;
class ShockWaveFactory;
class FireBallFactory;
class WindBladeFactory;
class FlashFactory;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum GAMESCENE {
	GAMESCENE_NONE,				//
	GAMESCENE_STAGE_TENGU,		//天狗ステージ
	GAMESCENE_BASS_TENGU,		//天狗ボスステージ
	GAMESCENE_STAGE_KASYA,		//火車ステージ
	GAMESCENE_BASS_KASYA,		//火車ボスステージ
	GAMESCENE_STAGE_NUM,		//火車ステージ

	GAMESCENE_PICTURE_OVERGAME, //ゲームオーバー
	GAMESCENE_PICTURE_RESULT,	//リザルト

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

void SetGameScene(GAMESCENE scene);