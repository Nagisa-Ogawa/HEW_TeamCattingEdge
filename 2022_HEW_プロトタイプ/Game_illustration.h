#pragma once
#include "main.h"

enum GAMESCENE;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGameIllust(GAMESCENE gamescene, GAMESCENE nowscene);
void UninitGameIllust(void);
void UpdateGameIllust(void);
void DrawGameIllust(void);

