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
static int  g_StaffRollTextureNo;

enum RESULT_SCENE
{
	RESULT,
	STAFFROLL,
};

RESULT_SCENE g_ResultScene;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	g_ResultScene = RESULT_SCENE::RESULT;
	//タイトル画面用テクスチャの読み込み
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/ED.png");
	g_StaffRollTextureNo= LoadTexture((char*)"data/TEXTURE/endroll.png");

}

void UninitResult(void)
{
}

void UpdateResult(void)
{
	switch (g_ResultScene)
	{
	case RESULT:
		//スペースキーが押されたらゲームシーンへ移行する
		if (IsButtonTriggeredX(0, XINPUT_GAMEPAD_A))
		{
			g_ResultScene = STAFFROLL;
		}
		break;
	case STAFFROLL:
		if (IsButtonTriggeredX(0, XINPUT_GAMEPAD_A))
		{
			SetFadeColor(0.0f, 0.0f, 0.0f, 0.0f);//フェードカラーを黒に変える
			SceneTransition(SCENE_TITLE);
		}
		break;
	default:
		break;
	}
}

void DrawResult(void)
{
	switch (g_ResultScene)
	{
	case RESULT:
		DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case STAFFROLL:
		DrawSpriteLeftTop(g_StaffRollTextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}
}