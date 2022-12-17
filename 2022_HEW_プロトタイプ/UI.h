#pragma once
#include "main.h"
#include "player.h"

class UI
{
private:
	PLAYER* m_pPlayer;
	DWORD m_time;
	UI();
public:
	UI(PLAYER* m_pPlayer):m_pPlayer(m_pPlayer) {};
	void InitUI(void);
	void UpdateUI(void);
	void DrawUI(void);
	void UninitUI(void);
};

