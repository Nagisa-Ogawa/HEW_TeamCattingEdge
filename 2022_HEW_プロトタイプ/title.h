#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

struct Anime
{
	int Texture = 0;

	D3DXVECTOR2 pos = D3DXVECTOR2(500, 500);
	D3DXVECTOR2 size = D3DXVECTOR2(100, 100);
	D3DXVECTOR2 pack = D3DXVECTOR2(1, 1);

	int anmChange = 0;
	int anmNext = 0;
};