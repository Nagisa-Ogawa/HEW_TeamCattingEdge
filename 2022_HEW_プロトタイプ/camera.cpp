/*==============================================================================

   カメラ制御 [camera.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "camera.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA_2D	g_Camera;	// カメラ用データ
static D3DXVECTOR2  g_Base;		// マップ全体の親座標

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = D3DXVECTOR2(0.0f, 60.0f);

	g_Base = D3DXVECTOR2(0.0f, 0.0f);
}

void UninitCamera(void)
{

}

void UpdateCamera(void)
{
	//カメラの座標を反転したものがベース座標になる
	g_Base = (g_Camera.pos * -1);
}

void DrawCamera(void)
{

}

CAMERA_2D* GetCamera(void)
{
	return &g_Camera;
}

D3DXVECTOR2 GetBase(void)
{
	return g_Base;
}
