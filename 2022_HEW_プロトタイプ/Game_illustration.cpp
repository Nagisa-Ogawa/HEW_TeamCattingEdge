#include "Game_illustration.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "sound.h"
#include "inputx.h"
#include "game.h"
#include "fade.h"

static GAMESCENE g_gamescene;
static GAMESCENE g_nowscene;
static int	g_TextureNo;	// テクスチャ識別子
static int	g_TextureAnime;	// テクスチャ識別子
static int	g_BGMNo;		// BGMの識別子

int Frame;

void InitGameIllust(GAMESCENE gamescene,GAMESCENE nowscene)
{
	Frame = 0;

	g_gamescene = gamescene;
	g_nowscene = nowscene;

	switch (g_gamescene)
	{
	case GAMESCENE_PICTURE_STAGE01:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ステージ1.png");
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ボス_天狗.png");
		break;
	case GAMESCENE_PICTURE_STAGE02:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ステージ2.png");
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ボス_火車.png");
		break;
	case GAMESCENE_PICTURE_STAGE03:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ステージ3.png");
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ボス_風神.png");
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ボス_雷神.png");
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ボス_風神雷神.png");
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/GameOverBG.png");
		g_TextureAnime = LoadTexture((char*)"data/TEXTURE/GameOverChar.png");
		break;
	case GAMESCENE_PICTURE_RESULT:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/ED.png");
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/endroll.png");
		break;
	default:
		break;
	}
}

void UninitGameIllust(void)
{
}

void UpdateGameIllust(void)
{

	//スペースキーが押されたらゲームシーンへ移行する
	if (IsButtonTriggeredX(0, XINPUT_GAMEPAD_A) || IsButtonTriggeredX(0, XINPUT_GAMEPAD_B))
	{
		switch (g_gamescene)
		{
		case GAMESCENE_PICTURE_STAGE01:
			SetGameScene(GAMESCENE_STAGE_TENGU);
			break;
		case GAMESCENE_PICTURE_STAGE01_BOSS:
			SetGameScene(GAMESCENE_BASS_TENGU);
			break;
		case GAMESCENE_PICTURE_STAGE02:
			SetGameScene(GAMESCENE_STAGE_KASYA);
			break;
		case GAMESCENE_PICTURE_STAGE02_BOSS:
			SetGameScene(GAMESCENE_BASS_KASYA);
			break;
		case GAMESCENE_PICTURE_STAGE03:
			SetGameScene(GAMESCENE_PICTURE_STAGE03_FUJIN);
			break;
		case GAMESCENE_PICTURE_STAGE03_FUJIN:
			SetGameScene(GAMESCENE_BASS_FUJIN);
			break;
		case GAMESCENE_PICTURE_STAGE03_RAIJIN:
			SetGameScene(GAMESCENE_BASS_RAIJIN);
			break;
		case GAMESCENE_PICTURE_STAGE03_FINAL:
			SetGameScene(GAMESCENE_BASS_FINAL);
			break;
		case GAMESCENE_PICTURE_OVERGAME:
			SetGameScene(GAMESCENE_PICTURE_RESULT);
			break;
		case GAMESCENE_PICTURE_RESULT:
			SetGameScene(GAMESCENE_PICTURE_STAFFROLL);
			break;
		case GAMESCENE_PICTURE_STAFFROLL:
			SceneFadeIn(SCENE_TITLE);
			break;
		default:
			break;
		}
	}
}

void DrawGameIllust(void)
{
	DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);

	if (g_gamescene == GAMESCENE_PICTURE_OVERGAME)
	{
		Frame++;

		if (Frame % 30 < 10)
		{
			DrawSpriteLeftTop(g_TextureAnime, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 250, 400, 400,
				0.0f, 0.0f, (float)1 / 3, 1.0f);
		}
		else if (Frame % 30 < 20)
		{
			DrawSpriteLeftTop(g_TextureAnime, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 250, 400, 400,
				(float)1 / 3, 0.0f, (float)1 / 3, 1.0f);
		}
		else
		{
			DrawSpriteLeftTop(g_TextureAnime, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 250, 400, 400,
				(float)2 / 3, 0.0f, (float)1 / 3, 1.0f);
		}
	}
}
