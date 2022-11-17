/*==============================================================================

   敵の発生制御 [enemyemitter.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "enemyemitter.h"
#include "enemy.h"


//敵の発生に必要な情報はゲームによって変わる
struct ENEMY_EMIT
{
	float	frame;	//敵の発生フレーム
	int		num;	//敵が発生する数
};

ENEMY_EMIT g_EnemyEmitTbl[] =
{
	{60.0f, 3},
	{200.0f, 6},
	{300.0f, 10},
	{400.0f, 5},
};

static int g_EmitCount = 0;	//敵の発生テーブルのインデックス
static float g_StageTimer = 0.0f;


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemyEmitter(void)
{
	//データのリセット
	g_EmitCount = 0;
	g_StageTimer = 0.0f;
}

void UninitEnemyEmitter(void)
{

}

void UpdateEnemyEmitter(void)
{
	//配列の要素数を計算する
	int numCrm = sizeof(g_EnemyEmitTbl) / sizeof(ENEMY_EMIT);

	//テーブルに登録されている敵がいなくなったら終了
	if (g_EmitCount >= numCrm)
		return;

	//現在のインデックスのタイマー値をステージタイマーが超えていたら
	//敵の発生タイミングが来たことになる
	if (g_EnemyEmitTbl[g_EmitCount].frame <= g_StageTimer)
	{
		//発生させる数の分だけループする
		for (int i = 0; i < g_EnemyEmitTbl[g_EmitCount].num; i++)
		{
			//敵の発生
			SetEnemy();
		}

		//インデックスを進める
		g_EmitCount++;
	}

	//時間経過
	g_StageTimer += 1.0f;
}

void DrawEnemyEmitter(void)
{

}
