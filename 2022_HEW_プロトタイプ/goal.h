#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct GOAL
{
	D3DXVECTOR2 pos;

	int			animePtn;
	int			animeCounter;

	int			use;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
