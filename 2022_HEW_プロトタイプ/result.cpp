/*==============================================================================

   タイトル画面 [title.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "result.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
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
static int	g_TextureNo;	// テクスチャ識別子
static int	g_BGMNo;		// BGMの識別子

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	//タイトル画面用テクスチャの読み込み
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/ED.png");

	//タイトル用BGMの読み込み
	g_BGMNo = LoadSound((char*)"data/BGM/sample000.wav");

}

void UninitResult(void)
{
	//BGMストップ
	StopSound(g_BGMNo);
}

void UpdateResult(void)
{
	//スペースキーが押されたらゲームシーンへ移行する
	if (IsButtonPressedX(0, XINPUT_GAMEPAD_A))
	{
		SetFadeColor(0.0f, 0.0f, 0.0f, 0.0f);//フェードカラーを黒に変える
		SceneTransition(SCENE_TITLE);
	}
}

void DrawResult(void)
{
	DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);
}