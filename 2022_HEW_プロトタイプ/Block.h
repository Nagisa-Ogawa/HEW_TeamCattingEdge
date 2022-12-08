#pragma once

#include <vector>
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BLOCK_SIZE (60)
#define STAGE_WIDTH (74 + 2 + 2)
#define STAGE_HEIGHT (18)

#define HIT_VERTICAL	0x00000001
#define HIT_HORIZONTAL	0x00000002
#define HIT_UP			0x00000010
#define HIT_DOWN		0x00000020
#define HIT_LEFT		0x00000040
#define HIT_RIGHT		0x00000080
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

float GetBlockHeight(void);

DWORD hitChackNormalPlayer_Block(D3DXVECTOR2 move);
DWORD HitChackEnemy_Block(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 moveVec);
void hitChackWarpPlayer_Block(D3DXVECTOR2 move);

float GetBlockHeight(void);
std::vector<std::vector<int>> GetPPBlocks();
