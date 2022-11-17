#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

enum STATUS
{
	normal,
	warpwait,
	warp,
	fall,
	STATUS_NUM,
};

enum MAPS
{
	tutorial,
	bass,
	NUM_MAPS,
};

struct PLAYER
{
	D3DXVECTOR2 pos;		//現在座標
	D3DXVECTOR2 oldpos;		//１フレーム前の座標
	D3DXVECTOR2 warppos;	//ワープ地点の座標
	D3DXVECTOR2	vel;		//速度ベクトル
	STATUS		status;		//プレイヤーの状態

	int			warpframe;  //ワープ時の入力時間
	int         waitafterwarp;
	float		warppower;  //ワープ力
	float		gravity;    //重力
	int			warpFlag;	//ワープ回数フラグ

	int			muki;
	int			animePtn;
	int			animeCounter;

	bool		move;

	int attackflag;

	int			hp;			//HP
	MAPS		map;		//現在MAP
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);
