#pragma once

#include "main.h"
#include "renderer.h"

class EnemyFactory;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

enum STATUS
{
	normal,
	warpwait,
	warp,
	
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
	//基礎情報
	D3DXVECTOR2 pos;		//現在座標
	D3DXVECTOR2 oldpos;		//１フレーム前の座標
	D3DXVECTOR2 warppos;	//ワープ地点の座標
	float		size;		//サイズ
	float       hitsize;    //ヒットサイズ
	D3DXVECTOR2	vel;		//速度ベクトル
	STATUS		status;		//プレイヤーの状態

	//ワープ関連
	int			warpframe;		//ワープ時の入力時間
	DWORD		warpRecast;		//ワープのリキャスト時間
	DWORD		warpStartRecast;//ワープのリキャストの初めの時間
	int         waitafterwarp;	//ワープ後の停止時間
	float		warppower;		//ワープ力
	float		gravity;		//重力
	int			warpFlag;		//ワープ回数フラグ

	//落下処理関連
	D3DXVECTOR2	dorpspeed;		//落下速度
	bool        LandingFlag;	//着地フラグ

	//アニメーション関連
	int			muki;
	int			animePtn;
	int			animeCounter;
	bool		move;

	//戦闘関連
	bool		mutekiflag;		//無敵フラグ
	int			mutekitime;		//無敵時間
	int			attackflag;		//アタックフラグ
	int			hp;				//HP

	//その他
	EnemyFactory* enemyfactory = nullptr;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void AdjustPlayer(D3DXVECTOR2 pos);

//ダメージ関数
void PlayerDamage(int num);

PLAYER* GetPlayer(void);
