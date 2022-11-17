
/*==============================================================================

   数値表示 [number.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "number.h"
#include "texture.h"
#include "sprite.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_TextureNo;	// テクスチャ識別子
static int g_Score;

//=============================================================================
// 初期化処理
//=============================================================================
void InitNumber(void)
{
	//タイトル画面用テクスチャの読み込み
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/number.png");
}

void UninitNumber(void)
{

}

void UpdateNumber(void)
{

}


void DrawNumber(void)
{
	int score = g_Score;;

	for (int i = 0; i < 5; i++)
	{
		//1の位の数値を取り出す
		int suuji = score % 10;;

		DrawSpriteLeftTop(g_TextureNo, 650.0f-(32.0f*i), 20.0f, 32.0f, 32.0f,
			0.1f*suuji, 0.0f, 0.1f, 1.0f);

		//数値全体を10で割る
		score /= 10;
	}
}

void SetNumber(int score)
{
	g_Score = score;
}