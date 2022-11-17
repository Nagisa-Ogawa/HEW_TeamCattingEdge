#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct GOAL
{
	D3DXVECTOR2 pos;

	int			animePtn;
	int			animeCounter;

	int			use;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
