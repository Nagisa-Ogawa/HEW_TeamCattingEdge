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
static int	g_BGMNo;		// BGMの識別子

void InitGameIllust(GAMESCENE gamescene,GAMESCENE nowscene)
{
	g_gamescene = gamescene;
	g_nowscene = nowscene;

	switch (g_gamescene)
	{
	case GAMESCENE_PICTURE_STAGE01:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ステージ1.png");
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/title.png");
		break;
	case GAMESCENE_PICTURE_STAGE02:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ステージ2.png");
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/title.png");
		break;
	case GAMESCENE_PICTURE_STAGE03:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/挿絵_ステージ3.png");
		break;
	case GAMESCENE_PICTURE_STAGE03_BOSS:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/title.png");
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/title.png");
		break;
	case GAMESCENE_PICTURE_RESULT:
		g_TextureNo = LoadTexture((char*)"data/TEXTURE/ED.png");
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
	if (IsButtonPressedX(0, XINPUT_GAMEPAD_A) || IsButtonPressedX(0, XINPUT_GAMEPAD_B))
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
			SetGameScene(GAMESCENE_BASS_FUJINRAIJIN);
			break;
		case GAMESCENE_PICTURE_STAGE03_BOSS:
			SetGameScene(GAMESCENE_BASS_FINAL);
			break;
		case GAMESCENE_PICTURE_OVERGAME:
			SetGameScene(g_nowscene);
			break;
		case GAMESCENE_PICTURE_RESULT:
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
}
