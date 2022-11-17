#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY 100		//敵の表示最大数

enum ENEMYSTATE
{
	chase = 0,
	wait,
	Return,
	NUN_STATE
};

struct ENEMY
{
	D3DXVECTOR2 pos;
//	D3DXVECTOR2	vel;	//速度ベクトル
	D3DXVECTOR2 dir;	//移動方向
	float		speed;	//移動スピード

	D3DXVECTOR2 start_pos;

	float		height;	//高さ
	int			muki;	//向き

	ENEMYSTATE		state;  //状態管理

	int			frame;	//有効フレーム
	int			use;	//可視フラグ
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(void);

ENEMY* GetEnemy(void);
