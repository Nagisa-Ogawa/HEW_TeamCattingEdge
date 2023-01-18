#pragma once
#include "main.h"
#include "player.h"

class UI
{
private:
	PLAYER* m_pPlayer;
	DWORD m_time;

	int WarpStock = 3;
	
	UI();
public:
	UI(PLAYER* m_pPlayer):m_pPlayer(m_pPlayer) {};
	void InitUI(void);
	void UpdateUI(void);
	void DrawUI(void);
	void UninitUI(void);

	void SwitchUse();
	void Warp_Stock(int i);
};

struct GUI
{
	D3DXVECTOR2 pos;

	int Texture = 0;
	bool use = 1;
	float a = 1.0f;
};