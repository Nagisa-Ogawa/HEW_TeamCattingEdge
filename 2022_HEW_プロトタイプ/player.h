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
	float		size;
	D3DXVECTOR2	vel;		//速度ベクトル
	STATUS		status;		//プレイヤーの状態

	//ワープ関連
	int			warpframe;  //ワープ時の入力時間
	int         waitafterwarp;//ワープ後の停止時間
	float		warppower;  //ワープ力
	float		gravity;    //重力
	int			warpFlag;	//ワープ回数フラグ

	bool        LandingFlag; //着地フラグ

	int			muki;
	int			animePtn;
	int			animeCounter;

	bool		move;

	int			attackflag;

	int			hp;			//HP
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void UpdateEndPlayer(void);
void DrawPlayer(void);

void AdjustPlayer(D3DXVECTOR2 pos);

PLAYER* GetPlayer(void);
