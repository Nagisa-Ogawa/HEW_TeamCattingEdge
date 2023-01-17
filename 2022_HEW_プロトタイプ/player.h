#pragma once

#include "main.h"
#include "game.h"

class EnemyFactory;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

enum STATUS
{
	normal,
	warpwait,
	warp,
	death,
	
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
	int			muki;	// 0:停止 1:右歩き 2:右走り 3:左歩き 4:左走り
	int			animeWalk;
	int			animeRun;
	int			animeAttack;
	int			animeDeath;
	int			animeCounterWalk;
	int			animeCounterRun;
	int			animeCounterAttack;
	int			animeCounterDeath;
	bool		move;

	//戦闘関連
	bool		mutekiflag;		//無敵フラグ
	int			mutekitime;		//無敵時間
	int			attackflag;		//アタックフラグ
	int			attackRecast;	//アタックリキャスト
	int			hp;				//HP

	//その他
	GAMESCENE	gamescene;
	EnemyFactory* enemyfactory = nullptr;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(GAMESCENE gamescene);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void AdjustPlayer(D3DXVECTOR2 pos);

//ダメージ関数
void PlayerDamage(int num);

PLAYER* GetPlayer(void);
D3DXVECTOR2 GetPlayerPos(void);
