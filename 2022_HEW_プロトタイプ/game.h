#pragma once

#include "main.h"
#include "renderer.h"
#include "BombFactory.h"
#include "ExplosionFactory.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

BombFactory* GetBombFactory();
ExplosionFactory* GetExplosionFactory();
