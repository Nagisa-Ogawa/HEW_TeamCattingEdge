#include "UI.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "camera.h"

void Warp_Recast(void);
void HP(void);
void TIME(void);

static GUI g_StatusBG;
static GUI g_Life;
static GUI g_Taiko;

CAMERA_2D* basePos;
static float g_AnimeTable[4] =
{
	0.0f,
	0.25f,
	0.5f,
	0.75f,
};

void UI::InitUI(void)
{
	m_pPlayer = GetPlayer();

	basePos = GetCamera();

	g_StatusBG.Texture = LoadTexture((char*)"data/TEXTURE/UIBG.png");
	g_Life.Texture = LoadTexture((char*)"data/TEXTURE/UILifeApple.png");
	g_Taiko.Texture = LoadTexture((char*)"data/TEXTURE/UITaiko.png");

	g_StatusBG.pos = D3DXVECTOR2(250.0f, 100.0f);
	g_Life.pos = D3DXVECTOR2(195.0f, 130.0f);
	g_Taiko.pos = D3DXVECTOR2(130, 100);
}

void UI::UpdateUI(void)
{

	if (GetKeyboardRelease(DIK_HOME))
		g_StatusBG.use = !g_StatusBG.use;

	WarpStock =3-  m_pPlayer->warpFlag;

	if (m_pPlayer->pos.x <  g_StatusBG.pos.x + 300 + basePos->pos.x && m_pPlayer->pos.y  <  g_StatusBG.pos.y + 200 + basePos->pos.y)
	{
		if (g_StatusBG.a > 0.2)
		{
			g_StatusBG.a -= 0.025;
		}
	}
	else
	{
		if (g_StatusBG.a < 1.0)
		{
			g_StatusBG.a += 0.025;
		}
	}
	g_Life.a = g_StatusBG.a;
	g_Taiko.a = g_StatusBG.a;


}

void UI::DrawUI(void)
{
	D3DXVECTOR2 basePos = GetBase();

	if (g_StatusBG.use)
	{
		DrawSpriteColor(g_StatusBG.Texture,
			g_StatusBG.pos.x, g_StatusBG.pos.y,
			400, 200,
			0,0,1,1,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StatusBG.a));
	}
	if (m_pPlayer->hp > 0)
	{
		for (int i = 0; i < m_pPlayer->hp; i++)
		{
			if (i % 2)
			{
				DrawSpriteColor(g_Life.Texture,
					g_Life.pos.x + 24 * i, g_Life.pos.y,
					50, 70,
					0, 0, 0.5f, 1,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Life.a));
			}

			if (i % 2 == 0&& i + 1 == m_pPlayer->hp)
			{
				DrawSpriteColor(g_Life.Texture,
					g_Life.pos.x + 24 * (i + 1), g_Life.pos.y,
					50, 70,
					0.5f, 0, 0.5f, 1,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Life.a));
			}
		}
	}

	DrawSpriteColor(g_Taiko.Texture,
		g_Taiko.pos.x, g_Taiko.pos.y,
		120, 120,
		g_AnimeTable[WarpStock], 0, 0.25f, 1,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Taiko.a));

}

void UI::UninitUI(void)
{

}

void UI::SwitchUse()
{

}

void UI::Warp_Stock(int i)
{
	if (i + WarpStock < 4 || i + WarpStock > -1)
	{
		WarpStock + i;
	}
}

void Warp_Recast(void)
{

}

void HP(void)
{

}

void TIME(void)
{

}
