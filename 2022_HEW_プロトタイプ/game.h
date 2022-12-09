#pragma once

#include "main.h"
#include "renderer.h"
#include "BombFactory.h"
#include "ExplosionFactory.h"

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

BombFactory* GetBombFactory();
ExplosionFactory* GetExplosionFactory();
