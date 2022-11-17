#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BULLET 100	//�e�̕\���ő吔

struct BULLET
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2	vel;	//���x�x�N�g��

	float		height;	//����
	int			muki;	//����

	int			frame;	//�L���t���[��
	int			use;	//���t���O
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR2 pos, int muki);

BULLET* GetBullet(void);
