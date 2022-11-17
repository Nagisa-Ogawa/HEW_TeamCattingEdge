/*==============================================================================

   プレイヤー弾の処理 [bullet.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "bullet.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "camera.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_SPEED (4.0f)//弾の速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_TextureNo;	// テクスチャ識別子
static BULLET g_Bullet[MAX_BULLET];//弾バッファ

static float g_RotateTable[4] =
{
	D3DX_PI,		//下向き(180°回転させる)
	(-D3DX_PI / 2),	//左向き(-90°回転させる)
	(+D3DX_PI / 2),	//右向き(+90°回転させる)
	0.0f,			//上向き
};

//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	//タイトル画面用テクスチャの読み込み
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/btleffect004.png");

	for (int i = 0; i < MAX_BULLET; i++)
	{
		g_Bullet[i].use = false;
	}
}

void UninitBullet(void)
{
	
}

void UpdateBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//可視フラグがオンであれば更新する
		if (g_Bullet[i].use == true)
		{
			//弾の座標更新
			g_Bullet[i].pos += g_Bullet[i].vel;

			//有効フレームを減らす
			g_Bullet[i].frame--;

			//有効フレームが0以下になったら可視フラグをオフにする
			if (g_Bullet[i].frame <= 0)
			{
				g_Bullet[i].use = false;
			}
		}
	}
}

void DrawBullet(void)
{
	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();

	for (int i = 0; i < MAX_BULLET; i++)
	{
		//可視フラグがオンであれば描画する
		if (g_Bullet[i].use == true)
		{
//			DrawSprite(g_TextureNo, g_Bullet[i].pos.x, g_Bullet[i].pos.y, 64.0f, 64.0f,
//				0.0f, 0.0f, 1.0f, 1.0f);
			DrawSpriteColorRotate(g_TextureNo, basePos.x + g_Bullet[i].pos.x, basePos.y + g_Bullet[i].pos.y, 32.0f, 32.0f,
				0.4f, 0.0f, 0.2f, 1.0f, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), g_RotateTable[g_Bullet[i].muki]);

		}
	}
}

void SetBullet(D3DXVECTOR2 pos, int muki)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//弾バッファの中から表示されていないものを探す
		if (g_Bullet[i].use == false)
		{
			g_Bullet[i].pos = pos;
			g_Bullet[i].muki = muki;

			//向きによって移動方向を決める
			switch (g_Bullet[i].muki)
			{
			case 0://下向き
				g_Bullet[i].vel = D3DXVECTOR2(0.0f, BULLET_SPEED);
				break;
			case 1://左向き
				g_Bullet[i].vel = D3DXVECTOR2(-BULLET_SPEED, 0.0f);
				break;
			case 2://右向き
				g_Bullet[i].vel = D3DXVECTOR2(BULLET_SPEED, 0.0f);
				break;
			case 3://上向き
				g_Bullet[i].vel = D3DXVECTOR2(0.0f, -BULLET_SPEED);
				break;
			}

			g_Bullet[i].height = 0.0f; //後で対応する
	
			//有効フレームをセット
			g_Bullet[i].frame = 200;

			//可視フラグオン
			g_Bullet[i].use = true;

			//弾を一つセットしたらループ終了
			break;
		}
	}
}

BULLET* GetBullet(void)
{
	return (&g_Bullet[0]);
}
