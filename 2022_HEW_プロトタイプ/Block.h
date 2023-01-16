#pragma once

#include <vector>
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BLOCK_SIZE (60.0f)

#define HIT_VERTICAL	0x00000001
#define HIT_HORIZONTAL	0x00000002
#define HIT_UP			0x00000010
#define HIT_DOWN		0x00000020
#define HIT_LEFT		0x00000040
#define HIT_RIGHT		0x00000080
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

float GetBlockHeight(void);

DWORD hitChackNormalPlayer_Block(D3DXVECTOR2 move);
DWORD HitChackEnemy_Block(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 moveVec);
DWORD HitCheckFireBall_Block(D3DXVECTOR2 pos, D3DXVECTOR2 size);
void hitChackWarpPlayer_Block(D3DXVECTOR2 move);
bool HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height,
	D3DXVECTOR2 box2pos, float box2width, float box2height);

float GetBlockHeight(void);
std::vector<std::vector<int>>* GetBlocks();
