/*==============================================================================

   プレイヤー管理 [player.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "texture.h"
#include "sprite.h"
#include "number.h"
#include "camera.h"
#include "inputx.h"
#include "Block.h"
#include "game.h"
#include "EnemyFactory.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX 4	//必要な頂点の数

#define DIVIDE_X 8	//横の分割数
#define DIVIDE_Y 7	//縦の分割数

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)

#define PLAYER_SPEED	4.0f //プレーヤーのスピード

#define TEST_CON 0

#define MONOSIZE 64.0f

#define WARPRECAST 3000//3000

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void PlayerStatusNormal(void);
void PlayerStatusWarpwait(void);
void PlayerStatusWarp(void);
void PlayerStatusAttack(void);
void PlayerStatusDeath(void);
void DrawWarpKaminari(D3DXVECTOR2 beforepos, D3DXVECTOR2 afterpos);

//スティック情報取得関数
D3DXVECTOR2 GetLeftStick(int padNo);
D3DXVECTOR2 GetRightStick(int padNo);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER g_Player;
static D3DXVECTOR2 g_Direction;

static int	g_SE_wapu;		// SEの識別子
static int	g_SE_attack;		// SEの識別子
static int	g_SE_walk;		// SEの識別子
static int	g_SE_dash;		// SEの識別子
static int	g_SE_damage;		// SEの識別子
static bool g_Frag_warpTextrue = false;
static bool g_Frag_dashSE = false;
static bool g_Frag_damageSE = false;

static int g_TextureLeft = 0;	//プレイヤー用テクスチャの識別子
static int g_TextureRight = 0;	//プレイヤー用テクスチャの識別子
static int g_TextureWarp = 0;	//プレイヤー用テクスチャの識別子
static int g_TextureAttack = 0;	//プレイヤー用テクスチャの識別子

static float g_AnimeTable[8] =
{
	0.0f,
	0.125f,
	0.25f,
	0.375f,
	0.5f,
	0.625f,
	0.75f,
	0.875f,
};

static float g_AnimeTableTate[7]=
{
	PATTERN_HEIGHT * 0.0f,	//待機
	PATTERN_HEIGHT * 1.0f,	//歩き
	PATTERN_HEIGHT * 2.0f,	//走り
	PATTERN_HEIGHT * 3.0f,	//攻撃
	PATTERN_HEIGHT * 4.0f,	//ワープ
	PATTERN_HEIGHT * 5.0f,	//飛び
	PATTERN_HEIGHT * 6.0f,	//死亡
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(GAMESCENE gamescene)
{
	//テクスチャの読み込み
	g_TextureLeft  = LoadTexture((char*)"data/TEXTURE/hituzi_L.png");
	g_TextureRight = LoadTexture((char*)"data/TEXTURE/hituzi_R.png");
	g_TextureWarp = LoadTexture((char*)"data/TEXTURE/ワープ.png");
	g_TextureAttack = LoadTexture((char*)"data/TEXTURE/ワープ.png");

	//データの初期化

	//基礎情報の初期化
	g_Player.pos = D3DXVECTOR2(200.0f, 800.0f);
	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.oldpos = g_Player.pos;
	g_Player.size = 120.0f;
	g_Player.hitsize = g_Player.size - 2.0f;
	g_Player.status = normal;

	//ワープ関連情報の初期化
	g_Player.warpframe = 0;
	g_Player.warpRecast = 0;
	g_Player.warpStartRecast = 0;
	g_Player.waitafterwarp = 0;
	g_Player.warppower = 420.0f; //最大距離
	g_Player.warpFlag = 3;//int
	g_Player.gravity = 0.6f;
	g_Player.beforewarppos = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.afterwarppos = D3DXVECTOR2(0.0f, 0.0f);

	//落下処理関連の初期化
	g_Player.dorpspeed = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.LandingFlag = false; //着地フラグ

	//アニメーション関連の初期化
	g_Player.muki = 0;
	g_Player.animeWalk  = 0;
	g_Player.animeAttack = 0;
	g_Player.animeDeath = 0;
	g_Player.animeCounterRun = 0;
	g_Player.animeCounterWalk = 0;

	g_Player.move = false;

	//戦闘関連情報の初期化
	g_Player.mutekiflag = false;
	g_Player.mutekitime = 0;
	g_Player.attackflag = 0;
	g_Player.attackRecast = 0;
	g_Player.hp = 10;

	//音関連の初期化
	g_SE_wapu = LoadSound((char*)"data/SE/wa-pu.wav");
	SetVolume(g_SE_wapu, 0.5f);
	g_SE_attack = LoadSound((char*)"data/SE/player_attack.wav");
	SetVolume(g_SE_attack, 0.5f);
	g_SE_walk = LoadSound((char*)"data/SE/player_walk.wav");
	SetVolume(g_SE_walk, 0.5f);
	g_SE_dash = LoadSound((char*)"data/SE/kasya-hasiri.wav");
	SetVolume(g_SE_dash, 1.0f);
	g_SE_damage = LoadSound((char*)"data/SE/player_damage.wav");
	SetVolume(g_SE_damage, 0.8f);

	//その他の初期化
	g_Player.enemyfactory = GetEnemyFactory();
	g_Player.gamescene = gamescene;

	switch (gamescene)
	{
	case GAMESCENE_STAGE_TENGU:
		break;
	case GAMESCENE_BASS_TENGU:
		break;
	case GAMESCENE_STAGE_KASYA:
		g_Player.pos = D3DXVECTOR2(400.0f, 540.0f);
		break;
	case GAMESCENE_BASS_KASYA:
		break;
	case GAMESCENE_BASS_FUJIN:
		g_Player.pos = D3DXVECTOR2(960.0f, 540.0f);
		break;
	case GAMESCENE_BASS_FINAL:
		g_Player.pos = D3DXVECTOR2(960.0f, 540.0f);
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}

	SetNumber(32768); //スコアの描画

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	StopSound(g_SE_dash);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (g_Frag_damageSE)
	{
		PlaySound(g_SE_damage, 0);

		g_Frag_damageSE = false;
	}

	//１フレーム前のポジションを更新
	g_Player.oldpos = g_Player.pos;

	DWORD result = 0;
	//プレーヤーの状態変化（ステータス）
	switch (g_Player.status)
	{
		//徒歩状態
	case normal:
		PlayerStatusNormal();
		result = hitChackNormalPlayer_Block(g_Player.vel);
		g_Player.dorpspeed.y += g_Player.gravity;
		break;
		//ワープ待機状態
	case warpwait:
		PlayerStatusWarpwait();
		result = hitChackNormalPlayer_Block(g_Player.vel);
		g_Player.dorpspeed.y = 1.0f;
		break;
		//ワ－プ状態
	case warp:
		PlayerStatusWarp();
		hitChackWarpPlayer_Block(g_Player.pos);
		break;
	case attack:
		PlayerStatusAttack();
		result = hitChackNormalPlayer_Block(g_Player.vel);

		g_Player.dorpspeed.y += g_Player.gravity;
		break;
	case death:
		PlayerStatusDeath();
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

	result = hitChackNormalPlayer_Block(g_Player.dorpspeed);

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
			g_Player.dorpspeed.y = 0.0f;
		}
		else
		{
			if (result & HIT_UP)
			{
				g_Player.LandingFlag = true;
				g_Player.pos.y = GetBlockHeight() - (g_Player.size / 2);
				g_Player.dorpspeed.y = 0.0f;
				g_Player.waitafterwarp = 0;
			}
		}
	}
	else // 最終的に地面に触れている
	{
		g_Player.dorpspeed.y = 0.0f;
	}

	g_Player.pos += g_Player.vel;
	g_Player.pos += g_Player.dorpspeed;

	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);

	//アニメーションカウンターをカウントアップして、ウエイト値を超えたら
	if (g_Player.animeCounterWalk > 10)
	{
		//アニメーションパターンを切り替える
		g_Player.animeWalk++;
		//最後のアニメーションパターンを表示したらリセットする
		if (g_Player.animeWalk >= 8)
			g_Player.animeWalk = 0;

		//アニメーションカウンターのリセット
		g_Player.animeCounterWalk = 0;
	}
	g_Player.animeCounterWalk++;

	//走る
	if (g_Player.animeCounterRun > 5)
	{
		//アニメーションパターンを切り替える
		g_Player.animeRun++;
		//最後のアニメーションパターンを表示したらリセットする
		if (g_Player.animeRun >= 8)
			g_Player.animeRun = 0;

		//アニメーションカウンターのリセット
		g_Player.animeCounterRun = 0;
	}
	g_Player.animeCounterRun++;

	//無敵判定の処理
	if (g_Player.mutekiflag == true)
	{
		g_Player.mutekitime++;

		if (g_Player.mutekitime >= 120)
		{
			g_Player.mutekitime = 0;
			g_Player.mutekiflag = false;
		}
	}

	if (g_Player.pos.y > 1200.0f)
	{
		SetGameScene(GAMESCENE_PICTURE_OVERGAME);
	}

	//攻撃リキャスト処理
	if (g_Player.attackflag > 0)
	{
		g_Player.attackRecast++;

		if (g_Player.attackRecast == 30)
		{
			g_Player.attackflag = 3;
			g_Player.animeCounterAttackKaminari = 0;
			g_Player.animeAttackKaminari = 0;
		}

		if (g_Player.attackRecast >= 60)
		{
			g_Player.attackflag = 0;
			g_Player.attackRecast = 0;
		}
	}

	//死亡確認
	if (g_Player.hp <= 0)
	{
		g_Player.status = death;
	}

	//カメラ座標の更新
	CAMERA_2D* pCamera = GetCamera();

	switch (g_Player.gamescene)
	{
	case GAMESCENE_STAGE_TENGU:
		pCamera->pos.x = g_Player.pos.x - SCREEN_WIDTH / 2;
		if (pCamera->pos.x <= 0.0f)
		{
			pCamera->pos.x = 0.0f;
		}
		if (pCamera->pos.x >= 5460.0f)
		{
			pCamera->pos.x = 5460.0f;
		}
		break;
	case GAMESCENE_STAGE_KASYA:
		if (pCamera->pos.x <= 13380.0f)
			pCamera->pos.x += 3.0f;//横スクロールの速度 2.0f->3.0f

		//ここで火車ボスステージへシーン移行
		if (g_Player.pos.x >= 15360.0f)
		{
			SetGameScene(GAMESCENE_PICTURE_STAGE02_BOSS	);
		}
		break;
	default:
		//pCamera->pos.x = SCREEN_WIDTH / 2;
		break;
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();

	//ワープ時の雷の描画
	if (g_Frag_warpTextrue == true)
	{
		DrawWarpKaminari(g_Player.beforewarppos, g_Player.pos);
	}

	if (g_Player.mutekiflag == true)
	{
		float color = (float)(g_Player.mutekitime % 30);

		switch (g_Player.status)
		{
			//徒歩状態
		case normal:
			if (g_Player.waitafterwarp > 0)
			{
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[1],
					g_AnimeTableTate[4],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				switch (g_Player.muki)
				{
				case 0:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[0],
						g_AnimeTableTate[0],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 1:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 2:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 3:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 4:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));					
					break;
				default:
					break;
				}
			}
			break;
			//ワープ待機状態
		case warpwait:
			//本体の描画
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[0],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));

			//ワープ先の描画
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.warppos.x,
				basePos.y + (g_Player.warppos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[1],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
			//ワ－プ状態
		case warp:
			break;
		case attack:
			switch (g_Player.muki)
			{
			case 2:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
				break;
			case 4:
				DrawSpriteColor(g_TextureLeft,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
				break;
			default:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
				break;
			}
			break;
		case death:
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[g_Player.animeDeath],
				g_AnimeTableTate[6],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		}
	}
	else
	{
		switch (g_Player.status)
		{
		//徒歩状態
		case normal:
			if (g_Player.waitafterwarp > 0)
			{
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[1],
					g_AnimeTableTate[4],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				switch (g_Player.muki)
				{
				case 0:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[0],
						g_AnimeTableTate[0],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 1:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 2:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 3:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 4:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				default:
					break;
				}
			}
			break;
		//ワープ待機状態
		case warpwait:
			//本体の描画
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[0],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			//ワープ先の描画
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.warppos.x,
				basePos.y + (g_Player.warppos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[1],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
			//ワ－プ状態
		case warp:
			break;
		case attack:
			switch (g_Player.muki)
			{
			case 2:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			case 4:
				DrawSpriteColor(g_TextureLeft,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			default:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
			break;
		case death:
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[g_Player.animeDeath],
				g_AnimeTableTate[6],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		}
	}

	//ベース座標を受け取る
	D3DXVECTOR2 BasePos = GetBase();

	if (g_Player.attackflag != 0)
	{
		float Width = 1.0f / 13.0f;

		if (g_Player.animeCounterAttackKaminari > 4)
		{
			//アニメーションパターンを切り替える
			g_Player.animeAttackKaminari++;
			//最後のアニメーションパターンを表示したらリセットする
			if (g_Player.animeAttackKaminari >= 7)
			{
				g_Player.animeAttackKaminari = 0;
			}
			//アニメーションカウンターのリセット
			g_Player.animeCounterAttackKaminari = 0;

		}
		g_Player.animeCounterAttackKaminari++;

		switch (g_Player.attackflag)
		{
		case 1://左
		/*	DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x - 120.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.4f, 0.0f, 0.2f, 1.0f,
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
				0.0f);*/

			DrawSpriteLeftTop(g_TextureAttack,
				basePos.x + g_Player.pos.x - 360.0f,
				basePos.y + g_Player.pos.y - 1020.0f,
				480.0f, 1080.0f,
				Width * (6 + g_Player.animeAttackKaminari), 0.0f, Width, 1.0f);
			break;
		case 2://右
		/*	DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x + 120.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.6f, 0.0f, -0.2f, 1.0f,
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
				0.0f);*/

			DrawSpriteLeftTop(g_TextureAttack,
				basePos.x + g_Player.pos.x - 120.0f,
				basePos.y + g_Player.pos.y - 1020.0f,
				480.0f, 1080.0f,
				Width * (6 + g_Player.animeAttackKaminari), 0.0f, Width, 1.0f);
			break;
		default:
			break;
		}
	}

}

void PlayerDamage(int num)
{
	g_Player.hp -= num;

	g_Player.mutekiflag = true;

	g_Frag_damageSE = true;
}

void PlayerStatusNormal(void)
{
	//左右移動
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	if (g_Player.vel.x != 0.0f && g_Frag_dashSE == false)
	{
		PlaySound(g_SE_dash, -1);

		g_Frag_dashSE = true;
	}
	else if (g_Player.vel.x == 0.0f)
	{
		StopSound(g_SE_dash);

		g_Frag_dashSE = false;
	}

	if (GetThumbLeftX(TEST_CON) > 0.8f)
	{
		g_Player.muki = 2;
	}
	else if (GetThumbLeftX(TEST_CON) > 0.1f)
	{
		g_Player.muki = 1;
	}
	else if (GetThumbLeftX(TEST_CON) < -0.8f)
	{
		g_Player.muki = 4;
	}
	else if (GetThumbLeftX(TEST_CON) < -0.1f)
	{
		g_Player.muki = 3;
	}
	else
	{
		g_Player.muki = 0;
	}

	//ワープ処理
	if (0.0 != GetThumbRightX(TEST_CON) || 0.0 != GetThumbRightY(TEST_CON))
	{
		if (g_Player.warpFlag != 0)
		{
			g_Player.status = warpwait;
		}
	}

	//ワープリキャスト処理
	if (g_Player.warpFlag < 3)
	{
		g_Player.warpRecast = timeGetTime();

		if (g_Player.warpRecast - g_Player.warpStartRecast >= WARPRECAST)//3.000
		{
			g_Player.warpFlag++;

			if (g_Player.warpFlag == 3)
			{
				g_Player.warpStartRecast = 0;
			}
			else
			{
				g_Player.warpStartRecast = timeGetTime();
			}
		}
	}

	//攻撃処理移行フラグ
	if (IsButtonPressedX(TEST_CON, XINPUT_GAMEPAD_RIGHT_SHOULDER) && g_Player.attackflag == 0)
	{
		g_Player.enemyfactory->CollisoinAttacktoEnemy(D3DXVECTOR2(g_Player.pos.x + 120.0f, g_Player.pos.y));

		g_Player.attackflag = 2;
		g_Player.muki = 2;
		g_Player.status = attack;

		g_Player.animeCounterAttack = 0;
		g_Player.animeAttack = 0;

		PlaySound(g_SE_attack, 0);
	}

	if (IsButtonPressedX(TEST_CON, XINPUT_GAMEPAD_LEFT_SHOULDER) && g_Player.attackflag == 0)
	{
		g_Player.enemyfactory->CollisoinAttacktoEnemy(D3DXVECTOR2(g_Player.pos.x - 120.0f, g_Player.pos.y));

		g_Player.attackflag = 1;
		g_Player.muki = 4;
		g_Player.status = attack;
		g_Player.animeCounterAttack = 0;
		g_Player.animeAttack = 0;

		PlaySound(g_SE_attack, 0);
	}
}

void PlayerStatusWarpwait(void)
{
	//左右移動
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	StopSound(g_SE_dash);

	g_Frag_dashSE = false;

	if (0.0 == GetThumbRightX(TEST_CON) && 0.0 == GetThumbRightY(TEST_CON))
	{
		g_Player.status = warp;
	}
	else
	{
		if (g_Player.warpframe < 30)//692,693の数値も一緒に変更が必要
		{
			g_Player.warpframe++;
		}

		D3DXVECTOR2 nowvel = GetRightStick(TEST_CON);
		D3DXVECTOR2 oldvel = g_Direction;

		if (oldvel.x < 0.0f)
		{
			oldvel.x += 0.1f;
		}
		else if(oldvel.x > 0.0f)
		{
			oldvel.x -= 0.1f;
		}

		if (oldvel.y < 0.0f)
		{
			oldvel.y += 0.1f;
		}
		else if (oldvel.y > 0.0f)
		{
			oldvel.y -= 0.1f;
		}

		if (D3DXVec2Length(&oldvel) <= D3DXVec2Length(&nowvel))
		{
			g_Direction = nowvel;
		}
		
		D3DXVECTOR2 warpvel = g_Direction;

		D3DXVec2Normalize(&warpvel, &warpvel);

		g_Player.warppos.x = g_Player.pos.x + (warpvel.x * -1) * (g_Player.warppower * ((float)g_Player.warpframe / 30.0f));
		g_Player.warppos.y = g_Player.pos.y + (warpvel.y) * (g_Player.warppower * ((float)g_Player.warpframe / 30.0f));
	}
}

void PlayerStatusWarp(void)
{
	//ワープ処理
	g_Direction = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.beforewarppos = g_Player.pos;

	g_Player.pos.x = g_Player.warppos.x;
	g_Player.pos.y = g_Player.warppos.y;

	g_Player.afterwarppos = g_Player.pos;

	g_Player.warpframe = 0;
	g_Player.waitafterwarp = 15;
	g_Player.warpFlag--;

	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.status = normal;

	PlaySound(g_SE_wapu, 0);

	g_Frag_warpTextrue = true;

	if (g_Player.warpStartRecast == 0)
	{
		g_Player.warpStartRecast = timeGetTime();
	}
}

void PlayerStatusAttack(void)
{
	//左右移動
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	if (g_Player.vel.x != 0.0f && g_Frag_dashSE == false)
	{
		PlaySound(g_SE_dash, -1);

		g_Frag_dashSE = true;
	}
	else if (g_Player.vel.x == 0.0f)
	{
		StopSound(g_SE_dash);

		g_Frag_dashSE = false;
	}

	//ワープ処理
	if (0.0 != GetThumbRightX(TEST_CON) || 0.0 != GetThumbRightY(TEST_CON))
	{
		if (g_Player.warpFlag != 0)
		{
			g_Player.status = warpwait;
		}
	}

	//ワープリキャスト処理
	if (g_Player.warpFlag < 3)
	{
		g_Player.warpRecast = timeGetTime();

		if (g_Player.warpRecast - g_Player.warpStartRecast >= WARPRECAST)//3.000
		{
			g_Player.warpFlag++;

			if (g_Player.warpFlag == 3)
			{
				g_Player.warpStartRecast = 0;
			}
			else
			{
				g_Player.warpStartRecast = timeGetTime();
			}
		}
	}

	if (g_Player.attackflag == 3)
	{
		g_Player.status = normal;
	}

	//攻撃アタックパターンの更新
	if (g_Player.animeCounterAttack > 6)
	{
		//アニメーションパターンを切り替える
		g_Player.animeAttack++;
		//最後のアニメーションパターンを表示したらリセットする
		if (g_Player.animeAttack >= 4)
			g_Player.animeAttack = 0;

		//アニメーションカウンターのリセット
		g_Player.animeCounterAttack = 0;
	}
	g_Player.animeCounterAttack++;
}

void PlayerStatusDeath(void)
{
	StopSound(g_SE_dash);

	g_Frag_dashSE = false;

	//死亡パターンの更新
	if (g_Player.animeCounterDeath > 10)
	{
		//最後のアニメーションパターンを表示したらリセットする
		if (g_Player.animeDeath >= 5 - 1)
		{
			if (g_Player.animeCounterDeath > 60)
			{
				SetGameScene(GAMESCENE_PICTURE_OVERGAME);
			}
		}
		else
		{
			//アニメーションパターンを切り替える
			g_Player.animeDeath++;

			//アニメーションカウンターのリセット
			g_Player.animeCounterDeath = 0;
		}
	}
	g_Player.animeCounterDeath++;
}

void DrawWarpKaminari(D3DXVECTOR2 beforepos, D3DXVECTOR2 afterpos)
{
	if (g_Player.animeCounterWarpKaminari > 2)
	{
		//アニメーションパターンを切り替える
		g_Player.animeWarpKaminari++;
		//最後のアニメーションパターンを表示したらリセットする
		if (g_Player.animeWarpKaminari >= 4)
		{
			g_Player.animeWarpKaminari = 0;
			g_Frag_warpTextrue = false;
		}
		//アニメーションカウンターのリセット
		g_Player.animeCounterWarpKaminari = 0;

	}
	g_Player.animeCounterWarpKaminari++;

	D3DXVECTOR2 BeforeKaminari(beforepos.x - 120.0f, beforepos.y - 1020.0f);
	D3DXVECTOR2 AfterKaminari(afterpos.x - 120.0f, afterpos.y - 1020.0f);

	float Width = 1.0f / 13.0f;

	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();

	//雷（ビフォー）の描画
	DrawSpriteLeftTop(g_TextureWarp,
		basePos.x + BeforeKaminari.x,
		basePos.y + BeforeKaminari.y,
		240.0f, 1080.0f,
		Width * (4 + g_Player.animeWarpKaminari), 0.0f, Width, 1.0f);

	//雷（ビフォー）の描画
	DrawSpriteLeftTop(g_TextureWarp,
		basePos.x + AfterKaminari.x,
		basePos.y + AfterKaminari.y,
		240.0f, 1080.0f,
		Width * (8 - g_Player.animeWarpKaminari), 0.0f, Width, 1.0f);
}

void AdjustPlayer(D3DXVECTOR2 pos)
{
	g_Player.pos += pos;
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

D3DXVECTOR2 GetPlayerPos(void)
{
	return g_Player.pos;
}

D3DXVECTOR2 GetLeftStick(int padNo)
{
	return D3DXVECTOR2(GetThumbLeftX(padNo), GetThumbLeftY(padNo));
}

D3DXVECTOR2 GetRightStick(int padNo)
{
	return D3DXVECTOR2(GetThumbRightX(padNo), GetThumbRightY(padNo));
}