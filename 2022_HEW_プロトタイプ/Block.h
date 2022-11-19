#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BLOCK_SIZE (60)
#define STAGE_WIDTH (32)
#define STAGE_HEIGHT (18)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
