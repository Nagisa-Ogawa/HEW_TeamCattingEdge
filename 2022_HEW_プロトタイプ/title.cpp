
/*==============================================================================

   タイトル画面 [title.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "title.h"
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
void Push();
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_BGMNo;		// BGMの識別子

static Anime LStick;
static Anime RStick;
static Anime RHituzi;
static Anime LHituzi;
static Anime Arrow;
static Anime ArrowH;

static int Frame;
static bool Warp;
static int warp;

static int BGManual;
static int Title;

static int Attack;
static int AttackFrame;

bool title;
int Apush;

bool arrow;

static float SticksW[5]=
{
	0,	0.2, 0.41, 0.61, 0.815
};

static float SticksH[3] =
{
	0.0f,(float)1 / 3,(float)2 / 3
};

static float HituziW[8] =
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

static float HituziH[7] =
{
	0.0f,(float)1/7,(float)2/7,(float)3/7,(float)4/7,(float)5/7,(float)6/7,
};

//=============================================================================
// 初期化処理
//=============================================================================
void InitTitle(void)
{
	Apush = 0;
	title = 1;

	warp = 0;
	Attack = 0;
	AttackFrame = 0;

	BGManual = LoadTexture((char*)"data/TEXTURE/bgm.png");
	Title = LoadTexture((char*)"data/TEXTURE/title.png");
	//タイトル画面用テクスチャの読み込み
	LStick.Texture = LoadTexture((char*)"data/TEXTURE/TStick.png");
	LStick.pack = D3DXVECTOR2(0.2, (float)1 / 3);
	LStick.pos = D3DXVECTOR2(200, 400);
	
	RStick.Texture = LStick.Texture;
	RStick.pack = D3DXVECTOR2(0.2, (float)1 / 3);
	RStick.pos = D3DXVECTOR2(1300, 700);

	RStick.Texture = LStick.Texture;
	RStick.pack = D3DXVECTOR2(0.2, (float)1 / 3);
	RStick.pos = D3DXVECTOR2(1300, 700);

	LHituzi.Texture = LoadTexture((char*)"data/TEXTURE/hituzi_L.png");
	LHituzi.pos = LStick.pos;
	LHituzi.pos.y -= 200;
	LHituzi.pack = D3DXVECTOR2(HituziW[1], HituziH[1]);
	LHituzi.size = D3DXVECTOR2(150, 150);

	RHituzi.Texture = LoadTexture((char*)"data/TEXTURE/hituzi_R.png");
	RHituzi.pos = LHituzi.pos;
	RHituzi.pos.x += 300;
	RHituzi.pack = D3DXVECTOR2(HituziW[1], HituziH[1]);
	RHituzi.size = D3DXVECTOR2(150, 150);

	Arrow.Texture = LoadTexture((char*)"data/TEXTURE/arrow.png");
	Arrow.pack = D3DXVECTOR2(0.5, 1);
	Arrow.pos = D3DXVECTOR2(0, RStick.pos.y);

	ArrowH.Texture =Arrow.Texture;
	ArrowH.pack = D3DXVECTOR2(0.5, 1);
	ArrowH.pos = D3DXVECTOR2(0, RStick.pos.y-200);

	//タイトル用BGMの読み込み
	g_BGMNo = LoadSound((char*)"data/BGM/sample000.wav");

	Frame = 0;

	//BGM再生(二つ目の引数はループ回数 負の値を指定すると無限ループ)
	//PlaySound(g_BGMNo, -1);
}

void UninitTitle(void)
{
	//テクスチャの解放
//	UninitTexture();

	//BGMストップ
	StopSound(g_BGMNo);
}

void UpdateTitle(void)
{
	

	//スペースキーが押されたらゲームシーンへ移行する
	if (IsButtonPressedX(0, XINPUT_GAMEPAD_A)||GetKeyboardPress(DIK_RETURN))
	{
		if (Apush %2 == 0)
		{
			Push();
		}
	}
	else if (!IsButtonPressedX(0, XINPUT_GAMEPAD_A)|| !GetKeyboardPress(DIK_RETURN))
	{
		if (!title && Apush % 2 == 1)
		{
			Apush++;
		}
	}
	
	

	if (GetKeyboardPress(DIK_SPACE))
	{
		warp++;
	}
	
	if (warp == 2 && Frame % 30 == 29 || warp >= 5 && Frame % 30 == 29)
	{
		warp++;
		if (warp >= 5)
			warp = 0;
	}

	if (warp  == 1 )
	{
		arrow = 1;
		Arrow.pos.x += 1.5f;
		ArrowH.pos.x -= 1.5f;
	}
	else if (warp == 4)
	{
		arrow = 0;
		Arrow.pos.x -= 1.5f;
		ArrowH.pos.x += 1.5f;
	}
	else
	{
		Arrow.pos.x = RStick.pos.x;
		ArrowH.pos.x = RStick.pos.x;
	}

	if (Frame % 120 == 119)
	{
		LStick.anmNext++;
		RStick.anmNext++;
		warp++;
		Attack++;
		if (LStick.anmNext >= 3)
		{
			LStick.anmNext = 0;
		}
		
		if (RStick.anmNext >= 2)
		{
			Warp = !Warp;
			RStick.anmNext = 0;
		}
		if (Attack >= 4)
		{
			Attack = 0;
		}
	}

	LHituzi.anmChange = LStick.anmNext;
	RHituzi.anmChange = LStick.anmNext;

	if (LStick.anmNext == 0)
	{
		LHituzi.anmNext = 0;
		RHituzi.anmNext = 0;
	}
	else
	{
		if (Frame % 8 == 0)
		{
			LHituzi.anmNext++;
			RHituzi.anmNext++;
		}
	}
	Frame++;
}

void DrawTitle(void)
{
	DrawSpriteLeftTop(BGManual, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);
	int a = 4 - arrow * 4;

	if (warp == 1)
	{
		DrawSpriteLeftTop(Arrow.Texture, Arrow.pos.x, Arrow.pos.y, Arrow.size.x, Arrow.size.y,
			HituziW[arrow * 4], 0, Arrow.pack.x, Arrow.pack.y);


		DrawSpriteLeftTop(ArrowH.Texture, ArrowH.pos.x, ArrowH.pos.y, ArrowH.size.x, ArrowH.size.y,
			HituziW[a], 0, ArrowH.pack.x, ArrowH.pack.y);
	}
	if (warp == 4)
	{
		DrawSpriteLeftTop(Arrow.Texture, Arrow.pos.x, Arrow.pos.y, Arrow.size.x, Arrow.size.y,
			HituziW[arrow * 4], 0, Arrow.pack.x, Arrow.pack.y);


		DrawSpriteLeftTop(ArrowH.Texture, ArrowH.pos.x, ArrowH.pos.y, ArrowH.size.x, ArrowH.size.y,
			0.5, 0, ArrowH.pack.x, ArrowH.pack.y);
	}

	DrawSpriteLeftTop(LStick.Texture, LStick.pos.x, LStick.pos.y, LStick.size.x, LStick.size.y,
		SticksW[1], SticksH[LStick.anmNext], LStick.pack.x, LStick.pack.y);							//LL

	DrawSpriteLeftTop(LStick.Texture, LStick.pos.x+300, LStick.pos.y, LStick.size.x, LStick.size.y,
		SticksW[0], SticksH[LStick.anmNext], LStick.pack.x, LStick.pack.y);							//LR

	DrawSpriteLeftTop(LHituzi.Texture, LHituzi.pos.x, LHituzi.pos.y, LHituzi.size.x, LHituzi.size.y,
		HituziW[LHituzi.anmNext % 8], HituziH[LHituzi.anmChange], LHituzi.pack.x, LHituzi.pack.y);	//HL
	
	DrawSpriteLeftTop(RHituzi.Texture, RHituzi.pos.x, RHituzi.pos.y, RHituzi.size.x, RHituzi.size.y,
		HituziW[RHituzi.anmNext % 8], HituziH[RHituzi.anmChange], RHituzi.pack.x, RHituzi.pack.y);	//HR

	DrawSpriteLeftTop(RStick.Texture, RStick.pos.x, RStick.pos.y, RStick.size.x, RStick.size.y,
		SticksW[2 + Warp], SticksH[RStick.anmNext *2], RStick.pack.x, RStick.pack.y);				//R

	switch (warp)
	{
	case 0:
		DrawSpriteLeftTop(LHituzi.Texture, RStick.pos.x + 200, RStick.pos.y - 200, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[0], LHituzi.pack.x, LHituzi.pack.y);	//HL
		break;

	case 1:
		DrawSpriteLeftTop(LHituzi.Texture, RStick.pos.x + 200, RStick.pos.y - 200, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[4], LHituzi.pack.x, LHituzi.pack.y);	//HL
		break;

	case 2:
		DrawSpriteLeftTop(LHituzi.Texture, RStick.pos.x - 200, RStick.pos.y - 200, LHituzi.size.x, LHituzi.size.y,
			HituziW[1], HituziH[4], LHituzi.pack.x, LHituzi.pack.y);	//HR
		break;

	case 3:
		DrawSpriteLeftTop(RHituzi.Texture, RStick.pos.x - 200, RStick.pos.y - 200, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[0], LHituzi.pack.x, LHituzi.pack.y);	//HR
		break;

	case 4:
		DrawSpriteLeftTop(RHituzi.Texture, RStick.pos.x - 200, RStick.pos.y - 200, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[4], LHituzi.pack.x, LHituzi.pack.y);	//HR
		break;

	case 5:
		DrawSpriteLeftTop(RHituzi.Texture, RStick.pos.x + 200, RStick.pos.y - 200, LHituzi.size.x, LHituzi.size.y,
			HituziW[1], HituziH[4], LHituzi.pack.x, LHituzi.pack.y);	//HL
		break;

	default:
		break;
	}

	

	switch (Attack)
	{
	case 0:
		AttackFrame = 0;
		DrawSpriteLeftTop(LHituzi.Texture, LStick.pos.x + 300, LStick.pos.y + 250, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[0], LHituzi.pack.x, LHituzi.pack.y);
		break;

	case 1:
		AttackFrame++;
		if (AttackFrame  < 3)
		{
		DrawSpriteLeftTop(LHituzi.Texture, LStick.pos.x + 300, LStick.pos.y + 250, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[3], LHituzi.pack.x, LHituzi.pack.y);
		}
		else if (AttackFrame  >= 3 && AttackFrame  < 6)
		{
			DrawSpriteLeftTop(LHituzi.Texture, LStick.pos.x + 300, LStick.pos.y + 250, LHituzi.size.x, LHituzi.size.y,
				HituziW[1], HituziH[3], LHituzi.pack.x, LHituzi.pack.y);
		}
		else if (AttackFrame >= 6 && AttackFrame < 9)
		{
			DrawSpriteLeftTop(LHituzi.Texture, LStick.pos.x + 300, LStick.pos.y + 250, LHituzi.size.x, LHituzi.size.y,
				HituziW[2], HituziH[3], LHituzi.pack.x, LHituzi.pack.y);
		}
		else if(AttackFrame >= 9 && AttackFrame < 24)
		{
			DrawSpriteLeftTop(LHituzi.Texture, LStick.pos.x + 300, LStick.pos.y + 250, LHituzi.size.x, LHituzi.size.y,
				HituziW[3], HituziH[3], LHituzi.pack.x, LHituzi.pack.y);
		}
		else
		{
		DrawSpriteLeftTop(LHituzi.Texture, LStick.pos.x + 300, LStick.pos.y + 250, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[0], LHituzi.pack.x, LHituzi.pack.y);
		}
		DrawSpriteLeftTop(LStick.Texture, LStick.pos.x, LStick.pos.y + 400, LStick.size.x, LStick.size.y,
			SticksW[4], SticksH[0], LStick.pack.x-0.055f, LStick.pack.y);	

		break;


	case 2:

		AttackFrame = 0;

		DrawSpriteLeftTop(RHituzi.Texture, LStick.pos.x , LStick.pos.y + 250, LHituzi.size.x, LHituzi.size.y,
			HituziW[0], HituziH[0], LHituzi.pack.x, LHituzi.pack.y);
		break;
	case 3:
		AttackFrame++;
		if (AttackFrame < 3)
		{
			DrawSpriteLeftTop(RHituzi.Texture, LStick.pos.x, LStick.pos.y + 250, RHituzi.size.x, RHituzi.size.y,
				HituziW[0], HituziH[3], RHituzi.pack.x, RHituzi.pack.y);
		}
		else if (AttackFrame >= 3 && AttackFrame < 6)
		{
			DrawSpriteLeftTop(RHituzi.Texture, LStick.pos.x , LStick.pos.y + 250, RHituzi.size.x, RHituzi.size.y,
				HituziW[1], HituziH[3], RHituzi.pack.x, RHituzi.pack.y);
		}
		else if (AttackFrame >= 6 && AttackFrame < 9)
		{
			DrawSpriteLeftTop(RHituzi.Texture, LStick.pos.x , LStick.pos.y + 250, RHituzi.size.x, RHituzi.size.y,
				HituziW[2], HituziH[3], RHituzi.pack.x, RHituzi.pack.y);
		}
		else if (AttackFrame >= 9 && AttackFrame < 24)
		{
			DrawSpriteLeftTop(RHituzi.Texture, LStick.pos.x , LStick.pos.y + 250, RHituzi.size.x, RHituzi.size.y,
				HituziW[3], HituziH[3], RHituzi.pack.x, RHituzi.pack.y);
		}
		else
		{
			DrawSpriteLeftTop(RHituzi.Texture, LStick.pos.x, LStick.pos.y + 250, RHituzi.size.x, RHituzi.size.y,
				HituziW[0], HituziH[0], RHituzi.pack.x, RHituzi.pack.y);
		}



		DrawSpriteLeftTop(LStick.Texture, LStick.pos.x+300, LStick.pos.y + 400, LStick.size.x, LStick.size.y,
			SticksW[4], SticksH[1], LStick.pack.x - 0.055f, LStick.pack.y);
		break;

	default:
		break;
	}


	if (title)
	{
		DrawSpriteLeftTop(Title, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f);
	}
}

void Push()
{
	if (title)
	{
		Apush++;
		title = 0;
	}
	else
	{
		SetFadeColor(0.0f, 0.0f, 0.0f, 0.0f);//フェードカラーを黒に変える
		SceneTransition(SCENE_GAME);

	}
}