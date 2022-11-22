
/*==============================================================================

   ゲーム画面 [game.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "polygon.h"
#include "player.h"
#include "goal.h"
#include "bg.h"
#include "Block.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "number.h"
#include "camera.h"
#include "enemyemitter.h"
#include "inputx.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_BGMNo;
static int g_TextureCloud = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	// 頂点管理の初期化処理
//	InitPolygon();

	// プレイヤーの初期化
	InitPlayer();

	// ゴールの初期化
	InitGoal();

	// 背景の初期化
	InitBG();

	// ステージの初期化
	InitBlock();

	// 弾の初期化
	InitBullet();

	// 敵の初期化
	//InitEnemy();

	//数値表示の初期化
	InitNumber();

	//カメラの初期化
	InitCamera();

	//敵の発生制御
	//InitEnemyEmitter();

	////敵の初期配置
	//for (int i = 0; i < 10; i++)
	//{
	//	D3DXVECTOR2 pos;
	//	pos.x = rand() % SCREEN_WIDTH;
	//	pos.y = rand() % SCREEN_HEIGHT;
	//	SetEnemy(pos, 0);
	//}

	//ゲーム用BGMの読み込み
	g_BGMNo = LoadSound((char*)"data/BGM/sample001.wav");

	//BGM再生(二つ目の引数はループ回数 負の値を指定すると無限ループ)
	//PlaySound(g_BGMNo, -1);
}

void UninitGame(void)
{
	//UninitEnemyEmitter();

	//カメラの終了処理
	UninitCamera();

	//数値表示の終了処理
	UninitNumber();

	//敵の終了処理
	//UninitEnemy();

	// 弾の終了処理
	UninitBullet();

	// ステージの終了処理
	UninitBlock();

	// 背景の終了処理
	UninitBG();

	// ゴールの終了処理
	UninitGoal();

	// プレイヤーの終了処理
	UninitPlayer();

	// 頂点管理の終了処理
//	UninitPolygon();

	//テクスチャの解放
//	UninitTexture();
}

void UpdateGame(void)
{
	// 頂点管理の更新処理
//	UpdatePolygon();

	//敵の発生制御
	//UpdateEnemyEmitter();


	// 敵の更新処理
	//UpdateEnemy();

	// プレイヤーの更新処理
	UpdatePlayer();

	// 弾の更新処理
	UpdateBullet();

	// ゴールの更新処理
	UpdateGoal();

	// ステージの更新処理
	UpdateBlock();

	// 背景の更新
	UpdateBG();

	//カメラの更新処理
	UpdateCamera();

	UpdateEndPlayer();

	//数値表示の更新処理
	UpdateNumber();

	//全てのアップデートが終わったら当たり判定を行う
	Collision_Bullet_Enemy();
}

void DrawGame(void)
{
	// 背景の描画
	DrawBG();

	// 頂点管理の描画処理
//	DrawPolygon();

	//// 弾の描画処理
	//DrawBullet();

	// 敵の描画処理
	//DrawEnemy();

	// ステージの描画処理
	DrawBlock();

	// プレイヤーの描画処理
	DrawPlayer();

	// ゴールの描画処理
	DrawGoal();

	// カメラの描画処理
	DrawCamera();

	// 弾の描画処理
	DrawBullet();

	//DrawEnemyEmitter();

	//ここからUIの描画
	//ゲーム関係の描画が全て終わってからUIの描画をする

	//数値表示の描画処理
	//DrawNumber();
}
