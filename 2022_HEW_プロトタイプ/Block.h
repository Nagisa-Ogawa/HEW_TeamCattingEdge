#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BLOCK_SIZE (60)
#define STAGE_WIDTH (32)
#define STAGE_HEIGHT (18)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
