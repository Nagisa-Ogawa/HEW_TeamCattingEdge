#pragma once

#include "main.h"
#include "renderer.h"

class EnemyFactory;
class BombFactory;
class ExplosionFactory;
class ShockWaveFactory;
class FireBallFactory;

//*****************************************************************************
// マクロ定義
//*****************************************************************************


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
