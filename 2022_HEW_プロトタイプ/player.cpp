/*==============================================================================

   頂点管理 [player.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "fade.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "bullet.h"
#include "number.h"
#include "camera.h"
#include "inputx.h"
#include "Block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX 4	//必要な頂点の数

#define DIVIDE_X 3	//横の分割数
#define DIVIDE_Y 4	//縦の分割数

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)

#define PLAYER_DISP_X (SCREEN_WIDTH/2)	//プレイヤーの表示座標X
#define PLAYER_DISP_Y (SCREEN_HEIGHT/2)	//プレイヤーの表示座標Y

#define PLAYER_SPEED	4.0f //プレーヤーのスピード
#define WARP_POWER		300.0f //ワープの最大距離

#define TEST_CON 0

#define MONOSIZE 64.0f

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void PlayerStatusNormal(void);
void PlayerStatusWarpwait(void);
void PlayerStatusWarp(void);

//スティック情報取得関数
D3DXVECTOR2 GetLeftStick(int padNo);
D3DXVECTOR2 GetRightStick(int padNo);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER g_Player;

static int g_TextureNo = 0;//プレイヤー用テクスチャの識別子
static int g_TextureAttack;

static float g_AnimeTable[4] =
{
	0.0f,
	0.33333f,
	0.66667f,
	0.33333f,
};

static float g_MukiTable[4] =
{
	0.0f,	//下向き
	0.25f,	//左向き
	0.5f,	//右向き
	0.75f,	//上向き
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	//テクスチャの読み込み
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/pipo-xmaschara03.png");

	//データの初期化
	g_Player.pos = D3DXVECTOR2(200.0f, 800.0f);
	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.oldpos = g_Player.pos;
	g_Player.size = 120.0f;
	g_Player.status = normal;

	g_Player.warpframe = 0;
	g_Player.waitafterwarp = 0;
	g_Player.warppower = 0.0f;
	g_Player.gravity = 4.0f;
	g_Player.warpFlag = 0;//int

	g_Player.LandingFlag = false; //着地フラグ

	g_Player.muki = 0;
	g_Player.animePtn = 0;
	g_Player.animeCounter = 0;

	g_Player.move = false;

	SetNumber(32768); //スコアの描画

	g_Player.attackflag = 0;

	g_Player.hp = 10;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	g_Player.oldpos = g_Player.pos;
	DWORD result = 0;
	//プレーヤーの状態変化（ステータス）
	switch (g_Player.status)
	{
		//徒歩状態
	case normal:
		PlayerStatusNormal();
		result = hitChackNormalPlayer_Block(g_Player.vel);
		g_Player.vel.y += g_Player.gravity;
		break;
		//ワープ待機状態
	case warpwait:
		PlayerStatusWarpwait();
		result = hitChackNormalPlayer_Block(g_Player.vel);
		g_Player.vel.y += g_Player.gravity / 4;
		break;
		//ワ－プ状態
	case warp:
		PlayerStatusWarp();
		hitChackWarpPlayer_Block(g_Player.pos);
		break;
	}

	//当たり判定処理
	if (result & HIT_LEFT) 
	{
		if (g_Player.vel.x > 0.0)
			g_Player.vel.x = 0.0f;
	}
	if (result & HIT_RIGHT) 
	{
		if (g_Player.vel.x < 0.0)
			g_Player.vel.x = 0.0f;
	}

	result = hitChackNormalPlayer_Block(g_Player.vel);

	//落下させるか？処理
	if ((result & HIT_UP) == 0 && g_Player.LandingFlag == true)
	{
		g_Player.LandingFlag = false; 
	}

	//落下処理
	if (g_Player.LandingFlag == false)
	{
		if (g_Player.waitafterwarp > 0)
		{
			g_Player.waitafterwarp--;
			g_Player.vel.y = 0.0f;
		}
		else
		{
			if (result & HIT_UP)
			{
				g_Player.LandingFlag = true;
				g_Player.pos.y = GetBlockHeight() - (g_Player.size / 2);
				g_Player.vel.y = 0.0f;
				g_Player.waitafterwarp = 0;
			}
		}
	}
	else // 最終的に地面に触れている
	{
		g_Player.vel.y = 0.0f;
	}

	g_Player.pos += g_Player.vel;

	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);

	////アニメーションカウンターをカウントアップして、ウエイト値を超えたら
	//if (g_Player.animeCounter > 10)
	//{
	//	//アニメーションパターンを切り替える
	//	g_Player.animePtn++;
	//	//最後のアニメーションパターンを表示したらリセットする
	//	if (g_Player.animePtn >= 4)
	//		g_Player.animePtn = 0;

	//	//アニメーションカウンターのリセット
	//	g_Player.animeCounter = 0;
	//}
	//g_Player.animeCounter++;

	if (g_Player.pos.y > 1200.0f)
	{
		SceneTransition(SCENE_OVER_GAME);
	}

	//カメラ座標の更新
	CAMERA_2D* pCamera = GetCamera();
	pCamera->pos.x = g_Player.pos.x - PLAYER_DISP_X;
	pCamera->pos.y = g_Player.pos.y - PLAYER_DISP_Y - 60.0f;
	/*if (pCamera->pos.x < 0)
		pCamera->pos.x = 0;
	if (pCamera->pos.y < 0)
		pCamera->pos.y = 0;*/

}

void UpdateEndPlayer(void)
{
	// g_Player.oldpos = g_Player.pos;
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();

	//キャラクターの描画
	switch (g_Player.status)
	{
		//徒歩状態
	case normal:
		DrawSpriteColor(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + (g_Player.pos.y),
			g_Player.size, g_Player.size,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
		//ワープ待機状態
	case warpwait:
		DrawSpriteColor(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + (g_Player.pos.y),
			g_Player.size, g_Player.size,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		DrawSpriteColor(g_TextureNo,
			basePos.x + g_Player.warppos.x,
			basePos.y + (g_Player.warppos.y),
			g_Player.size, g_Player.size,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		break;
		//ワ－プ状態
	case warp:
		DrawSpriteColor(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + g_Player.pos.y,
			g_Player.size, g_Player.size,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	}

	//ベース座標を受け取る
	D3DXVECTOR2 BasePos = GetBase();


	if (g_Player.attackflag != 0)
	{
		if (g_Player.attackflag == 2)
		{
			DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x + 32.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.6f, 0.0f, -0.2f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f);
		}
		else
		{
			DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x - 32.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.4f, 0.0f, 0.2f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f);
		}

		g_Player.attackflag = 0;
	}
}

void AdjustPlayer(D3DXVECTOR2 pos)
{
	g_Player.pos += pos;
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

void PlayerStatusNormal(void)
{
	//左右移動
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	//ワープ処理(ワープ中ではない状態でスペースキーが押されたとき)
	if (0.0 != GetThumbRightX(TEST_CON) || 0.0 != GetThumbRightY(TEST_CON))
	{
		g_Player.status = warpwait;
	}

	//攻撃処理移行フラグ
	if (IsButtonPressedX(TEST_CON, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
	}
}

void PlayerStatusWarpwait(void)
{
	//左右移動
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	if (0.0 == GetThumbRightX(TEST_CON) && 0.0 == GetThumbRightY(TEST_CON))
	{
		g_Player.status = warp;
	}
	else
	{
		if (g_Player.warpframe < 60)
		{
			g_Player.warpframe++;
		}

		D3DXVECTOR2 Direction = GetRightStick(TEST_CON);

		D3DXVec2Normalize(&Direction, &Direction);

		g_Player.warppos.x = g_Player.pos.x + (Direction.x * -1) * (WARP_POWER * ((float)g_Player.warpframe / 60.0f));
		g_Player.warppos.y = g_Player.pos.y + (Direction.y) * (WARP_POWER * ((float)g_Player.warpframe / 60.0f));
	}
}

void PlayerStatusWarp(void)
{
	g_Player.pos.x = g_Player.warppos.x;
	g_Player.pos.y = g_Player.warppos.y;

	g_Player.warpframe = 0;

	g_Player.waitafterwarp = 9;

	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.status = normal;
}

D3DXVECTOR2 GetLeftStick(int padNo)
{
	return D3DXVECTOR2(GetThumbLeftX(padNo), GetThumbLeftY(padNo));
}

D3DXVECTOR2 GetRightStick(int padNo)
{
	return D3DXVECTOR2(GetThumbRightX(padNo), GetThumbRightY(padNo));
}


//void SetMap(MAPS map)
//{
//	g_Player.pos = D3DXVECTOR2(PLAYER_DISP_X, PLAYER_DISP_Y);
//	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);
//	g_Player.oldpos = g_Player.pos;
//	g_Player.status = normal;
//
//	g_Player.warpframe = 0;
//	g_Player.warppower = 0.0f;
//	g_Player.gravity = 0.2f;
//	g_Player.warpFlag = 0;
//
//	g_Player.muki = 0;
//	g_Player.animePtn = 0;
//	g_Player.animeCounter = 0;
//
//	g_Player.move = false;
//
//	g_Player.map = map;
//}
