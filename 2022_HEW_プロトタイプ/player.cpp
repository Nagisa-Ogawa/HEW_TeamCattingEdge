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
#include "ground.h"
#include "enemy_cloud.h"
#include "enemy_turret.h"
#include "enemy_bass.h"

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
#define WARP_POWER		400.0f //ワープの最大距離

#define TEST_CON 0

#define NUM_GROUND 11

#define MONOSIZE 64.0f

#define NUM_ENEMY_CLOUD 2

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void PlayerStatusNormal(void);
void PlayerStatusWarpwait(void);
void PlayerStatusWarp(void);

int HitCheckGround(D3DXVECTOR2 box1pos, float box1width, float box1height);
int HitCheckGroundBASS(D3DXVECTOR2 box1pos, float box1width, float box1height);
void  HitChackEnemy(float x, float y, float size, ENEMY_PROT* enemy);
void SetMap(MAPS map);

//スティック情報取得関数
D3DXVECTOR2 GetLeftStick(int padNo);
D3DXVECTOR2 GetRightStick(int padNo);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER g_Player;

ENEMY_CLOUD g_EC[NUM_ENEMY_CLOUD] = {
	ENEMY_CLOUD(2800.0f, 50.0f, 0.0f, 150.0f),
	ENEMY_CLOUD(2700.0f, 100.0f,45.0f, 150.0f),
};
ENEMY_TURRET g_e2(2600.0f - MONOSIZE, 500.0f - MONOSIZE * 2, 1);
ENEMY_BASS g_EB(600.0f, 100, 0.0f, 200.0f, 1);

static ground g_ground[NUM_GROUND];
static ground g_bassmap[3];

static int g_TextureNo = 0;//プレイヤー用テクスチャの識別子
static int g_TextureNoShadow = 0;//プレイヤー用影のテクスチャの識別子
static int g_TextureGround = 0;//プレイヤー用影のテクスチャの識別子
static int g_TextureAttack;
static int g_TextureCloud = 0;
static int g_TextureTurret = 0;
static int g_TextureBass = 0;


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

	//影テクスチャの読み込み
	g_TextureNoShadow = LoadTexture((char*)"data/TEXTURE/mapchip.png");

	g_TextureGround = LoadTexture((char*)"data/TEXTURE/wall_03.png");

	g_TextureAttack = LoadTexture((char*)"data/TEXTURE/btleffect001.png");

	g_TextureCloud = LoadTexture((char*)"data/TEXTURE/カボチャ.png");

	g_TextureTurret = LoadTexture((char*)"data/TEXTURE/キノコ02.png");

	g_TextureBass = LoadTexture((char*)"data/TEXTURE/おばけ04.png");

	//データの初期化
	g_Player.pos = D3DXVECTOR2(PLAYER_DISP_X, PLAYER_DISP_Y);
	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.oldpos = g_Player.pos;
	g_Player.status = normal;

	g_Player.warpframe = 0;
	g_Player.waitafterwarp = 0;
	g_Player.warppower = 0.0f;
	g_Player.gravity = 0.2f;
	g_Player.warpFlag = 0;//int

	g_Player.muki = 0;
	g_Player.animePtn = 0;
	g_Player.animeCounter = 0;

	g_Player.move = false;

	SetNumber(32768); //スコアの描画

	g_Player.attackflag = 0;

	g_Player.hp = 10;
	g_Player.map = tutorial;

	//フィールドの初期化 (X,Y,横幅、立幅)
	g_ground[0].SetVertex(0.0f, 500.0f, 960.0f, MONOSIZE * 2);
	g_ground[1].SetVertex(-1000.0f, -500.0f, 1000.0f, 1200.0f);
	g_ground[2].SetVertex(960.0f, 500.0f + MONOSIZE, 150.0f, MONOSIZE);
	g_ground[3].SetVertex(1100.0f, 500.0f, 80.0f, MONOSIZE * 2);
	g_ground[4].SetVertex(1180.0f, 500.0f - MONOSIZE, 300.0f, MONOSIZE * 3);
	g_ground[5].SetVertex(1600.0f, 500.0f - MONOSIZE, 200.0f, MONOSIZE * 3);
	g_ground[6].SetVertex(1800.0f, 500.0f + MONOSIZE, 150.0f, MONOSIZE);
	g_ground[7].SetVertex(1950.0f, 500.0f - MONOSIZE, 650.0f, MONOSIZE * 3);
	g_ground[8].SetVertex(1950.0f, -500.0f, 100.0f, 950.0f);
	g_ground[9].SetVertex(2600.0f, 500.0f - MONOSIZE * 2, 500.0f, MONOSIZE * 4);
	g_ground[10].SetVertex(3100.0f, -500.0f, 1000.0f, 1200.0f);
	
	//ボスマップの初期化
	g_bassmap[0].SetVertex(-1000.0f,-500.0f,1000.0,1200.0f);
	g_bassmap[1].SetVertex(960.0f, -500.0f, 1000.0, 1200.0f);
	g_bassmap[2].SetVertex(0.0f, 500.0f, 960.0, 500.0f);

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
	//プレーヤーの状態変化（ステータス）
	switch (g_Player.status)
	{
		//徒歩状態
	case normal:
		PlayerStatusNormal();
		break;
		//ワープ待機状態
	case warpwait:
		PlayerStatusWarpwait();
		break;
		//ワ－プ状態
	case warp:
		PlayerStatusWarp();
		break;
	}

	//敵の更新

	for (int i = 0; i < NUM_ENEMY_CLOUD; i++)
	{
		g_EC[i].Update();
	}

	if (g_e2.GetLive())
		g_e2.Update();

	if (g_EB.GetLive())
		g_EB.Update();

	else
	{
		SceneTransition(SCENE_RESULT);
	}

	////ジャンプ中の処理
	//if (g_Player.jumpFlag == true)
	//{
	//	//高さの更新
	//	g_Player.height += g_Player.jumpPower;

	//	//ジャンプ力の減衰
	//	g_Player.jumpPower -= g_Player.gravity;

	//	//地面に着地した場合
	//	if (g_Player.height <= 0.0f)
	//	{
	//		//高さを0に戻してジャンプ中フラグを落とす
	//		g_Player.height = 0.0f;
	//		g_Player.jumpFlag = false;
	//	}
	//}

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

	//衝突判定
	int a ;

	//マップ選択
	switch (g_Player.map)
	{
	case tutorial:
		a = (HitCheckGround(g_Player.pos, 64.0f, 64.0f));
		break;
	case bass:
		a = (HitCheckGroundBASS(g_Player.pos, 64.0f, 64.0f));
		break;
	default:
		break;
	}

	switch (a)
	{
	case 1://床にいる場合
		g_Player.pos.y = g_Player.oldpos.y;
		break;
	case 2://壁にぶつかる場合
		g_Player.pos.x = g_Player.oldpos.x;

		if (g_Player.status == warpwait)
		{
			g_Player.pos.y += 1.0f;
		}
		else
		{
			g_Player.pos.y += 10.0f;
		}
		break;
	case 3://壁の中
		g_Player.pos = g_Player.oldpos;
		break;
	case 0://どこにも触れていない
		if (g_Player.waitafterwarp <= 0)
		{
			if (g_Player.status == warpwait)
			{
				g_Player.pos.y += 1.0f;
			}
			else
			{
				g_Player.pos.y += 10.0f;
			}
		}
		else
		{
			g_Player.waitafterwarp--;
		}

		break;
	}
	
	if (IsButtonPressedX(0, XINPUT_GAMEPAD_Y))
		SetMap(tutorial);

	if (IsButtonPressedX(0, XINPUT_GAMEPAD_X))
		SetMap(bass);

	if (g_Player.pos.y > 1000.0f)
	{
		SceneTransition(SCENE_OVER_GAME);
	}

	g_Player.oldpos = g_Player.pos;

	//カメラ座標の更新
	CAMERA_2D* pCamera = GetCamera();
	pCamera->pos.x = g_Player.pos.x - PLAYER_DISP_X;
	pCamera->pos.y = g_Player.pos.y - PLAYER_DISP_Y - 60.0f;
	/*if (pCamera->pos.x < 0)
		pCamera->pos.x = 0;
	if (pCamera->pos.y < 0)
		pCamera->pos.y = 0;*/

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();

	////影の描画
	//DrawSpriteColor(g_TextureNoShadow,
	//	basePos.x + (g_Player.pos.x + 2.0f),
	//	basePos.y + (g_Player.pos.y + 30.0f),
	//	64.0f, 64.0f,
	//	0.875f,
	//	0.875f,
	//	0.125f,
	//	0.125f,
	//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

	//キャラクターの描画
	switch (g_Player.status)
	{
		//徒歩状態
	case normal:
		DrawSpriteColorRotate(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + (g_Player.pos.y),
			64.0f, 64.0f,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			0.0f);
		break;
		//ワープ待機状態
	case warpwait:
		DrawSpriteColorRotate(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + (g_Player.pos.y),
			64.0f, 64.0f,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			0.0f);

		DrawSpriteColorRotate(g_TextureNo,
			basePos.x + g_Player.warppos.x,
			basePos.y + (g_Player.warppos.y),
			64.0f, 64.0f,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),
			0.0f);
		break;
		//ワ－プ状態
	case warp:
		DrawSpriteColorRotate(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + (g_Player.pos.y),
			64.0f, 64.0f,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		break;
	}

	//ベース座標を受け取る
	D3DXVECTOR2 BasePos = GetBase();


	//グラウンドの描画 と　敵の描画
	switch (g_Player.map)
	{
	case tutorial:
		for (int i = 0; i < NUM_GROUND; i++)
		{
			DrawSpriteLeftTop(g_TextureGround, BasePos.x + g_ground[i].GetVertex0(), BasePos.y + g_ground[i].GetVertex1(), g_ground[i].GetVertex2(), g_ground[i].GetVertex3(), 0.0f, 0.0f, 1.0f, 1.0f);
		}

		for (int i = 0; i < NUM_ENEMY_CLOUD; i++)
		{
			if (g_EC[i].GetLive())
				DrawSpriteLeftTop(g_TextureCloud, BasePos.x + g_EC[i].GetPosX(), BasePos.y + g_EC[i].GetPosY(), g_EC[i].GetSize(), g_EC[i].GetSize(), 0.08f, 0.14f, 0.18f, 0.1f);
		}

		if (g_e2.GetLive())
			DrawSpriteLeftTop(g_TextureTurret, BasePos.x + g_e2.GetPosX(), BasePos.y + g_e2.GetPosY(), g_e2.GetSize(), g_e2.GetSize(), 0.00f, 0.25f, 0.33f, 0.25f);
		break;
	case bass:
		for (int i = 0; i < 3; i++)
		{
			DrawSpriteLeftTop(g_TextureGround, BasePos.x + g_bassmap[i].GetVertex0(), BasePos.y + g_bassmap[i].GetVertex1(), g_bassmap[i].GetVertex2(), g_bassmap[i].GetVertex3(), 0.0f, 0.0f, 1.0f, 1.0f);
		}

		if (g_EB.GetLive())
			DrawSpriteLeftTop(g_TextureBass, BasePos.x + g_EB.GetPosX(), BasePos.y + g_EB.GetPosY(), g_EB.GetSize(), g_EB.GetSize(), 0.00f, 0.25f, 0.33f, 0.25f);
		break;
	default:
		break;
	}

	if (g_Player.attackflag != 0)
	{
		if (g_Player.attackflag == 2)
		{
			DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x + 32.0f,
				basePos.y + (g_Player.pos.y),
				64.0f, 64.0f,
				0.6f, 0.0f, -0.2f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f);
		}
		else
		{
			DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x - 32.0f,
				basePos.y + (g_Player.pos.y),
				64.0f, 64.0f,
				0.4f, 0.0f, 0.2f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f);
		}

		g_Player.attackflag = 0;
	}
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

void PlayerStatusNormal(void)
{
	//左右移動
	g_Player.pos.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	//ワープ処理(ワープ中ではない状態でスペースキーが押されたとき)
	if (0.0 != GetThumbRightX(TEST_CON) || 0.0 != GetThumbRightY(TEST_CON))
	{
		g_Player.status = warpwait;
	}

	if (IsButtonPressedX(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		switch (g_Player.map)
		{
		case tutorial:
			if (GetThumbLeftX(TEST_CON) <= 0.0f)
			{
				g_Player.attackflag = 1;
				for (int i = 0; i < NUM_ENEMY_CLOUD; i++)
				{
					HitChackEnemy(g_Player.pos.x - MONOSIZE, g_Player.pos.y, MONOSIZE, &g_EC[i]);
				}
				HitChackEnemy(g_Player.pos.x - MONOSIZE, g_Player.pos.y, MONOSIZE, &g_e2);
			}
			else
			{
				g_Player.attackflag = 2;
				for (int i = 0; i < NUM_ENEMY_CLOUD; i++)
				{
					HitChackEnemy(g_Player.pos.x + MONOSIZE, g_Player.pos.y, MONOSIZE, &g_EC[i]);
				};
				HitChackEnemy(g_Player.pos.x + MONOSIZE, g_Player.pos.y, MONOSIZE, &g_e2);
			}
			break;
		case bass:
			if (GetThumbLeftX(TEST_CON) <= 0.0f)
			{
				g_Player.attackflag = 1;
				HitChackEnemy(g_Player.pos.x - MONOSIZE, g_Player.pos.y, MONOSIZE, &g_EB);
			}
			else
			{
				g_Player.attackflag = 2;
				HitChackEnemy(g_Player.pos.x + MONOSIZE, g_Player.pos.y, MONOSIZE, &g_EB);
			}
			break;
		default:
			break;
		}
	}
}

void PlayerStatusWarpwait(void)
{
	//左右移動
	g_Player.pos.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

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

		g_Player.vel = GetRightStick(TEST_CON);

		D3DXVec2Normalize(&g_Player.vel, &g_Player.vel);

		g_Player.warppos.x = g_Player.pos.x + (g_Player.vel.x * -1) * (WARP_POWER * ((float)g_Player.warpframe / 60.0f));
		g_Player.warppos.y = g_Player.pos.y + (g_Player.vel.y) * (WARP_POWER * ((float)g_Player.warpframe / 60.0f));
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

//フィールドとの接触を判定する
//1:床に接触
//2:壁に接触
//3:壁の中
//0:接触無し
int HitCheckGround(D3DXVECTOR2 box1pos, float box1width, float box1height)
{
	bool frag1 = false;
	bool frag2 = false;

	float box1Xmin = box1pos.x - (box1width / 2);
	float box1Xmax = box1pos.x + (box1width / 2);
	float box1Ymin = box1pos.y - (box1height / 2);
	float box1Ymax = box1pos.y + (box1height / 2);

	for (int i = 0; i < NUM_GROUND; i++)
	{
		float box2Xmin = g_ground[i].GetVertex0();
		float box2Xmax = g_ground[i].GetVertex0() + g_ground[i].GetVertex2();
		float box2Ymin = g_ground[i].GetVertex1();
		float box2Ymax = g_ground[i].GetVertex1() + g_ground[i].GetVertex3();

		if (box1Xmin < box2Xmax)
		{
			if (box1Xmax > box2Xmin)
			{
				if (box1Ymin < box2Ymax)
				{
					if (box1Ymax > box2Ymin)
					{

						if (box2Ymin >= box1Ymax - 10.0f)
						{
							frag1 = true;
						}
						else if (box2Xmin + 10.0f >= box1Xmax || box2Xmax - 10.0f <= box1Xmin)
						{
							frag2 = true;
						}

						if (frag1 == false && frag2 == false)
						{
							return 3;
						}
					}
				}
			}
		}
	}

	if (frag1 == true && frag2 == true)
	{
		return 3;
	}
	else if (frag1 == true)
	{
		return 1;
	}
	else if (frag2 == true)
	{
		return 2;
	}
}

int HitCheckGroundBASS(D3DXVECTOR2 box1pos, float box1width, float box1height)
{
	bool frag1 = false;
	bool frag2 = false;

	float box1Xmin = box1pos.x - (box1width / 2);
	float box1Xmax = box1pos.x + (box1width / 2);
	float box1Ymin = box1pos.y - (box1height / 2);
	float box1Ymax = box1pos.y + (box1height / 2);

	for (int i = 0; i < 3; i++)
	{
		float box2Xmin = g_bassmap[i].GetVertex0();
		float box2Xmax = g_bassmap[i].GetVertex0() + g_bassmap[i].GetVertex2();
		float box2Ymin = g_bassmap[i].GetVertex1();
		float box2Ymax = g_bassmap[i].GetVertex1() + g_bassmap[i].GetVertex3();

		if (box1Xmin < box2Xmax)
		{
			if (box1Xmax > box2Xmin)
			{
				if (box1Ymin < box2Ymax)
				{
					if (box1Ymax > box2Ymin)
					{

						if (box2Ymin >= box1Ymax - 10.0f)
						{
							frag1 = true;
						}
						else if (box2Xmin + 10.0f >= box1Xmax || box2Xmax - 10.0f <= box1Xmin)
						{
							frag2 = true;
						}

						if (frag1 == false && frag2 == false)
						{
							return 3;
						}
					}
				}
			}
		}
	}

	if (frag1 == true && frag2 == true)
	{
		return 3;
	}
	else if (frag1 == true)
	{
		return 1;
	}
	else if (frag2 == true)
	{
		return 2;
	}
}

void SetMap(MAPS map)
{
	g_Player.pos = D3DXVECTOR2(PLAYER_DISP_X, PLAYER_DISP_Y);
	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.oldpos = g_Player.pos;
	g_Player.status = normal;

	g_Player.warpframe = 0;
	g_Player.warppower = 0.0f;
	g_Player.gravity = 0.2f;
	g_Player.warpFlag = 0;

	g_Player.muki = 0;
	g_Player.animePtn = 0;
	g_Player.animeCounter = 0;

	g_Player.move = false;

	g_Player.map = map;
}

void  HitChackEnemy(float x, float y,float size,ENEMY_PROT* enemy) 
{
	float box1Xmin = x - (size / 2);
	float box1Xmax = x + (size / 2);
	float box1Ymin = y - (size / 2);
	float box1Ymax = y + (size / 2);

	for (int i = 0; i < 3; i++)
	{
		float box2Xmin = enemy->GetPosX();
		float box2Xmax = enemy->GetPosX() + enemy->GetSize();
		float box2Ymin = enemy->GetPosY();
		float box2Ymax = enemy->GetPosY() + enemy->GetSize();

		if (box1Xmin < box2Xmax)
		{
			if (box1Xmax > box2Xmin)
			{
				if (box1Ymin < box2Ymax)
				{
					if (box1Ymax > box2Ymin)
					{
						enemy->Damage(1);
					}
				}
			}
		}
	}
}