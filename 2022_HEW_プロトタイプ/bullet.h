#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BULLET 100	//弾の表示最大数

struct BULLET
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2	vel;	//速度ベクトル

	float		height;	//高さ
	int			muki;	//向き

	int			frame;	//有効フレーム
	int			use;	//可視フラグ
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR2 pos, int muki);

BULLET* GetBullet(void);
